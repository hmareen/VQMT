#include "YUVDiff.hpp"
#include "VideoYUV.hpp"

#include <stdio.h>
#include <iostream>

#include <stdlib.h> # rand
#include <random>
#include <vector>
#include <algorithm>

YUVDiff::YUVDiff(int h, int w)
{
  height = h;
  width = w;
}


YUVDiff::~YUVDiff()
{
}

unsigned int colorMapUndershoots[] = {
  0xFF0000FF, // Blue
};

unsigned int colorMapOvershoots[] = {
  0xFFFF0000, // red
};

unsigned int colorMaps[][256] = {
  // Jet color map from blue to red
  { 0xFF000080, 0xFF000084, 0xFF000089, 0xFF00008D,
  0xFF000092, 0xFF000096, 0xFF00009B, 0xFF00009F, 0xFF0000A4, 0xFF0000A9,
  0xFF0000AD, 0xFF0000B2, 0xFF0000B6, 0xFF0000BB, 0xFF0000BF, 0xFF0000C4,
  0xFF0000C9, 0xFF0000CD, 0xFF0000D2, 0xFF0000D6, 0xFF0000DB, 0xFF0000DF,
  0xFF0000E4, 0xFF0000E8, 0xFF0000ED, 0xFF0000F2, 0xFF0000F6, 0xFF0000FB,
  0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0004FF,
  0xFF0008FF, 0xFF000CFF, 0xFF0010FF, 0xFF0014FF, 0xFF0018FF, 0xFF001CFF,
  0xFF0020FF, 0xFF0024FF, 0xFF0028FF, 0xFF002CFF, 0xFF0030FF, 0xFF0034FF,
  0xFF0038FF, 0xFF003CFF, 0xFF0040FF, 0xFF0044FF, 0xFF0048FF, 0xFF004CFF,
  0xFF0050FF, 0xFF0054FF, 0xFF0058FF, 0xFF005CFF, 0xFF0060FF, 0xFF0064FF,
  0xFF0068FF, 0xFF006CFF, 0xFF0070FF, 0xFF0074FF, 0xFF0078FF, 0xFF007CFF,
  0xFF0081FF, 0xFF0085FF, 0xFF0089FF, 0xFF008DFF, 0xFF0091FF, 0xFF0095FF,
  0xFF0099FF, 0xFF009DFF, 0xFF00A1FF, 0xFF00A5FF, 0xFF00A9FF, 0xFF00ADFF,
  0xFF00B1FF, 0xFF00B5FF, 0xFF00B9FF, 0xFF00BDFF, 0xFF00C1FF, 0xFF00C5FF,
  0xFF00C9FF, 0xFF00CDFF, 0xFF00D1FF, 0xFF00D5FF, 0xFF00D9FF, 0xFF00DDFF,
  0xFF00E1FB, 0xFF00E5F8, 0xFF02E9F5, 0xFF05EDF2, 0xFF08F1EE, 0xFF0CF5EB,
  0xFF0FF9E8, 0xFF12FDE5, 0xFF15FFE1, 0xFF19FFDE, 0xFF1CFFDB, 0xFF1FFFD8,
  0xFF22FFD4, 0xFF26FFD1, 0xFF29FFCE, 0xFF2CFFCB, 0xFF2FFFC7, 0xFF33FFC4,
  0xFF36FFC1, 0xFF39FFBE, 0xFF3CFFBB, 0xFF3FFFB7, 0xFF43FFB4, 0xFF46FFB1,
  0xFF49FFAE, 0xFF4CFFAA, 0xFF50FFA7, 0xFF53FFA4, 0xFF56FFA1, 0xFF59FF9D,
  0xFF5DFF9A, 0xFF60FF97, 0xFF63FF94, 0xFF66FF90, 0xFF6AFF8D, 0xFF6DFF8A,
  0xFF70FF87, 0xFF73FF83, 0xFF77FF80, 0xFF7AFF7D, 0xFF7DFF7A, 0xFF80FF77,
  0xFF83FF73, 0xFF87FF70, 0xFF8AFF6D, 0xFF8DFF6A, 0xFF90FF66, 0xFF94FF63,
  0xFF97FF60, 0xFF9AFF5D, 0xFF9DFF59, 0xFFA1FF56, 0xFFA4FF53, 0xFFA7FF50,
  0xFFAAFF4C, 0xFFAEFF49, 0xFFB1FF46, 0xFFB4FF43, 0xFFB7FF3F, 0xFFBBFF3C,
  0xFFBEFF39, 0xFFC1FF36, 0xFFC4FF33, 0xFFC7FF2F, 0xFFCBFF2C, 0xFFCEFF29,
  0xFFD1FF26, 0xFFD4FF22, 0xFFD8FF1F, 0xFFDBFF1C, 0xFFDEFF19, 0xFFE1FF15,
  0xFFE5FF12, 0xFFE8FF0F, 0xFFEBFF0C, 0xFFEEFF08, 0xFFF2FD05, 0xFFF5F902,
  0xFFF8F500, 0xFFFBF100, 0xFFFFEE00, 0xFFFFEA00, 0xFFFFE600, 0xFFFFE200,
  0xFFFFDF00, 0xFFFFDB00, 0xFFFFD700, 0xFFFFD400, 0xFFFFD000, 0xFFFFCC00,
  0xFFFFC800, 0xFFFFC500, 0xFFFFC100, 0xFFFFBD00, 0xFFFFBA00, 0xFFFFB600,
  0xFFFFB200, 0xFFFFAE00, 0xFFFFAB00, 0xFFFFA700, 0xFFFFA300, 0xFFFFA000,
  0xFFFF9C00, 0xFFFF9800, 0xFFFF9400, 0xFFFF9100, 0xFFFF8D00, 0xFFFF8900,
  0xFFFF8600, 0xFFFF8200, 0xFFFF7E00, 0xFFFF7A00, 0xFFFF7700, 0xFFFF7300,
  0xFFFF6F00, 0xFFFF6C00, 0xFFFF6800, 0xFFFF6400, 0xFFFF6000, 0xFFFF5D00,
  0xFFFF5900, 0xFFFF5500, 0xFFFF5100, 0xFFFF4E00, 0xFFFF4A00, 0xFFFF4600,
  0xFFFF4300, 0xFFFF3F00, 0xFFFF3B00, 0xFFFF3700, 0xFFFF3400, 0xFFFF3000,
  0xFFFF2C00, 0xFFFF2900, 0xFFFF2500, 0xFFFF2100, 0xFFFF1D00, 0xFFFF1A00,
  0xFFFF1600, 0xFFFF1200, 0xFFFB0F00, 0xFFF60B00, 0xFFF20700, 0xFFED0300,
  0xFFE80000, 0xFFE40000, 0xFFDF0000, 0xFFDB0000, 0xFFD60000, 0xFFD20000,
  0xFFCD0000, 0xFFC90000, 0xFFC40000, 0xFFBF0000, 0xFFBB0000, 0xFFB60000,
  0xFFB20000, 0xFFAD0000, 0xFFA90000, 0xFFA40000, 0xFF9F0000, 0xFF9B0000,
  0xFF960000, 0xFF920000, 0xFF8D0000, 0xFF890000, 0xFF840000, 0xFF800000,
  }

};

