//
// Copyright(c) Multimedia Signal Processing Group (MMSPG),
//              Ecole Polytechnique Fédérale de Lausanne (EPFL)
//              http://mmspg.epfl.ch
//              Zhou Wang
//              https://ece.uwaterloo.ca/~z70wang/
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

//
// This is an OpenCV implementation of the original Matlab implementation
// from Nikolay Ponomarenko available from http://live.ece.utexas.edu/research/quality/.
// Please refer to the following papers:
// - Z. Wang, A.C. Bovik, H.R. Sheikh, and E.P. Simoncelli, "Image quality
//   assessment: from error visibility to structural similarity," IEEE
//   Transactions on Image Processing, vol. 13, no. 4, pp. 600–612, April 2004.
//

#include "SSIM.hpp"
#include <iostream>

const float SSIM::C1 = 6.5025f;
const float SSIM::C2 = 58.5225f;

SSIM::SSIM(int h, int w) : Metric(h, w)
{
}

float SSIM::compute(const cv::Mat& original, const cv::Mat& processed)
{
	cv::Scalar res = computeSSIM(original, processed);
	return float(res.val[0]);
}

cv::Scalar SSIM::computeSSIM(const cv::Mat& img1, const cv::Mat& img2)
{

	int ht = img1.rows;
	int wt = img1.cols;
	int w = wt - 10;
	int h = ht - 10;

	cv::Mat mu1(h,w,CV_32F), mu2(h,w,CV_32F);
	cv::Mat mu1_sq(h,w,CV_32F), mu2_sq(h,w,CV_32F), mu1_mu2(h,w,CV_32F);
	cv::Mat img1_sq(ht,wt,CV_32F), img2_sq(ht,wt,CV_32F), img1_img2(ht,wt,CV_32F);
	cv::Mat sigma1_sq(h,w,CV_32F), sigma2_sq(h,w,CV_32F), sigma12(h,w,CV_32F);
	cv::Mat tmp1(h,w,CV_32F), tmp2(h,w,CV_32F), tmp3(h,w,CV_32F);
	cv::Mat ssim_map(h,w,CV_32F), cs_map(h,w,CV_32F);

	// mu1 = filter2(window, img1, 'valid');
	applyGaussianBlur(img1, mu1, 11, 1.5);

	// mu2 = filter2(window, img2, 'valid');
	applyGaussianBlur(img2, mu2, 11, 1.5);

	// mu1_sq = mu1.*mu1;
	cv::multiply(mu1, mu1, mu1_sq);
	// mu2_sq = mu2.*mu2;
	cv::multiply(mu2, mu2, mu2_sq);
	// mu1_mu2 = mu1.*mu2;
	cv::multiply(mu1, mu2, mu1_mu2);

	cv::multiply(img1, img1, img1_sq);
	cv::multiply(img2, img2, img2_sq);
	cv::multiply(img1, img2, img1_img2);

	// sigma1_sq = filter2(window, img1.*img1, 'valid') - mu1_sq;
	applyGaussianBlur(img1_sq, sigma1_sq, 11, 1.5);
	sigma1_sq -= mu1_sq;

	// sigma2_sq = filter2(window, img2.*img2, 'valid') - mu2_sq;
	applyGaussianBlur(img2_sq, sigma2_sq, 11, 1.5);
	sigma2_sq -= mu2_sq;

	// sigma12 = filter2(window, img1.*img2, 'valid') - mu1_mu2;
	applyGaussianBlur(img1_img2, sigma12, 11, 1.5);
	sigma12 -= mu1_mu2;

	// cs_map = (2*sigma12 + C2)./(sigma1_sq + sigma2_sq + C2);
	tmp1 = 2*sigma12 + C2;
	tmp2 = sigma1_sq + sigma2_sq + C2;
	cv::divide(tmp1, tmp2, cs_map);
	// ssim_map = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))./((mu1_sq + mu2_sq + C1).*(sigma1_sq + sigma2_sq + C2));
	tmp3 = 2*mu1_mu2 + C1;
	cv::multiply(tmp1, tmp3, tmp1);
	tmp3 = mu1_sq + mu2_sq + C1;
	cv::multiply(tmp2, tmp3, tmp2);
	cv::divide(tmp1, tmp2, ssim_map);

	// mssim = mean2(ssim_map);
	double mssim = cv::mean(ssim_map).val[0];
	// mcs = mean2(cs_map);
	double mcs = cv::mean(cs_map).val[0];

	cv::Scalar res(mssim, mcs);

	return res;
}

