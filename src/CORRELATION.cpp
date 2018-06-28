#include "CORRELATION.hpp"


CORRELATION::CORRELATION(int h, int w) : Metric(h, w) {
}

float CORRELATION::compute(const cv::Mat& original, const cv::Mat& processed) {
    return compute_correlation_coefficient(original, processed);
}

float CORRELATION::compute_correlation_linear(const cv::Mat& original, const cv::Mat& processed) {
    cv::Mat tmp(height, width, CV_32F);

    // Calculate correlation
    cv::multiply(original, processed, tmp);
    return cv::sum(tmp).val[0] / (height*width);
}

float CORRELATION::compute_correlation_normalized(const cv::Mat& original, const cv::Mat& processed) {
    cv::Mat original_tmp(height, width, CV_32F);
    cv::Mat processed_tmp(height, width, CV_32F);

    // Normalize (=divide by sum of squares (= euclidean length)
    cv::normalize(original, original_tmp);
    cv::normalize(processed, processed_tmp);

    // Calculate correlation
    cv::multiply(original_tmp, processed_tmp, original_tmp);
    return cv::sum(original_tmp).val[0];
}

float CORRELATION::compute_correlation_coefficient(const cv::Mat& original, const cv::Mat& processed) {
    cv::Mat original_tmp(height, width, CV_32F);
    cv::Mat processed_tmp(height, width, CV_32F);

    // Subtract mean
    cv::subtract(original, cv::mean(original).val[0], original_tmp);
    cv::subtract(processed, cv::mean(processed).val[0], processed_tmp);

    // Normalize (=divide by sum of squares (= euclidean length))
    cv::normalize(original_tmp, original_tmp);
    cv::normalize(processed_tmp, processed_tmp);

    // Calculate correlation
    cv::multiply(original_tmp, processed_tmp, original_tmp);
    return cv::sum(original_tmp).val[0];
}

float CORRELATION::compute_correlation_coefficient(const cv::Mat& original, const cv::Mat& processed, int white_x, int white_y, int white_width, int white_height) {
  cv::Mat original_tmp = original.clone();
  cv::Mat processed_tmp = processed.clone();

  // Set whitebox all to zero
  for (int i = white_x; i < white_x + white_width; i++) {
    for (int j = white_y; j < white_y + white_height; j++) {
      original_tmp.at<float>(j, i, 0) = 0;
      processed_tmp.at<float>(j, i, 0) = 0;
    }
  }

  // Subtract mean
  cv::subtract(original_tmp, cv::mean(original_tmp).val[0], original_tmp);
  cv::subtract(processed_tmp, cv::mean(processed_tmp).val[0], processed_tmp);

  // Normalize (=divide by sum of squares (= euclidean length))
  cv::normalize(original_tmp, original_tmp);
  cv::normalize(processed_tmp, processed_tmp);

  // Calculate correlation
  cv::multiply(original_tmp, processed_tmp, original_tmp);
  return cv::sum(original_tmp).val[0];
}

/* Subtracted functions (copy paste for efficiency: no need to keep extra Mat in memory) */

float CORRELATION::compute_correlation_linear_subtract(const cv::Mat& original, const cv::Mat& processed, const cv::Mat& subtract) {
    cv::Mat original_tmp(height, width, CV_32F);
    cv::Mat processed_tmp(height, width, CV_32F);

    // Subtract
    cv::subtract(original, subtract, original_tmp);
    cv::subtract(processed, subtract, processed_tmp);

    // Calculate correlation
    cv::multiply(original_tmp, processed_tmp, original_tmp);
    return cv::sum(original_tmp).val[0] / (height*width);
}

float CORRELATION::compute_correlation_normalized_subtract(const cv::Mat& original, const cv::Mat& processed, const cv::Mat& subtract) {
    cv::Mat original_tmp(height, width, CV_32F);
    cv::Mat processed_tmp(height, width, CV_32F);

    // Subtract
    cv::subtract(original, subtract, original_tmp);
    cv::subtract(processed, subtract, processed_tmp);

    // Normalize (=divide by sum of squares (= euclidean length)
    cv::normalize(original_tmp, original_tmp);
    cv::normalize(processed_tmp, processed_tmp);

    // Calculate correlation
    cv::multiply(original_tmp, processed_tmp, original_tmp);
    return cv::sum(original_tmp).val[0];
}

float CORRELATION::compute_correlation_coefficient_subtract(const cv::Mat& original, const cv::Mat& processed, const cv::Mat& subtract) {
    cv::Mat original_tmp(height, width, CV_32F);
    cv::Mat processed_tmp(height, width, CV_32F);

    // Subtract
    cv::subtract(original, subtract, original_tmp);
    cv::subtract(processed, subtract, processed_tmp);

    // Subtract mean
    cv::subtract(original_tmp, cv::mean(original_tmp).val[0], original_tmp);
    cv::subtract(processed_tmp, cv::mean(processed_tmp).val[0], processed_tmp);

    // Normalize (=divide by sum of squares (= euclidean length)
    cv::normalize(original_tmp, original_tmp);
    cv::normalize(processed_tmp, processed_tmp);

    // Calculate correlation
    cv::multiply(original_tmp, processed_tmp, original_tmp);
    return cv::sum(original_tmp).val[0];
}