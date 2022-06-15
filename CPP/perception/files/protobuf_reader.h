#ifndef _PROTOBUF_READER_H_
#define _PROTOBUF_READER_H_

#include <cstdlib>
#include <cstring>
#include <fstream>

class ProtobufReader {
 public:
  ProtobufReader() : fp_(nullptr), buf_size_(0), buf_(nullptr) {}
  ~ProtobufReader() { Close(); }

  void* buf() { return buf_; }

  bool Open(const char* filename) {
    Close();
    fp_ = nullptr;
    fp_ = fopen(filename, "rb");
    return fp_;
  }

  template <class _Tp>
  bool Read(_Tp& data) {
    if (!fp_ || feof(fp_)) {
      return false;
    }
    int size;
    if (fread(&size, sizeof(int), 1, fp_) != 1) {
      return false;
    }
    if (buf_size_ < size) {
      buf_size_ = size;
      free(buf_);
      buf_ = malloc(buf_size_);
    }
    if (fread(buf_, 1, size, fp_) != size) {
      return false;
    }
    if (!data.ParseFromArray(buf_, size)) {
      return false;
    }
    return true;
  }

  bool Close() {
    if (fp_) {
      fclose(fp_);
    }
    return true;
  }

 private:
  FILE* fp_;
  int buf_size_;
  void* buf_;
};

#endif /* _PROTOBUF_READER_H_ */