// Copy paste from above, only returning ssim_map
void SSIM::computeSSIMMap(const cv::Mat& img1, const cv::Mat& img2, const cv::Mat& ssim_map) {

    int ht = img1.rows;
    int wt = img1.cols;
    int w = wt - 10;
    int h = ht - 10;

    cv::Mat mu1(h, w, CV_32F), mu2(h, w, CV_32F);
    cv::Mat mu1_sq(h, w, CV_32F), mu2_sq(h, w, CV_32F), mu1_mu2(h, w, CV_32F);
    cv::Mat img1_sq(ht, wt, CV_32F), img2_sq(ht, wt, CV_32F), img1_img2(ht, wt, CV_32F);
    cv::Mat sigma1_sq(h, w, CV_32F), sigma2_sq(h, w, CV_32F), sigma12(h, w, CV_32F);
    cv::Mat tmp1(h, w, CV_32F), tmp2(h, w, CV_32F), tmp3(h, w, CV_32F);
    cv::Mat cs_map(h, w, CV_32F);

    // mu1 = filter2(window, img1, 'valid');
    applyGaussianBlur(img1, mu1, 11, 1.5);

    // mu2 = filter2(window, img2, 'valid');
    applyGaussianBlur(img2, mu2, 11, 1.5);

    // mu1_sq = mu1.*mu1;
    cv::multiply(mu1, mu1, mu1_sq);
    // mu2_sq = mu2.*mu2;
    cv::multiply(mu2, mu2, mu2_sq);
    // mu1_mu2 = mu1.*mu2;
    cv::multiply(mu1, mu2, mu1_mu2);

    cv::multiply(img1, img1, img1_sq);
    cv::multiply(img2, img2, img2_sq);
    cv::multiply(img1, img2, img1_img2);

    // sigma1_sq = filter2(window, img1.*img1, 'valid') - mu1_sq;
    applyGaussianBlur(img1_sq, sigma1_sq, 11, 1.5);
    sigma1_sq -= mu1_sq;

    // sigma2_sq = filter2(window, img2.*img2, 'valid') - mu2_sq;
    applyGaussianBlur(img2_sq, sigma2_sq, 11, 1.5);
    sigma2_sq -= mu2_sq;

    // sigma12 = filter2(window, img1.*img2, 'valid') - mu1_mu2;
    applyGaussianBlur(img1_img2, sigma12, 11, 1.5);
    sigma12 -= mu1_mu2;

    // cs_map = (2*sigma12 + C2)./(sigma1_sq + sigma2_sq + C2);
    tmp1 = 2 * sigma12 + C2;
    tmp2 = sigma1_sq + sigma2_sq + C2;
    cv::divide(tmp1, tmp2, cs_map);
    // ssim_map = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))./((mu1_sq + mu2_sq + C1).*(sigma1_sq + sigma2_sq + C2));
    tmp3 = 2 * mu1_mu2 + C1;
    cv::multiply(tmp1, tmp3, tmp1);
    tmp3 = mu1_sq + mu2_sq + C1;
    cv::multiply(tmp2, tmp3, tmp2);
    cv::divide(tmp1, tmp2, ssim_map);
}

// Computes the PSNR + computes a histogram of the differences, and stores it in outputHistogram (index 0 is amount of 0-differences, etc.)
void SSIM::compute_with_hist_sub(const cv::Mat& original, const cv::Mat& processed, const cv::Mat& unwatermarked, int* outputHistograms[], int amountOfBins, int blockSizes[], int amountOfBlockSizes) {
    //float minMaxSSIM = 1.0; // Relative
    float minMaxSSIM = 1.0; // Absolute
    int h = height - 10;
    int w = width - 10;
    cv::Mat ssim_map_unwatermarked(h, w, CV_32F);
    cv::Mat ssim_map_watermarked(h, w, CV_32F);

    computeSSIMMap(processed, original, ssim_map_watermarked);
    computeSSIMMap(unwatermarked, original, ssim_map_unwatermarked);

    for(int i = 0; i < amountOfBlockSizes; i++) {
        int blockSize = blockSizes[i];
        int h_blocks = (h % blockSize) ? h / blockSize + 1 : h / blockSize;
        int w_blocks = (w % blockSize) ? w / blockSize + 1 : w / blockSize;
        cv::Mat ssim_map_watermarked_blocks(h_blocks, w_blocks, CV_32F);
        cv::Mat ssim_map_unwatermarked_blocks(h_blocks, w_blocks, CV_32F);

        // Separate in blocks
        averagePerBlock(ssim_map_watermarked, ssim_map_watermarked_blocks, h, w, blockSize);
        averagePerBlock(ssim_map_unwatermarked, ssim_map_unwatermarked_blocks, h, w, blockSize);

        // Compute differences
        cv::subtract(ssim_map_watermarked_blocks, ssim_map_unwatermarked_blocks, ssim_map_watermarked_blocks);

        // RELATIVE
        //cv::divide(ssim_map_watermarked_blocks, ssim_map_unwatermarked_blocks, ssim_map_watermarked_blocks);

        // Calculate histogram of changes
        histogramMatDiff(ssim_map_watermarked_blocks, outputHistograms[i], amountOfBins, minMaxSSIM);
    }
    
}