#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)
// RGB -> YUV
#define RGB2Y(R, G, B) CLIP(( (  66 * (R) + 129 * (G) +  25 * (B) + 128) >> 8) +  16)
#define RGB2U(R, G, B) CLIP(( ( -38 * (R) -  74 * (G) + 112 * (B) + 128) >> 8) + 128)
#define RGB2V(R, G, B) CLIP(( ( 112 * (R) -  94 * (G) -  18 * (B) + 128) >> 8) + 128)

#define RGB2Y2(R, G, B) CLIP(( (  0.299 * (R) + 0.587 * (G) +  0.114 * (B))))
#define RGB2U2(R, G, B) CLIP(( ( -0.169 * (R) -  0.331 * (G) + 0.5 * (B) + 128)))
#define RGB2V2(R, G, B) CLIP(( ( 0.5 * (R) -  0.419 * (G) -  0.081 * (B) + 128)))

void YUVDiff::calculate_and_map_differences(const cv::Mat & original, const cv::Mat & processed, const cv::Mat & subtract, cv::Mat & diff_y, cv::Mat & diff_u, cv::Mat & diff_v, int mse_or_psnr)
{
  cv::Mat tmp(height, width, CV_32F);
  // MSE
  cv::subtract(original, processed, tmp);
  cv::multiply(tmp, tmp, tmp);

  //printf("%f\n", cv::mean(tmp).val[0]);
  //bool mse_or_psnr = 0; // MSE
  //mse_or_psnr = 1; // PSNR

  float lowerRange, upperRange, biggerValueIsBetter;
  if (mse_or_psnr == 1) { // PSNR
    // c - 10.0*log10(mseData[i]);
    // = c - 10*ln(mseData)/ln(10)
    cv::Scalar c(20.0*log10(255.0));
    cv::Scalar tenDivLnTen(-10 / log(10));
    cv::log(tmp, tmp);
    cv::multiply(tmp, tenDivLnTen, tmp);
    cv::add(tmp, c, tmp);

    lowerRange = 22.0;
    upperRange = 50.0;
    biggerValueIsBetter = true;
  }
  else if (mse_or_psnr == 0) { // MSE
    lowerRange = 1.0;
    upperRange = 400.0;
    biggerValueIsBetter = false;
  }
  else if (mse_or_psnr == 2) { // corr_coef
    // Yes, this is copy-pasted from CORRELATION.cpp. Sue me.
    cv::Mat original_tmp(height, width, CV_32F);
    cv::Mat processed_tmp(height, width, CV_32F);

    // Subtract
    cv::subtract(original, subtract, original_tmp);
    cv::subtract(processed, subtract, processed_tmp);

    // Experimenting code, not working yet 

    // Subtract mean
    //cv::subtract(original_tmp, cv::mean(original_tmp).val[0], original_tmp);
    //cv::subtract(processed_tmp, cv::mean(processed_tmp).val[0], processed_tmp);

    // Normalize (=divide by sum of squares (= euclidean length)
    //cv::normalize(original_tmp, original_tmp);
    //cv::normalize(processed_tmp, processed_tmp);


    //cv::multiply(original_tmp, processed_tmp, tmp);
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        //printf("(%d, %d): %f\n", i, j, tmp.at<float>(i, j));
      }
    }

    printf("Sum: %f\n", cv::sum(tmp).val[0]);
    double min, max;
    cv::minMaxLoc(tmp, &min, &max);
    printf("Max: %f\n", max);
    printf("Min: %f\n", min);

    lowerRange = 0;
    upperRange = 400;
    biggerValueIsBetter = false;
  }
 else {
    printf("Undefined mse_or_psnr: %d\n", mse_or_psnr);
  }
 
  int colorIdx = 0; // for now there is only 1 color map
  unsigned int upperColor = (biggerValueIsBetter) ? colorMapUndershoots[colorIdx] : colorMapOvershoots[colorIdx];
  unsigned int lowerColor = (biggerValueIsBetter) ? colorMapOvershoots[colorIdx] : colorMapUndershoots[colorIdx];
  unsigned int* colormap = colorMaps[colorIdx];

  int offset = 50; // cut of last range of colors to make overshoot/undershoot more distinct
  float rangeScale = 1.0 / (upperRange - lowerRange) * (255 - offset * 2);
  unsigned int color;
  int r, g, b;
  unsigned char y, u, v;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      float v = tmp.at<float>(i, j);

      if (v >= upperRange) {
        color = upperColor;
      }
      else if (v <= lowerRange) {
        color = lowerColor;
      }
      else {
        if(biggerValueIsBetter) 
          color = colormap[(int)((upperRange - v)*rangeScale) + offset];
        else
          color = colormap[(int)((v-lowerRange)*rangeScale) + offset];
      }

      // Convert to YUV and save
      r = (color & 0x00FF0000) >> 16;
      g = (color & 0x0000FF00) >> 8;
      b = (color & 0x000000FF);
      y = RGB2Y(r, g, b);
      u = RGB2U(r, g, b);
      v = RGB2V(r, g, b);

      diff_y.at<unsigned char>(i, j) = y;

      // YUV420
      diff_u.at<unsigned char>(i/2, j/2) = u;
      diff_v.at<unsigned char>(i/2, j/2) = v;

      //YUV 444
      //diff_u.at<unsigned char>(i, j) = u;
      //diff_v.at<unsigned char>(i, j) = v;

    }
  }
}

