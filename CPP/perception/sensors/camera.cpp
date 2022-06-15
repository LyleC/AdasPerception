#include "camera.h"

using namespace std;

Camera::Camera(const Intrinsic intrinsic, const Extrinsic extrinsic,
               const cv::Size image_size)
    : intrinsic_(intrinsic), extrinsic_(extrinsic), image_size_(image_size) {
  intrinsic_matrix_ = BuildIntrinsicMatrix(intrinsic_);
  extrinsic_matrix_ = BuildExtrinsicMatrix(extrinsic_);
  CalcTransMatrix();
}

void Camera::CalcTransMatrix() {
  // 根据intrinsic_matrix_和extrinsic_matrix_，
  // 计算得到trans_matrix_uv2xy_和trans_matrix_xy2uv_
  cv::Mat mat_K = intrinsic_matrix_ * extrinsic_matrix_;
  cv::hconcat(mat_K.colRange(0, 2), mat_K.colRange(3, 4),
              trans_matrix_xy2uv_);  // delete col:3
  cv::invert(trans_matrix_xy2uv_, trans_matrix_uv2xy_);
}

cv::Mat Camera::BuildIntrinsicMatrix(Intrinsic &intrinsic) {
  cv::Mat mat_int = (cv::Mat_<float>(3, 4) << intrinsic_.fx, 0, intrinsic_.cx,
                     0, 0, intrinsic_.fy, intrinsic_.cy, 0, 0, 0, 1, 0);
  return mat_int;
}

cv::Mat Camera::BuildExtrinsicMatrix(Extrinsic &extrinsic) {
  // 计算得到extrinsic_matrix_
  cv::Mat euler =
      (cv::Mat_<float>(3, 1) << extrinsic.roll, extrinsic.pitch, extrinsic.yaw);
  cv::Mat mat_R;
  cv::Rodrigues(euler, mat_R, cv::noArray());
  cv::Mat mat_T =
      (cv::Mat_<float>(3, 1) << extrinsic_.X, extrinsic_.Y, extrinsic_.Z);
  cv::Mat mat_RT;
  cv::hconcat(mat_R, mat_T, mat_RT);
  //[R T;0 1]
  cv::Mat mat_01 = (cv::Mat_<float>(1, 4) << 0, 0, 0, 1);
  cv::Mat mat_RT01;
  cv::vconcat(mat_RT, mat_01, mat_RT01);
  return mat_RT01;
}

// update the parameters of the camera model after pitch angle correction
bool Camera::Update(const float delta_pitch) {
  // 根据俯仰角的瞬态校正结果，更新外参，并更新两个方向的投影转换矩阵
  extrinsic_.pitch += delta_pitch;
  extrinsic_matrix_ = BuildExtrinsicMatrix(extrinsic_);
  CalcTransMatrix();
  return true;
}

cv::Point2f Camera::ProjectPoint_UV2XY(cv::Point2f point_uv) {
  // 输入：图像坐标系的点坐标
  // 输出：世界坐标系的点坐标
  cv::Mat point_uv1 = (cv::Mat_<float>(3, 1) << point_uv.x, point_uv.y, 1);
  cv::Mat point_world = trans_matrix_uv2xy_ * point_uv1;
  cv::Point2f point_xy;
  point_xy.x =
      point_world.at<float>(0, 0) / (point_world.at<float>(2, 0) + FLT_MIN);
  point_xy.y =
      point_world.at<float>(1, 0) / (point_world.at<float>(2, 0) + FLT_MIN);
  return point_xy;
}

cv::Point2f Camera::ProjectPoint_XY2UV(cv::Point2f point_xy) {
  // 输入：世界坐标系的点坐标
  // 输出：图像坐标系的点坐标
  cv::Mat point_xy1 = (cv::Mat_<float>(3, 1) << point_xy.x, point_xy.y, 1);
  cv::Mat point_cam = trans_matrix_xy2uv_ * point_xy1;
  cv::Point2f point_uv;
  point_uv.x =
      point_cam.at<float>(0, 0) / (point_cam.at<float>(2, 0) + FLT_MIN);
  point_uv.y =
      point_cam.at<float>(1, 0) / (point_cam.at<float>(2, 0) + FLT_MIN);
  return point_uv;
}

float Camera::ProjectDistance_UV2XY(float dist_v) {
  // 输入：图像坐标系的V坐标
  // 输出：世界坐标系的Y坐标
  cv::Mat point_uv1 = (cv::Mat_<float>(3, 1) << 0, dist_v, 1);
  cv::Mat point_world = trans_matrix_uv2xy_ * point_uv1;
  float dist_x =
      point_world.at<float>(0, 0) / (point_world.at<float>(2, 0) + FLT_MIN);
  return dist_x;
}

float Camera::ProjectDistance_XY2UV(float dist_x) {
  // 输入：世界坐标系的Y坐标
  // 输出：图像坐标系的V坐标
  cv::Mat point_xy1 = (cv::Mat_<float>(3, 1) << dist_x, 0, 1);
  cv::Mat point_cam = trans_matrix_xy2uv_ * point_xy1;
  float dist_v =
      point_cam.at<float>(1, 0) / (point_cam.at<float>(2, 0) + FLT_MIN);
  return dist_v;
}

float Camera::ProjectWidth_UV2XY(float width, float prior) {
  // 输入：图像坐标系的像素宽度，目标宽度先验
  // 输出：世界坐标系的Y坐标
  float dist = intrinsic_.fx * prior / (width + FLT_MIN);
  return dist;
}

float Camera::ProjectWidth_XY2UV(float dist, float prior) {
  // 输入：世界坐标系的Y坐标，目标宽度先验
  // 输出：图像坐标系的像素宽度
  float width = intrinsic_.fx * prior / (dist + FLT_MIN);
  return width;
}

ostream &Camera::Print(ostream &os) const {
  os << "投影矩阵：" << endl;
  os << intrinsic_matrix_ << endl;
  os << extrinsic_matrix_ << endl;
  os << trans_matrix_xy2uv_ << endl;
  os << trans_matrix_uv2xy_ << endl;
  return os;
}

ostream &operator<<(ostream &os, const Camera &cam) { return cam.Print(os); }