//
// Copyright(c) Multimedia Signal Processing Group (MMSPG),
//              Ecole Polytechnique Fédérale de Lausanne (EPFL)
//              http://mmspg.epfl.ch
// All rights reserved.
// Author: Philippe Hanhart (philippe.hanhart@epfl.ch)
//
// Permission is hereby granted, without written agreement and without
// license or royalty fees, to use, copy, modify, and distribute the
// software provided and its documentation for research purpose only,
// provided that this copyright notice and the original authors' names
// appear on all copies and supporting documentation.
// The software provided may not be commercially distributed.
// In no event shall the Ecole Polytechnique Fédérale de Lausanne (EPFL)
// be liable to any party for direct, indirect, special, incidental, or
// consequential damages arising out of the use of the software and its
// documentation.
// The Ecole Polytechnique Fédérale de Lausanne (EPFL) specifically
// disclaims any warranties.
// The software provided hereunder is on an "as is" basis and the Ecole
// Polytechnique Fédérale de Lausanne (EPFL) has no obligation to provide
// maintenance, support, updates, enhancements, or modifications.
//

#include "PSNR.hpp"


PSNR::PSNR(int h, int w) : Metric(h, w)
{
}

float PSNR::compute(const cv::Mat& original, const cv::Mat& processed) {
    cv::Mat tmp(height, width, CV_32F);
    cv::subtract(original, processed, tmp);
    cv::multiply(tmp, tmp, tmp);
    return float(10 * log10(255 * 255 / cv::mean(tmp).val[0]));
}

// Computes the absolute differences + computes a histogram of the differences, and stores it in outputHistogram (index 0 is amount of 0-differences, etc.)
void PSNR::compute_with_hist(const cv::Mat& original, const cv::Mat& processed, int* outputHistogram) {
	cv::Mat tmp(height,width,CV_32F);
	cv::absdiff(original, processed, tmp);

    // Extra: calculate histogram of changes
    histogramMat(tmp, outputHistogram);
}

// Computes the MSE + computes a histogram of the differences, and stores it in outputHistogram (index 0 is amount of 0-differences, etc.)
void PSNR::compute_with_hist_sub(const cv::Mat& original, const cv::Mat& processed, const cv::Mat& unwatermarked, int** outputHistograms, int amountOfBins, int blockSizes[], int amountOfBlockSizes) {
    //float minMaxMSE = 1.0; // Relative
    float minMaxMSE = 256; // Absolute
    cv::Mat tmp_processed(height, width, CV_32F);
    cv::Mat tmp_unwatermarked(height, width, CV_32F);
    // Compute diff
    //cv::subtract(processed, original, tmp_processed);
    //cv::subtract(unwatermarked, original, tmp_unwatermarked);
    // Compute abs diff
    cv::absdiff(processed, original, tmp_processed);
    cv::absdiff(unwatermarked, original, tmp_unwatermarked);

    // Compute MSE
    //cv::multiply(tmp_processed, tmp_processed, tmp_processed);
    //cv::multiply(tmp_unwatermarked, tmp_unwatermarked, tmp_unwatermarked);

    for(int i = 0; i < amountOfBlockSizes; i++) {
        int blockSize = blockSizes[i];
        int h_blocks = (height % blockSize) ? height / blockSize + 1 : height / blockSize;
        int w_blocks = (width % blockSize) ? width / blockSize + 1 : width / blockSize;
        cv::Mat tmp_processed_blocks(h_blocks, w_blocks, CV_32F);
        cv::Mat tmp_unwatermarked_blocks(h_blocks, w_blocks, CV_32F);

        // Separate in blocks
        averagePerBlock(tmp_processed, tmp_processed_blocks, height, width, blockSize);
        averagePerBlock(tmp_unwatermarked, tmp_unwatermarked_blocks, height, width, blockSize);

        // Compute differences
        cv::subtract(tmp_unwatermarked_blocks, tmp_processed_blocks, tmp_processed_blocks);

        // RELATIVE
        //cv::divide(tmp_processed_blocks, tmp_unwatermarked_blocks, tmp_processed_blocks);

        // Calculate histogram of changes
        histogramMatDiff(tmp_processed_blocks, outputHistograms[i], amountOfBins, minMaxMSE);
    }
}