/* Calculate the difference between original and processed, and subtract that from extra */
void YUVDiff::calculate_and_subtract_differences(const cv::Mat& original, const cv::Mat& processed, const cv::Mat& extra, cv::Mat& subtract_diff) {
  // We need to temporary convert to 16-bit signed!
  cv::Mat tmp(original.rows, original.cols, CV_16S);
  // First find the difference between original and processed
  cv::subtract(original, processed, tmp, cv::noArray(), CV_16S);

  // Then, subtract this difference from extra
  // (or actually, add, because we subtract original - processed, and not processed - original)
  cv::add(extra, tmp, subtract_diff, cv::noArray(), CV_8U);
}

void YUVDiff::randomly_change_non_stationary(const cv::Mat & original, int seed, float mean, float stdev, cv::Mat & new_noised)
{
  // First copy
  original.copyTo(new_noised);

  // Add random noise
  // srand(seed);

  stdev /= std::max(original.rows, original.cols);
  stdev *= 1.4;
  std::mt19937 de(seed);
  std::normal_distribution<float> nd(mean, stdev);

  std::vector<float> xdiff;
  std::vector<float> ydiff;
  xdiff.push_back(-50.0f);
  ydiff.push_back(42.0f);
  for (int i = 0; i < original.rows; i++) {
    ydiff.push_back(nd(de) + ydiff.back());
  }
  for (int j = 0; j < original.cols; j++) {
    xdiff.push_back(nd(de) + xdiff.back());
  }

  for (int i = 0; i < original.rows; i++) {
    for (int j = 0; j < original.cols; j++) {
      // Add noise, but clip
      new_noised.at<unsigned char>(i, j) = std::max(
        0, std::min(new_noised.at<unsigned char>(i, j) + int(xdiff[j] + ydiff[i]), 255));
    }
  }

  //for (int i = 0; i < original.rows; i++) {
  //  for (int j = 0; j < original.cols; j++) {
  //    // Add noise, but clip
  //    new_noised.at<unsigned char>(i, j) = std::max(
  //      0, std::min(new_noised.at<unsigned char>(i, j) + int(nd(de)), 255));
  //  }
  //}

}

