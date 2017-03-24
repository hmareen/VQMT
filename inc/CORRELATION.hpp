
/**************************************************************************

 Calculation of the correlation image quality measure.

**************************************************************************/

#ifndef CORRELATION_hpp
#define CORRELATION_hpp

#include "Metric.hpp"

class CORRELATION : protected Metric {
public:
	CORRELATION(int height, int width);
	// Compute correlation between original and processed
    float compute(const cv::Mat& original, const cv::Mat& processed);
};

#endif
