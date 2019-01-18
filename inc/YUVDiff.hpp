#ifndef YUVDiff_hpp
#define YUVDiff_hpp

#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
class YUVDiff
{
public:
  YUVDiff(int h, int w);
  ~YUVDiff();
  void calculate_and_map_differences(const cv::Mat& original, const cv::Mat& processed, cv::Mat& diff_y, cv::Mat& diff_u, cv::Mat& diff_v, bool mse_or_psnr);
  void calculate_and_subtract_differences(const cv::Mat& original, const cv::Mat& processed, const cv::Mat& extra, cv::Mat& subtract_diff);
  void write_yuv(FILE* file, const cv::Mat& diff_y, const cv::Mat& diff_u, const cv::Mat& diff_v);
protected:
  int height;
  int width;
  void write_mat(FILE* file, const cv::Mat& mat);
};

#endif