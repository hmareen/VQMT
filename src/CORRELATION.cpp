#include "CORRELATION.hpp"


CORRELATION::CORRELATION(int h, int w) : Metric(h, w) {
}

float CORRELATION::compute(const cv::Mat& original, const cv::Mat& processed) {
    return compute_correlation_coefficient(original, processed);
}

float CORRELATION::compute_correlation_linear(const cv::Mat& original, const cv::Mat& processed) {
    cv::Mat tmp(original.rows, original.cols, CV_32F);

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
  return compute_correlation_coefficient(original, processed, height, width);
}

float CORRELATION::compute_correlation_coefficient(const cv::Mat& original, const cv::Mat& processed, int h, int w) {
  cv::Mat original_tmp(h, w, CV_32F);
  cv::Mat processed_tmp(h, w, CV_32F);

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

float CORRELATION::compute_correlation_coefficient_lowpass(const cv::Mat& original, const cv::Mat& processed, int lowpass_height, int lowpass_width, int total_height, int total_width) {
  cv::Mat original_dct(height, width, CV_32F);
  cv::Mat processed_dct(height, width, CV_32F);
  cv::Mat original_lowpass(lowpass_height, lowpass_width, CV_32F, float(0));
  cv::Mat processed_lowpass(lowpass_height, lowpass_width, CV_32F, float(0));

  // Perform DCT
  cv::dct(original, original_dct);
  cv::dct(processed, processed_dct);

  // Cut off high frequencies (only copy low frequencies)
  for (int i = 0; i < lowpass_height; i++) {
    for (int j = 0; j < lowpass_width; j++) {
      original_lowpass.at<float>(i, j) = original_dct.at<float>(i, j);
      processed_lowpass.at<float>(i, j) = processed_dct.at<float>(i, j);
    }
  }

  // Perform inverse DCT
  cv::dct(original_lowpass, original_lowpass, cv::DCT_INVERSE);
  cv::dct(processed_lowpass, processed_lowpass, cv::DCT_INVERSE);

  // Calculate correlation of lowpassed
  return compute_correlation_coefficient(original_lowpass, processed_lowpass, lowpass_height, lowpass_width);
}

float CORRELATION::compute_correlation_coefficient_lowpass(const cv::Mat& original, const cv::Mat& processed, int lowpass_height, int lowpass_width) {
  return compute_correlation_coefficient_lowpass(original, processed, lowpass_height, lowpass_width, height, width);
}

float CORRELATION::compute_correlation_coefficient_subtract_lowpass(const cv::Mat& original, const cv::Mat& processed, const cv::Mat& subtract, int lowpass_height, int lowpass_width) {
  cv::Mat original_tmp(height, width, CV_32F);
  cv::Mat processed_tmp(height, width, CV_32F);

  // Subtract
  cv::subtract(original, subtract, original_tmp);
  cv::subtract(processed, subtract, processed_tmp);

  return compute_correlation_coefficient_lowpass(original_tmp, processed_tmp, lowpass_height, lowpass_width);
}

float CORRELATION::compute_correlation_coefficient_lowpass_blocks(const cv::Mat& original, const cv::Mat& processed, int lowpass_height, int lowpass_width, int block_height, int block_width) {
  cv::Mat original_dct(block_height, block_width, CV_32F);
  cv::Mat processed_dct(block_height, block_width, CV_32F);

  cv::Mat original_lowpass(lowpass_height, lowpass_width, CV_32F);
  cv::Mat processed_lowpass(lowpass_height, lowpass_width, CV_32F);
  float average_correlation = 0.0;
  for (int y = 0; y < height; y += block_height) {
    for (int x = 0; x < width; x += block_width) {
      cv::Mat original_block(original, cv::Rect(x, y, block_width, block_height));
      cv::Mat processed_block(processed, cv::Rect(x, y, block_width, block_height));

      average_correlation += compute_correlation_coefficient_lowpass(original_block, processed_block, lowpass_height, lowpass_width, block_height, block_width);
    }
  }

  average_correlation /= (height / block_height);
  average_correlation /= (width / block_width);
  
  return average_correlation;

}

float CORRELATION::compute_correlation_coefficient_subtract_lowpass_blocks(const cv::Mat& original, const cv::Mat& processed, const cv::Mat& subtract, int lowpass_height, int lowpass_width, int block_height, int block_width) {
  cv::Mat original_tmp(height, width, CV_32F);
  cv::Mat processed_tmp(height, width, CV_32F);

  // Subtract
  cv::subtract(original, subtract, original_tmp);
  cv::subtract(processed, subtract, processed_tmp);

  return compute_correlation_coefficient_lowpass_blocks(original_tmp, processed_tmp, lowpass_height, lowpass_width, block_height, block_width);
}