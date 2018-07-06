#pragma once
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
class YUVDiff
{
public:
  YUVDiff(int h, int w);
  ~YUVDiff();
  int calculate_and_map_differences(const cv::Mat& original, const cv::Mat& processed, cv::Mat& diff_y, cv::Mat& diff_u, cv::Mat& diff_v);
protected:
  int height;
  int width;
};

