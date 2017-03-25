
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

    float compute_correlation_linear_subtract(const cv::Mat& original, const cv::Mat& processed, const cv::Mat& subtract);
    float compute_correlation_normalized_subtract(const cv::Mat& original, const cv::Mat& processed, const cv::Mat& subtract);
    float compute_correlation_coefficient_subtract(const cv::Mat& original, const cv::Mat& processed, const cv::Mat& subtract);
};

#endif
