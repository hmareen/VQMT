
/**************************************************************************

 Calculation of the correlation image quality measure.

**************************************************************************/

#ifndef CORRELATION_hpp
#define CORRELATION_hpp

#include "Metric.hpp"

class CORRELATION : protected Metric {
public:
	CORRELATION(int height, int width);
    // Default
    float compute(const cv::Mat& original, const cv::Mat& processed);
	// Compute correlation between original and processed
    float compute_correlation_linear(const cv::Mat& original, const cv::Mat& processed);
    float compute_correlation_normalized(const cv::Mat& original, const cv::Mat& processed);
    float compute_correlation_coefficient(const cv::Mat& original, const cv::Mat& processed);
    float compute_correlation_coefficient(const cv::Mat& original, const cv::Mat& processed, int white_x, int white_y, int white_width, int white_height);
    float compute_correlation_coefficient(const cv::Mat& original, const cv::Mat& processed, int h, int w);
    float compute_correlation_coefficient_binarized(const cv::Mat& original, const cv::Mat& processed, float factor_threshold_std);

    float compute_correlation_linear_subtract(const cv::Mat& original, const cv::Mat& processed, const cv::Mat& subtract);
    float compute_correlation_normalized_subtract(const cv::Mat& original, const cv::Mat& processed, const cv::Mat& subtract);
    float compute_correlation_coefficient_subtract(const cv::Mat& original, const cv::Mat& processed, const cv::Mat& subtract);
    float compute_correlation_coefficient_subtract_binarized(const cv::Mat& original, const cv::Mat& processed, const cv::Mat& subtract, float factor_threshold_std);

    float compute_correlation_coefficient_lowpass(const cv::Mat& original, const cv::Mat& processed, int lowpass_height, int lowpass_width);
    float compute_correlation_coefficient_lowpass(const cv::Mat& original, const cv::Mat& processed, int lowpass_height, int lowpass_width, int total_height, int total_width);
    float compute_correlation_coefficient_subtract_lowpass(const cv::Mat& original, const cv::Mat& processed, const cv::Mat& subtract, int lowpass_height, int lowpass_width);

    float compute_correlation_coefficient_lowpass_blocks(const cv::Mat& original, const cv::Mat& processed, int lowpass_height, int lowpass_width, int block_height, int block_width);
    float compute_correlation_coefficient_subtract_lowpass_blocks(const cv::Mat& original, const cv::Mat& processed, const cv::Mat& subtract, int lowpass_height, int lowpass_width, int block_height, int block_width);
};

#endif
