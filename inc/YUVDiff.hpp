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
  void calculate_and_map_differences(const cv::Mat& original, const cv::Mat& processed, const cv::Mat & subtract, cv::Mat& diff_y, cv::Mat& diff_u, cv::Mat& diff_v, int mse_or_psnr);
  void calculate_and_subtract_differences(const cv::Mat& original, const cv::Mat& processed, const cv::Mat& extra, cv::Mat& subtract_diff);
  
  void randomly_change(const cv::Mat & original, int seed, float mean, float stdev, cv::Mat & new_noised);
  void randomly_change_non_stationary(const cv::Mat & original, int seed, float mean, float stdev, cv::Mat & new_noised);
  void change_fixed(const cv::Mat & original, int fixed_value, cv::Mat & new_fixed);

  void add_to_average_video(const cv::Mat& average_video_processed, const cv::Mat& video_to_add);
  void finish_average_video(const cv::Mat& average_video_processed, int nr_of_videos_added);

  void write_yuv(FILE* file, const cv::Mat& diff_y, const cv::Mat& diff_u, const cv::Mat& diff_v);
protected:
  int height;
  int width;
  void write_mat(FILE* file, const cv::Mat& mat);
};

#endif