void YUVDiff::randomly_change(const cv::Mat & original, int seed, float mean, float stdev, cv::Mat & new_noised)
{
  // First copy
  original.copyTo(new_noised);

  // Add random noise
  //srand(seed);
  std::default_random_engine de(seed);
  std::normal_distribution<float> nd(mean, stdev);
  for (int i = 0; i < original.rows; i++) {
    for (int j = 0; j < original.cols; j++) {
      // Add noise, but clip
      new_noised.at<unsigned char>(i, j) = std::max(0, std::min(new_noised.at<unsigned char>(i, j) + int(nd(de)), 255));
    }
  }
}

void YUVDiff::change_fixed(const cv::Mat & original, int fixed_value, cv::Mat & new_fixed)
{
  // Could probably be done much more efficient than double loop

  // First copy
  original.copyTo(new_fixed);

  // Set to fixed value
  for (int i = 0; i < original.rows; i++) {
    for (int j = 0; j < original.cols; j++) {
      new_fixed.at<unsigned char>(i, j) = fixed_value;
    }
  }
}


void YUVDiff::add_to_average_video(const cv::Mat& average_video_processed, const cv::Mat& video_to_add) {
  cv::add(average_video_processed, video_to_add, average_video_processed);
}

void YUVDiff::finish_average_video(const cv::Mat& average_video_processed, int nr_of_videos_added) {
  cv::divide(average_video_processed, nr_of_videos_added, average_video_processed);
}

void YUVDiff::write_yuv(FILE* file, const cv::Mat& diff_y, const cv::Mat& diff_u, const cv::Mat& diff_v) {
  write_mat(file, diff_y);
  write_mat(file, diff_u);
  write_mat(file, diff_v);
}

void YUVDiff::write_mat(FILE* file, const cv::Mat& mat) {
  unsigned int size = mat.rows*mat.cols;
  std::vector<unsigned char> array(size);
  if (mat.isContinuous()) {
    unsigned char* array = mat.data;
    fwrite(array, sizeof(unsigned char), size, file);
  }
  else {
    printf("Matrix not continuous");
  }
}