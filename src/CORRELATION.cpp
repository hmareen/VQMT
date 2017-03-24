#include "CORRELATION.hpp"


CORRELATION::CORRELATION(int h, int w) : Metric(h, w) {
}

float CORRELATION::compute(const cv::Mat& original, const cv::Mat& processed) {
    cv::Mat original_tmp(height, width, CV_32F);
    cv::Mat processed_tmp(height, width, CV_32F);

    // Normalize (=divide by sum of squares (= euclidean length)
    cv::normalize(original, original_tmp);
    cv::normalize(processed, processed_tmp);

    // Subtract mean
    cv::subtract(original_tmp, cv::mean(original_tmp).val[0], original_tmp);
    cv::subtract(processed_tmp, cv::mean(processed_tmp).val[0], processed_tmp);

    // Calculate correlation
    cv::multiply(original_tmp, processed_tmp, original_tmp);
    return cv::sum(original_tmp).val[0];
}

