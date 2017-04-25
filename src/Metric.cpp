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

#include "Metric.hpp"
#include <iostream>
#include <algorithm>

Metric::Metric(int h, int w)
{
	height = h;
	width = w;
}

Metric::~Metric()
{

}

void Metric::histogram(cv::Mat input, int* outputHistogram, float lowerRange, float upperRange, int histSize) {
    float range[] = { lowerRange, upperRange };
    const float* histRange = { range };
    cv::Mat hist;
    cv::calcHist(&input, 1, 0, cv::Mat(), // do not use mask
        hist, 1, &histSize, &histRange);

    for(int i = 0; i < histSize; i++) {
        int binVal = (int)hist.at<float>(0, i);
        outputHistogram[i] = binVal;
    }
}

// Computes a histogram of the differences, and stores it in outputHistogram (index 0 is amount of 0-differences, etc.)
void Metric::histogramMat(cv::Mat input, int* outputHistogram) {
    int nVals = 256;
    float lowerRange = 0.0;
    float upperRange = (float) nVals;
    int histSize = nVals;
    histogram(input, outputHistogram, lowerRange, upperRange, histSize); // 8 bit: 2^8 = 256
}

// Computes a histogram of the differences, and stores it in outputHistogram (index 0 is amount of -255-differences, etc.)
// + in both negative and positive direction
void Metric::histogramMatDiff(cv::Mat input, int* outputHistogram) {
    int nVals = 256;
    float lowerRange = -(float)nVals + 1.0;
    float upperRange = (float)nVals;
    int histSize = (nVals * 2) - 1;
    histogram(input, outputHistogram, lowerRange, upperRange, histSize); // 8 bit: 2^8 = 256    
}

// Computes a histogram of the differences, and stores it in outputHistogram (index 0 is amount of -1-differences, etc.)
// + in both negative and positive direction
// + in range [-1.0, 1.0]
void Metric::histogramMatDiffFloat(cv::Mat input, int* outputHistogram, int amountOfBins) {
    int nVals = amountOfBins;
    float lowerRange = -1.0;
    float upperRange = 1.0;
    int histSize = nVals * 2;
    histogram(input, outputHistogram, lowerRange, upperRange, histSize); // 8 bit: 2^8 = 256    
}

// Computes the average per block
void Metric::averagePerBlock(cv::Mat& input, cv::Mat &output, int height, int width, int blockSize) {
    cv::Mat block;
    for(int i = 0; i*blockSize < width; i++) {
        int blockWidth = std::min(width - i, blockSize);
        for(int j = 0; j*blockSize < height; j++) {
            int blockHeight = std::min(height - j, blockSize);
            block = input(cv::Rect(i, j, blockWidth, blockHeight));
            float average = cv::mean(block).val[0];
            output.at<float>(j, i) = average;
        }
    }
}


void Metric::applyGaussianBlur(const cv::Mat& src, cv::Mat& dst, int ksize, double sigma)
{
	int invalid = (ksize-1)/2;
	cv::Mat tmp(src.rows, src.cols, CV_32F);
	cv::GaussianBlur(src, tmp, cv::Size(ksize,ksize), sigma);
	tmp(cv::Range(invalid, tmp.rows-invalid), cv::Range(invalid, tmp.cols-invalid)).copyTo(dst);
}
