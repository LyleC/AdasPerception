#ifndef _PROTOBUF_WRITER_H_
#define _PROTOBUF_WRITER_H_

#ifdef __linux__
#include <unistd.h>
#endif  // LINUX
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include "google/protobuf/message.h"

template <class _Tp>
class ProtobufWriter {
 public:
  ProtobufWriter() : stop_(false) {}
  ProtobufWriter(const std::string& filename) {
    stop_ = false;
    Open(filename);
  }
  ~ProtobufWriter() {
    stop_ = true;
    if (write_thread_.joinable()) {
      write_thread_.join();
    }
    Close();
  }

  void Open(const std::string& filename) {
    Close();
    os_.open(filename, std::ios::binary | std::ios::trunc);
    write_thread_ =
        std::thread(std::mem_fn(&ProtobufWriter<_Tp>::WriteThread), this);
  }

  void Write(const _Tp& data) {
    if (!os_.is_open()) {
      return;
    }
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(data);
  }

  void WriteThread() {
    _Tp data;
    while (!stop_) {
      mutex_.lock();
      if (queue_.empty()) {
        mutex_.unlock();
#ifdef __linux__
        usleep(3 * 1000);
#else
        _sleep(3 * 1000);
#endif
        continue;
      }

      std::swap(data, queue_.front());
      queue_.pop();
      mutex_.unlock();
      union {
        int size;
        char size_ptr[sizeof(int)];
      };
      size = data.ByteSize();
      os_.write(size_ptr, sizeof(int));
      data.SerializeToOstream(&os_);
      os_.flush();
    }

    while (!queue_.empty()) {
      mutex_.lock();
      data = queue_.front();
      queue_.pop();
      mutex_.unlock();
      union {
        int size;
        char size_ptr[sizeof(int)];
      };
      size = data.ByteSize();
      os_.write(size_ptr, sizeof(int));
      data.SerializeToOstream(&os_);
      os_.flush();
    }
  }

  void Close() {
    if (os_.is_open()) {
      os_.flush();
      os_.close();
    }
  }

 private:
  std::ofstream os_;
  std::queue<_Tp> queue_;
  std::thread write_thread_;
  std::mutex mutex_;
  bool stop_;
};

#endif /* _PROTOBUF_WRITER_H_ */
