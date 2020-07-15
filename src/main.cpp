//
// Copyright(c) Multimedia Signal Processing Group (MMSPG),
//              Ecole Polytechnique Fédérale de Lausanne (EPFL)
//              http://mmspg.epfl.ch
// All rights reserved.
// Author: Philippe Hanhart (philippe.hanhart@epfl.ch)
// Adaptation by Hannes Mareen (hannes.mareen@ugent.be)
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

/**************************************************************************

 Old Usage:
  VQMT.exe OriginalVideo ProcessedVideo Height Width NumberOfFrames ChromaFormat Output Metrics

  OriginalVideo: the original video as raw YUV video file, progressively scanned, and 8 bits per sample
  ProcessedVideo: the processed video as raw YUV video file, progressively scanned, and 8 bits per sample
  Height: the height of the video
  Width: the width of the video
  NumberOfFrames: the number of frames to process
  ChromaFormat: the chroma subsampling format. 0: YUV400, 1: YUV420, 2: YUV422, 3: YUV444
  Output: the name of the output file(s)
  Metrics: the list of metrics to use
   available metrics:
   - PSNR: Peak Signal-to-Noise Ratio (PNSR)
   - HIST: PSNR + Histogram of (absolute) differences (NEW by Hannes)
   - SSIM: Structural Similarity (SSIM)
   - MSSSIM: Multi-Scale Structural Similarity (MS-SSIM)
   - VIFP: Visual Information Fidelity, pixel domain version (VIFp)
   - PSNRHVS: Peak Signal-to-Noise Ratio taking into account Contrast Sensitivity Function (CSF) (PSNR-HVS)
   - PSNRHVSM: Peak Signal-to-Noise Ratio taking into account Contrast Sensitivity Function (CSF) and between-coefficient contrast masking of DCT basis functions (PSNR-HVS-M)

Example:
   VQMT.exe original.yuv processed.yuv " " 1088 1920 250 1 results PSNR SSIM MSSSIM VIFP
   will create the following output files in CSV (comma-separated values) format:
   - results_pnsr.csv
   - results_ssim.csv
   - results_msssim.csv
   - results_vifp.csv


*************
New Usage (with adaptations Hannes):
   VQMT.exe OriginalVideo ProcessedVideo ExtraVideo Height Width White_X White_Y White_Width White_Height NumberOfFrames ChromaFormat Output Metrics

   Whitebox args: Calculates metric only over whiteboxed area
                  only works with metrics that end with _WHITE and _LOWPASS. 
                  Set to random value if not needed.
    White_X: X-coordinate of whitebox - Also used for BLOCK_WIDTH in LOWPASS_BLOCKS
    White_Y: Y-coordinate of whitebox - Also used for BLOCK_HEIGHT in LOWPASS_BLOCKS
    White_Width: Width of whitebox
    White_Height: Height of whitebox

   ExtraVideo: extra video that will first be subtracted from both OriginalVideo and ProcessedVideo.
               Will only be subtracted if used with following metrics: HIST_DIFF, SSIM_HIST_DIFF, CORRELATION_LIN, CORRELATION_NORM, CORRELATION_COEF, CORRELATION_COEF_LOWPASS, CORRELATION_COEF_LOWPASS_BLOCKS
               Set to " " if you don't use it with those metrics

   More available metrics:
    - ABS_ERR: sum of absolute differences (or sum of absolute errors)
    - ABS_ERR_WHITE: sum of absolute differences without whitebox

    - CORRELATION_LIN: linear correlation, with subtraction of ExtraVideo
    - CORRELATION_NORM: normalized correlation, with subtraction of ExtraVideo
    - CORRELATION_COEF: correlation coefficient, with subtraction of ExtraVideo
    - CORRELATION_LIN_NO_SUB: same as CORRELATION_LIN, but without subtraction
    - CORRELATION_NORM_NO_SUB: same as CORRELATION_NORM, but without subtraction
    - CORRELATION_COEF_NO_SUB: same as CORRELATION_COEF, but without subtraction

    - CORRELATION_COEF_NO_SUB_WHITE: : same as CORRELATION_COEF_NO_SUB, but only taken over whitebox
    - CORRELATION_COEF_NO_SUB_LOWPASS: same as CORRELATION_COEF_NO_SUB, but only taken over lowpassed DCT coefficients (lowpass set to whitebox width and height)
    - CORRELATION_COEF_LOWPASS: same as CORRELATION_COEF_NO_SUB, but with subtraction
    - CORRELATION_COEF_LOWPASS_BLOCKS: same as CORRELATION_COEF_LOWPASS, but DCT coefficients split in blocks (block width and block height set with white_x and white_y params),
                                       then taken over lowpass coefficients of every block  (lowpass set to whitebox width and height)
    - CORRELATION_COEF_NO_SUB_LOWPASS_BLOCKS: same as CORRELATION_COEF_LOWPASS_BLOCKS, but without subtraction

    - HIST: histogram of differences (MSE)
    - HIST_DIFF: histogram of differences (MSE) per block, for several block sizes
    - SSIM_HIST_DIFF: same as HIST_DIFF, but using SSIM

    - YUV_DIFF: creates new YUV that visualizes difference (like YUVToolkit, using PSNR)
    - YUV_DIFF_MSE: creates new YUV that visualizes difference (like YUVToolkit, using MSE)
    - YUV_SUBTRACT_DIFF: creates new YUV = extra + (original - processed)
                         For example, if processed = watermarked in uncompressed domain and extra = watermarked + compressed
                         Then this can serve as an attack to delete the watermark out of the compressed video.
    

  Example correlation coefficient (DEFAULT, USED IN MY PUBLISHED PAPER):
  VQMT.exe original.yuv processed.yuv " " 1080 1920 0 0 0 0 250 1 orig_vs_processed CORRELATION_COEF_NO_SUB

  Example Whitebox (only taking block of 20x80 pixels in lower-right corner):
  VQMT.exe original.yuv processed.yuv " " 1080 1920 1900 1000 20 80 250 1 results ABS_ERR_WHITE

  Example lowpass blocks (blocks of size 8x8, then lowpass 2x2 DCT coefficients of every block)
  VQMT.exe original.yuv processed.yuv " " 1080 1920 2 2 8 8 250 1 results CORRELATION_COEF_NO_SUB_LOWPASS_BLOCKS

  Example yuv diff (visualization of original - processed)
  VQMT.exe original.yuv processed.yuv " " 1080 1920 0 0 0 0 250 1 original_vs_processed YUV_DIFF

  Example yuv subtract diff
  VQMT.exe original.yuv processed.yuv extra.yuv 1080 1920 0 0 0 0 250 1 extra YUV_SUBTRACT_DIFF

 Notes:
 - SSIM comes for free when MSSSIM is computed (but you still need to specify it to get the output)
 - PSNRHVS and PSNRHVSM are always computed at the same time (but you still need to specify both to get the two outputs)
 - When using MSSSIM, the height and width of the video have to be multiple of 16
 - When using VIFP, the height and width of the video have to be multiple of 8

 Changes in version 1.1 (since 1.0) on 30/3/13
 - Added support for large files (>2GB)
 - Added support for different chroma sampling formats (YUV400, YUV420, YUV422, and YUV444)

**************************************************************************/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <opencv2/core/core.hpp>
#include "VideoYUV.hpp"
#include "PSNR.hpp"
#include "SSIM.hpp"
#include "MSSSIM.hpp"
#include "VIFP.hpp"
#include "PSNRHVS.hpp"
#include "CORRELATION.hpp"
#include "YUVDiff.hpp"

enum Params {
	PARAM_ORIGINAL = 1,	// Original video stream (YUV)
	PARAM_PROCESSED,	// Processed video stream (YUV)
  PARAM_EXTRA,     // Extra video (YUV) (or " " if no extra)
	PARAM_HEIGHT,		// Height
	PARAM_WIDTH,		// Width
  PARAM_WHITE_X,
  PARAM_WHITE_Y,
  PARAM_WHITE_WIDTH,
  PARAM_WHITE_HEIGHT,
	PARAM_NBFRAMES,		// Number of frames
	PARAM_CHROMA,		// Chroma format
	PARAM_RESULTS,		// Output file for results
	PARAM_METRICS,		// Metric(s) to compute
    PARAM_SIZE
};

enum Metrics {
    METRIC_PSNR = 0,
    METRIC_PSNR_U,
    METRIC_PSNR_V,
	METRIC_ABS_ERR,
  METRIC_MEAN_ABS_ERR,
  METRIC_ABS_ERR_WHITE,
    METRIC_SSIM,
    METRIC_SSIM_U,
    METRIC_SSIM_V,
    METRIC_MSSSIM,
    METRIC_VIFP,
    METRIC_VIFP_U,
    METRIC_VIFP_V,
    METRIC_PSNRHVS,
    METRIC_PSNRHVSM,
    METRIC_CORRELATION_LIN,
    METRIC_CORRELATION_NORM,
    METRIC_CORRELATION_COEF,
    METRIC_CORRELATION_LIN_NO_SUB,
    METRIC_CORRELATION_NORM_NO_SUB,
    METRIC_CORRELATION_COEF_NO_SUB,
    METRIC_CORRELATION_COEF_NO_SUB_WCHROMA,
    METRIC_CORRELATION_COEF_NO_SUB_WCHROMA_WHITE,
    METRIC_CORRELATION_COEF_NO_SUB_WHITE,
    METRIC_CORRELATION_COEF_NO_SUB_LOWPASS,
    METRIC_CORRELATION_COEF_LOWPASS,
    METRIC_CORRELATION_COEF_LOWPASS_BLOCKS,
    METRIC_CORRELATION_COEF_NO_SUB_LOWPASS_BLOCKS,
    METRIC_CORRELATION_COEF_BINARIZED,
    METRIC_CORRELATION_COEF_NO_SUB_BINARIZED,
    METRIC_EUCL_DIST_SQ,
    METRIC_EUCL_DIST_SQ_NO_SUB,
    METRIC_SIZE_1_VALUE,
    METRIC_HIST,
    METRIC_HIST_DIFF,
    METRIC_SSIM_HIST_DIFF,
    METRIC_SIZE_YUV_BEGIN,
    METRIC_YUV_DIFF,
    METRIC_YUV_DIFF_U,
    METRIC_YUV_DIFF_V,
    METRIC_YUV_DIFF_MSE,
    METRIC_YUV_DIFF_CORR_COEF,
    METRIC_YUV_SUBTRACT_DIFF,
    METRIC_YUV_CHANGE,
    METRIC_YUV_CHANGE_Y,
    METRIC_YUV_CHANGE_U,
    METRIC_YUV_CHANGE_V,
    METRIC_YUV_CHANGE_NS,
    METRIC_YUV_CHANGE_NS_Y,
    METRIC_YUV_CHANGE_NS_U,
    METRIC_YUV_CHANGE_NS_V,
    METRIC_YUV_FIXED_Y,
    METRIC_YUV_ONLY_Y,
    METRIC_YUV_ONLY_U,
    METRIC_YUV_ONLY_V,
    METRIC_YUV_U2Y,
    METRIC_YUV_V2Y,
    METRIC_YUV_AVG,
    METRIC_SIZE
};

enum Hists {
    BLOCKS1 = 0,
    BLOCKS2,
    BLOCKS4,
    BLOCKS8,
    BLOCKS16,
    HISTS_SIZE
};

int main (int argc, const char *argv[])
{
	// Check number of input parameters
	if (argc < PARAM_SIZE) {
		fprintf(stderr, "Check software usage: at least %d parameters are required.\n", PARAM_SIZE);
		return EXIT_FAILURE;
	}

  bool with_chroma = false;

	double duration = static_cast<double>(cv::getTickCount());

	// Input parameters
	char *endptr = NULL;
	int height = static_cast<int>(strtol(argv[PARAM_HEIGHT], &endptr, 10));
	if (*endptr) {
		fprintf(stderr, "Incorrect value for video height: %s\n", argv[PARAM_HEIGHT]);
		return EXIT_FAILURE;
	}
	int width = static_cast<int>(strtol(argv[PARAM_WIDTH], &endptr, 10));
	if (*endptr) {
		fprintf(stderr, "Incorrect value for video width: %s\n", argv[PARAM_WIDTH]);
		return EXIT_FAILURE;
	}
  int white_x = static_cast<int>(strtol(argv[PARAM_WHITE_X], &endptr, 10));
  if (*endptr) {
    fprintf(stderr, "Incorrect value for video white X: %s\n", argv[PARAM_WHITE_X]);
    return EXIT_FAILURE;
  }
  int white_y = static_cast<int>(strtol(argv[PARAM_WHITE_Y], &endptr, 10));
  if (*endptr) {
    fprintf(stderr, "Incorrect value for video white Y: %s\n", argv[PARAM_WHITE_Y]);
    return EXIT_FAILURE;
  }
  int white_height = static_cast<int>(strtol(argv[PARAM_WHITE_HEIGHT], &endptr, 10));
  if (*endptr) {
    fprintf(stderr, "Incorrect value for video white height: %s\n", argv[PARAM_WHITE_HEIGHT]);
    return EXIT_FAILURE;
  }
  int white_width = static_cast<int>(strtol(argv[PARAM_WHITE_WIDTH], &endptr, 10));
  if (*endptr) {
    fprintf(stderr, "Incorrect value for video white width: %s\n", argv[PARAM_WHITE_WIDTH]);
    return EXIT_FAILURE;
  }
	int nbframes = static_cast<int>(strtol(argv[PARAM_NBFRAMES], &endptr, 10));
	if (*endptr) {
		fprintf(stderr, "Incorrect value for number of frames: %s\n", argv[PARAM_NBFRAMES]);
		return EXIT_FAILURE;
	}
	int chroma = static_cast<int>(strtol(argv[PARAM_CHROMA], &endptr, 10));
	if (*endptr) {
		fprintf(stderr, "Incorrect value for chroma: %s\n", argv[PARAM_CHROMA]);
		return EXIT_FAILURE;
	}

  // Input video streams
  VideoYUV *original = new VideoYUV(argv[PARAM_ORIGINAL], height, width, nbframes, chroma);
  VideoYUV *processed = new VideoYUV(argv[PARAM_PROCESSED], height, width, nbframes, chroma);

  VideoYUV *extra;
  if (strlen(argv[PARAM_EXTRA]) > 1) {
    extra = new VideoYUV(argv[PARAM_EXTRA], height, width, nbframes, chroma);
  }

	// Output files for results
	FILE *result_file[METRIC_SIZE] = {NULL};
    FILE *result_file_hist_psnr[HISTS_SIZE] = { NULL };
    FILE *result_file_hist_ssim[HISTS_SIZE] = { NULL };

    bool is_create_average = false;

	char *str = new char[256];
	for (int i=7; i<argc; i++) {
		if (strcmp(argv[i], "PSNR") == 0) {
			sprintf(str, "%s_psnr.csv", argv[PARAM_RESULTS]);
			result_file[METRIC_PSNR] = fopen(str, "w");
		}
    else if (strcmp(argv[i], "PSNR_U") == 0) {
      sprintf(str, "%s_psnr_u.csv", argv[PARAM_RESULTS]);
      result_file[METRIC_PSNR_U] = fopen(str, "w");
      with_chroma = true;
    }
    else if (strcmp(argv[i], "PSNR_V") == 0) {
      sprintf(str, "%s_psnr_v.csv", argv[PARAM_RESULTS]);
      result_file[METRIC_PSNR_V] = fopen(str, "w");
      with_chroma = true;
    }
		else if (strcmp(argv[i], "ABS_ERR") == 0) {
			sprintf(str, "%s_abs_err.csv", argv[PARAM_RESULTS]);
			result_file[METRIC_ABS_ERR] = fopen(str, "w");
		}
    else if (strcmp(argv[i], "MEAN_ABS_ERR") == 0) {
      sprintf(str, "%s_mae.csv", argv[PARAM_RESULTS]);
      result_file[METRIC_MEAN_ABS_ERR] = fopen(str, "w");
    }
    else if (strcmp(argv[i], "ABS_ERR_WHITE") == 0) {
      sprintf(str, "%s_abs_err_white.csv", argv[PARAM_RESULTS]);
      result_file[METRIC_ABS_ERR_WHITE] = fopen(str, "w");
    }
        else if(strcmp(argv[i], "HIST") == 0) {
            sprintf(str, "%s_hist.csv", argv[PARAM_RESULTS]);
            result_file[METRIC_HIST] = fopen(str, "w");
        }
        else if(strcmp(argv[i], "HIST_DIFF") == 0) {
            sprintf(str, "%s_hist_diff.csv", argv[PARAM_RESULTS]);
            //result_file[METRIC_HIST_DIFF] = fopen(str, "w");
            for(int i = 0; i < HISTS_SIZE; i++) {
                int blockSize = pow(2, i);
                sprintf(str, "%s_hist_diff_b%d.csv", argv[PARAM_RESULTS], blockSize);
                result_file_hist_psnr[i] = fopen(str, "w");
            }
        }
		else if (strcmp(argv[i], "SSIM") == 0) {
			sprintf(str, "%s_ssim.csv", argv[PARAM_RESULTS]);
			result_file[METRIC_SSIM] = fopen(str, "w");
		}
    else if (strcmp(argv[i], "SSIM_U") == 0) {
      sprintf(str, "%s_ssim_u.csv", argv[PARAM_RESULTS]);
      result_file[METRIC_SSIM_U] = fopen(str, "w");
      with_chroma = true;
    }
    else if (strcmp(argv[i], "SSIM_V") == 0) {
      sprintf(str, "%s_ssim_v.csv", argv[PARAM_RESULTS]);
      result_file[METRIC_SSIM_V] = fopen(str, "w");
      with_chroma = true;
    }
		else if (strcmp(argv[i], "MSSSIM") == 0) {
			sprintf(str, "%s_msssim.csv", argv[PARAM_RESULTS]);
			result_file[METRIC_MSSSIM] = fopen(str, "w");
		}
        else if(strcmp(argv[i], "SSIM_HIST_DIFF") == 0) {
            sprintf(str, "%s_ssim_hist_diff.csv", argv[PARAM_RESULTS]);
            //result_file[METRIC_SSIM_HIST_DIFF] = fopen(str, "w");
            for(int i = 0; i < HISTS_SIZE; i++) {
                int blockSize = pow(2, i);
                sprintf(str, "%s_ssim_hist_diff_b%d.csv", argv[PARAM_RESULTS], blockSize);
                result_file_hist_ssim[i] = fopen(str, "w");
            }
        }
		else if (strcmp(argv[i], "VIFP") == 0) {
			sprintf(str, "%s_vifp.csv", argv[PARAM_RESULTS]);
			result_file[METRIC_VIFP] = fopen(str, "w");
		}
    else if (strcmp(argv[i], "VIFP_U") == 0) {
      sprintf(str, "%s_vifp_u.csv", argv[PARAM_RESULTS]);
      result_file[METRIC_VIFP_U] = fopen(str, "w");
      with_chroma = true;
    }
    else if (strcmp(argv[i], "VIFP_V") == 0) {
      sprintf(str, "%s_vifp_v.csv", argv[PARAM_RESULTS]);
      result_file[METRIC_VIFP_V] = fopen(str, "w");
      with_chroma = true;
    }
		else if (strcmp(argv[i], "PSNRHVS") == 0) {
			sprintf(str, "%s_psnrhvs.csv", argv[PARAM_RESULTS]);
			result_file[METRIC_PSNRHVS] = fopen(str, "w");
		}
		else if (strcmp(argv[i], "PSNRHVSM") == 0) {
			sprintf(str, "%s_psnrhvsm.csv", argv[PARAM_RESULTS]);
			result_file[METRIC_PSNRHVSM] = fopen(str, "w");
        }
        else if(strcmp(argv[i], "CORRELATION_LIN") == 0) {
            sprintf(str, "%s_corr_lin.csv", argv[PARAM_RESULTS]);
            result_file[METRIC_CORRELATION_LIN] = fopen(str, "w");
        }
        else if(strcmp(argv[i], "CORRELATION_NORM") == 0) {
            sprintf(str, "%s_corr_norm.csv", argv[PARAM_RESULTS]);
            result_file[METRIC_CORRELATION_NORM] = fopen(str, "w");
        }
        else if(strcmp(argv[i], "CORRELATION_COEF") == 0) {
            sprintf(str, "%s_corr_coef.csv", argv[PARAM_RESULTS]);
            result_file[METRIC_CORRELATION_COEF] = fopen(str, "w");
        }
        else if(strcmp(argv[i], "CORRELATION_LIN_NO_SUB") == 0) {
            sprintf(str, "%s_corr_lin_no_sub.csv", argv[PARAM_RESULTS]);
            result_file[METRIC_CORRELATION_LIN_NO_SUB] = fopen(str, "w");
        } else if(strcmp(argv[i], "CORRELATION_NORM_NO_SUB") == 0) {
            sprintf(str, "%s_corr_norm_no_sub.csv", argv[PARAM_RESULTS]);
            result_file[METRIC_CORRELATION_NORM_NO_SUB] = fopen(str, "w");
        } else if(strcmp(argv[i], "CORRELATION_COEF_NO_SUB") == 0) {
            sprintf(str, "%s_corr_coef_no_sub.csv", argv[PARAM_RESULTS]);
            result_file[METRIC_CORRELATION_COEF_NO_SUB] = fopen(str, "w");
        } else if (strcmp(argv[i], "CORRELATION_COEF_NO_SUB_WCHROMA") == 0) {
          with_chroma = true;
          sprintf(str, "%s_corr_coef_no_sub_wchroma.csv", argv[PARAM_RESULTS]);
          result_file[METRIC_CORRELATION_COEF_NO_SUB_WCHROMA] = fopen(str, "w");
        }
        else if (strcmp(argv[i], "CORRELATION_COEF_NO_SUB_WCHROMA_WHITE") == 0) {
          with_chroma = true;
          sprintf(str, "%s_corr_coef_no_sub_wchroma_white_%d_%d_%d_%d.csv", argv[PARAM_RESULTS], white_x, white_y, white_width, white_height);
          result_file[METRIC_CORRELATION_COEF_NO_SUB_WCHROMA] = fopen(str, "w");
        } else if (strcmp(argv[i], "CORRELATION_COEF_NO_SUB_WHITE") == 0) {
          sprintf(str, "%s_corr_coef_no_sub_white_%d_%d_%d_%d.csv", argv[PARAM_RESULTS], white_x, white_y, white_width, white_height);
          result_file[METRIC_CORRELATION_COEF_NO_SUB_WHITE] = fopen(str, "w");
        } else if (strcmp(argv[i], "CORRELATION_COEF_LOWPASS") == 0) {
          sprintf(str, "%s_corr_coef_lowpass_%d_%d.csv", argv[PARAM_RESULTS], white_height, white_width);
          result_file[METRIC_CORRELATION_COEF_LOWPASS] = fopen(str, "w");
        } else if (strcmp(argv[i], "CORRELATION_COEF_NO_SUB_LOWPASS") == 0) {
          sprintf(str, "%s_corr_coef_no_sub_lowpass_%d_%d.csv", argv[PARAM_RESULTS], white_height, white_width);
          result_file[METRIC_CORRELATION_COEF_NO_SUB_LOWPASS] = fopen(str, "w");
        } else if (strcmp(argv[i], "CORRELATION_COEF_LOWPASS_BLOCKS") == 0) {
          sprintf(str, "%s_corr_coef_lowpass_%d_%d_blocks_%d_%d.csv", argv[PARAM_RESULTS], white_height, white_width, white_x, white_y);
          result_file[METRIC_CORRELATION_COEF_LOWPASS_BLOCKS] = fopen(str, "w");
        } else if (strcmp(argv[i], "CORRELATION_COEF_NO_SUB_LOWPASS_BLOCKS") == 0) {
          sprintf(str, "%s_corr_coef_no_sub_lowpass_%d_%d_blocks_%d_%d.csv", argv[PARAM_RESULTS], white_height, white_width, white_x, white_y);
          result_file[METRIC_CORRELATION_COEF_NO_SUB_LOWPASS_BLOCKS] = fopen(str, "w");
        } else if (strcmp(argv[i], "CORRELATION_COEF_NO_SUB_BINARIZED") == 0) {
          sprintf(str, "%s_corr_coef_no_sub_binarized.csv", argv[PARAM_RESULTS]);
          result_file[METRIC_CORRELATION_COEF_NO_SUB_BINARIZED] = fopen(str, "w");
        } else if (strcmp(argv[i], "EUCL_DIST_SQ") == 0) {
          sprintf(str, "%s_eucl_dist_sq.csv", argv[PARAM_RESULTS]);
          result_file[METRIC_EUCL_DIST_SQ] = fopen(str, "w");
        } else if (strcmp(argv[i], "EUCL_DIST_SQ_NO_SUB") == 0) {
        sprintf(str, "%s_eucl_dist_sq_no_sub.csv", argv[PARAM_RESULTS]);
        result_file[METRIC_EUCL_DIST_SQ_NO_SUB] = fopen(str, "w");
        } else if (strcmp(argv[i], "CORRELATION_COEF_NO_SUB") == 0) {
        sprintf(str, "%s_corr_coef_no_sub.csv", argv[PARAM_RESULTS]);
        result_file[METRIC_CORRELATION_COEF_NO_SUB] = fopen(str, "w");
        } else if (strcmp(argv[i], "YUV_DIFF") == 0) {
          sprintf(str, "%s_diff.yuv", argv[PARAM_RESULTS]);
          result_file[METRIC_YUV_DIFF] = fopen(str, "wb");
        } else if (strcmp(argv[i], "YUV_DIFF_U") == 0) {
          sprintf(str, "%s_diff_u.yuv", argv[PARAM_RESULTS]);
          result_file[METRIC_YUV_DIFF_U] = fopen(str, "wb");
        } else if (strcmp(argv[i], "YUV_DIFF_V") == 0) {
          sprintf(str, "%s_diff_v.yuv", argv[PARAM_RESULTS]);
          result_file[METRIC_YUV_DIFF_V] = fopen(str, "wb");
        } else if (strcmp(argv[i], "YUV_DIFF_MSE") == 0) {
          sprintf(str, "%s_diff_mse.yuv", argv[PARAM_RESULTS]);
          result_file[METRIC_YUV_DIFF_MSE] = fopen(str, "wb");
        } else if (strcmp(argv[i], "YUV_DIFF_CORR_COEF") == 0) {
          sprintf(str, "%s_diff_corr_coef.yuv", argv[PARAM_RESULTS]);
          result_file[METRIC_YUV_DIFF_CORR_COEF] = fopen(str, "wb");
        }
        else if (strcmp(argv[i], "YUV_SUBTRACT_DIFF") == 0) {
          sprintf(str, "%s_subtract_diff.yuv", argv[PARAM_RESULTS]);
          result_file[METRIC_YUV_SUBTRACT_DIFF] = fopen(str, "wb");
        }
        else if (strcmp(argv[i], "YUV_CHANGE") == 0) {
          sprintf(str, "%s_change_%d_%d.yuv", argv[PARAM_RESULTS], white_x, white_y);
          result_file[METRIC_YUV_CHANGE] = fopen(str, "wb");
        }
        else if (strcmp(argv[i], "YUV_CHANGE_Y") == 0) {
        sprintf(str, "%s_change_y_%d_%d.yuv", argv[PARAM_RESULTS], white_x, white_y);
        result_file[METRIC_YUV_CHANGE_Y] = fopen(str, "wb");
        } else if (strcmp(argv[i], "YUV_CHANGE_U") == 0) {
          sprintf(str, "%s_change_u_%d_%d.yuv", argv[PARAM_RESULTS]);
          result_file[METRIC_YUV_CHANGE_U] = fopen(str, "wb");
        } else if (strcmp(argv[i], "YUV_CHANGE_V") == 0) {
          sprintf(str, "%s_change_v_%d_%d.yuv", argv[PARAM_RESULTS]);
          result_file[METRIC_YUV_CHANGE_V] = fopen(str, "wb");
        }
        else if (strcmp(argv[i], "YUV_CHANGE_NS") == 0) {
        sprintf(str, "%s_change_ns_%d_%d.yuv", argv[PARAM_RESULTS], white_x, white_y);
        result_file[METRIC_YUV_CHANGE_NS] = fopen(str, "wb");
        }
        else if (strcmp(argv[i], "YUV_CHANGE_NS_Y") == 0) {
        sprintf(str, "%s_change_ns_y_%d_%d.yuv", argv[PARAM_RESULTS], white_x, white_y);
        result_file[METRIC_YUV_CHANGE_NS_Y] = fopen(str, "wb");
        }
        else if (strcmp(argv[i], "YUV_CHANGE_NS_U") == 0) {
        sprintf(str, "%s_change_ns_u_%d_%d.yuv", argv[PARAM_RESULTS]);
        result_file[METRIC_YUV_CHANGE_NS_U] = fopen(str, "wb");
        }
        else if (strcmp(argv[i], "YUV_CHANGE_NS_V") == 0) {
        sprintf(str, "%s_change_ns_v_%d_%d.yuv", argv[PARAM_RESULTS]);
        result_file[METRIC_YUV_CHANGE_NS_V] = fopen(str, "wb");
        } else if (strcmp(argv[i], "YUV_FIXED_Y") == 0) {
          sprintf(str, "%s_fixed_y_%d.yuv", argv[PARAM_RESULTS], white_x);
          result_file[METRIC_YUV_FIXED_Y] = fopen(str, "wb");
        } else if (strcmp(argv[i], "YUV_ONLY_Y") == 0) {
          sprintf(str, "%s_only_y_%d_%d.yuv", argv[PARAM_RESULTS], white_x, white_y);
          result_file[METRIC_YUV_ONLY_Y] = fopen(str, "wb");
        } else if (strcmp(argv[i], "YUV_ONLY_U") == 0) {
        sprintf(str, "%s_only_u_%d_%d.yuv", argv[PARAM_RESULTS], white_x, white_y);
        result_file[METRIC_YUV_ONLY_U] = fopen(str, "wb");
        } else if (strcmp(argv[i], "YUV_ONLY_V") == 0) {
        sprintf(str, "%s_only_v_%d_%d.yuv", argv[PARAM_RESULTS], white_x, white_y);
        result_file[METRIC_YUV_ONLY_V] = fopen(str, "wb");
        } else if (strcmp(argv[i], "YUV_U2Y") == 0) {
        sprintf(str, "%s_u2y_%d_%d.yuv", argv[PARAM_RESULTS], white_x, white_y);
        result_file[METRIC_YUV_U2Y] = fopen(str, "wb");
        } else if (strcmp(argv[i], "YUV_V2Y") == 0) {
        sprintf(str, "%s_v2y_%d_%d.yuv", argv[PARAM_RESULTS], white_x, white_y);
        result_file[METRIC_YUV_V2Y] = fopen(str, "wb");
        }
        else if (strcmp(argv[i], "YUV_AVG") == 0) {
        is_create_average = true;
        sprintf(str, "%s_avg_%d_%d.yuv", argv[PARAM_RESULTS], white_x, white_y);
        result_file[METRIC_YUV_AVG] = fopen(str, "wb");
        }
	}
	delete[] str;


  std::vector<VideoYUV*> videosToAverage;
  //std::vector<cv::Mat> videosToAverageFrames;
  //std::vector<cv::Mat> videosToAverageFramesU;
  //std::vector<cv::Mat> videosToAverageFramesV;
  
  int nr_video_to_average_begin = white_x;
  int nr_video_to_average_end = white_y;
  int nr_of_videos_to_average = nr_video_to_average_end - nr_video_to_average_begin + 1;

  if (is_create_average) {
    for (int i = nr_video_to_average_begin; i <= nr_video_to_average_end; i++) {
      std::string videoToAverageName = std::string(argv[PARAM_ORIGINAL]) + std::to_string(i) + std::string(argv[PARAM_PROCESSED]);
      //std::cout << videoToAverageName << "\n";
      videosToAverage.push_back(new VideoYUV(videoToAverageName.c_str(), height, width, nbframes, chroma));
      //videosToAverageFrames.push_back(cv::Mat(height, width, CV_16U));
    }
  }

	// Check size for VIFp downsampling
	if (result_file[METRIC_VIFP] != NULL && (height % 8 != 0 || width % 8 != 0)) {
		fprintf(stderr, "VIFp: 'height' and 'width' have to be multiple of 8.\n");
		exit(EXIT_FAILURE);
	}
  /*
  if (result_file[METRIC_VIFP_U] != NULL && (height/2) % 8 != 0 || (width/2) % 8 != 0) {
    fprintf(stderr, "VIFp U: U-channel 'height' and 'width' have to be multiple of 8.\n");
    exit(EXIT_FAILURE);
  }
  */
	// Check size for MS-SSIM downsampling
	if (result_file[METRIC_MSSSIM] != NULL && (height % 16 != 0 || width % 16 != 0)) {
		fprintf(stderr, "MS-SSIM: 'height' and 'width' have to be multiple of 16.\n");
		exit(EXIT_FAILURE);
	}

	// Print header to file
	for (int m=0; m<METRIC_SIZE_1_VALUE; m++) {
		if (result_file[m] != NULL) {
			fprintf(result_file[m], "frame,value\n");
		}
	}

    // Extra: print header of quality histograms
    if(result_file[METRIC_HIST] != NULL) {
        fprintf(result_file[METRIC_HIST], "frame");
        for(int i = 0; i < 256; i++) {
            fprintf(result_file[METRIC_HIST], ",%d", i);
        }
        fprintf(result_file[METRIC_HIST], "\n");
    }

    // Extra: histograms sub with blocksizes
    int blockSizes[HISTS_SIZE] = { 0 };
    for(int i = 0; i < HISTS_SIZE; i++) {
        blockSizes[i] = pow(2, i);
    }
    const int amountOfPSNRBins = 16;
    float minMaxRelPSNR = 256;
    const int amountOfSSIMBins = 20;
    float minMaxRelSSIM = 1.0;
    for(int i = 0; i < HISTS_SIZE; i++) {
        if(result_file_hist_psnr[i] != NULL) {
            fprintf(result_file_hist_psnr[i], "frame");
            for(int j = 0; j < amountOfPSNRBins * 2; j++) {
                fprintf(result_file_hist_psnr[i], ",%f", (-minMaxRelPSNR + j * (minMaxRelPSNR / amountOfPSNRBins)));
            }
            fprintf(result_file_hist_psnr[i], "\n");
        }

        if(result_file_hist_ssim[i] != NULL) {
            fprintf(result_file_hist_ssim[i], "frame");
            for(float j = 0; j < amountOfSSIMBins * 2; j++) {
                fprintf(result_file_hist_ssim[i], ",%f", (-minMaxRelSSIM + j * minMaxRelSSIM / amountOfSSIMBins));
            }
            fprintf(result_file_hist_ssim[i], "\n");
        }
    }

	PSNR *psnr     = new PSNR(height, width);
	SSIM *ssim     = new SSIM(height, width);
  SSIM *ssim_u = new SSIM(height/2, width/2);
	MSSSIM *msssim = new MSSSIM(height, width);
	VIFP *vifp     = new VIFP(height, width);
  VIFP *vifp_u = new VIFP(height/2, width/2);
	PSNRHVS *phvs  = new PSNRHVS(height, width);
    CORRELATION *correlation = new CORRELATION(height, width);
    YUVDiff *yuvDiff = new YUVDiff(height, width);

    cv::Mat original_frame(height, width, CV_32F), processed_frame(height, width, CV_32F);
    cv::Mat extra_frame(height, width, CV_32F);
    
    int u_height = original->comp_height[1];
    int u_width = original->comp_width[1];
    int v_height = original->comp_height[2];
    int v_width = original->comp_width[2];
    YUVDiff *yuvDiff_uv = new YUVDiff(u_height, u_width);

    // CHROMA
    CORRELATION *correlation_chroma_u = new CORRELATION(u_height, u_width);
    cv::Mat original_frame_chroma_u(u_height, u_width, CV_32F), processed_frame_chroma_u(u_height, u_width, CV_32F);
    cv::Mat extra_frame_chroma_u(u_height, u_width, CV_32F);
    CORRELATION *correlation_chroma_v = new CORRELATION(v_height, v_width);
    cv::Mat original_frame_chroma_v(v_height, v_width, CV_32F), processed_frame_chroma_v(v_height, v_width, CV_32F);
    cv::Mat extra_frame_chroma_v(v_height, v_width, CV_32F);

    // for YUV_DIFF_ variants
    cv::Mat y(height, width, CV_8U);
    //cv::Mat u(height, width, CV_8U), v(height, width, CV_8U); // YUV444 output
    cv::Mat u(u_height, u_width, CV_8U), v(v_height, v_width, CV_8U); // YUV420 output

	  float result[METRIC_SIZE_1_VALUE] = {0};
    float result_avg[METRIC_SIZE_1_VALUE] = { 0 };
    bool result_non_inf[METRIC_SIZE_1_VALUE] = { false };

    int histBuffer[256] = { 0 };
    int maxHistBuffer[256] = { 0 };
    int* histSubBuffers[HISTS_SIZE];
    int* maxHistSubBuffers[HISTS_SIZE];
    int* histSSIMSubBuffers[HISTS_SIZE];
    int* maxHistSSIMSubBuffers[HISTS_SIZE];

    // Initialize
    for(int i = 0; i < HISTS_SIZE; i++) {
        int n = amountOfPSNRBins * 2;
        histSubBuffers[i] = new int[n];
        memset(histSubBuffers[i], 0, n*sizeof(int));
        maxHistSubBuffers[i] = new int[n];
        memset(maxHistSubBuffers[i], 0, n*sizeof(int));

        n = amountOfSSIMBins * 2;
        histSSIMSubBuffers[i] = new int[n];
        memset(histSSIMSubBuffers[i], 0, n*sizeof(int));
        maxHistSSIMSubBuffers[i] = new int[n];
        memset(maxHistSSIMSubBuffers[i], 0, n*sizeof(int));
    }

	for (int frame=0; frame<nbframes; frame++) {
    //printf("Frame %d\n", frame);
		// Grab frame
    if (!is_create_average) {
      if (!original->readOneFrame()) exit(EXIT_FAILURE);
      original->getLuma(original_frame, CV_32F);
      if (!processed->readOneFrame()) exit(EXIT_FAILURE);
      processed->getLuma(processed_frame, CV_32F);
      if (strlen(argv[PARAM_EXTRA]) > 1) {
        if (!extra->readOneFrame()) exit(EXIT_FAILURE);
        extra->getLuma(extra_frame, CV_32F);
      }

      if (with_chroma) {
        original->getChroma0(original_frame_chroma_u, CV_32F);
        processed->getChroma0(processed_frame_chroma_u, CV_32F);
        if (strlen(argv[PARAM_EXTRA]) > 1) {
          extra->getChroma0(extra_frame_chroma_u, CV_32F);
        }
        original->getChroma1(original_frame_chroma_v, CV_32F);
        processed->getChroma1(processed_frame_chroma_v, CV_32F);
        if (strlen(argv[PARAM_EXTRA]) > 1) {
          extra->getChroma1(extra_frame_chroma_v, CV_32F);
        }
      }
    }

		// Compute PSNR
		if (result_file[METRIC_PSNR] != NULL) {
            result[METRIC_PSNR] = psnr->compute(original_frame, processed_frame);
		}

    if (result_file[METRIC_PSNR_U] != NULL) {
      result[METRIC_PSNR_U] = psnr->compute(original_frame_chroma_u, processed_frame_chroma_u, u_width, u_height);
    }
    if (result_file[METRIC_PSNR_V] != NULL) {
      result[METRIC_PSNR_V] = psnr->compute(original_frame_chroma_v, processed_frame_chroma_v, v_width, v_height);
    }

		if (result_file[METRIC_ABS_ERR] != NULL) {
			result[METRIC_ABS_ERR] = psnr->compute_abs_error(original_frame, processed_frame);
		}
    if (result_file[METRIC_MEAN_ABS_ERR] != NULL) {
      result[METRIC_MEAN_ABS_ERR] = psnr->compute_abs_error(original_frame, processed_frame) / (height * width);
    }

    if (result_file[METRIC_ABS_ERR_WHITE] != NULL) {
      result[METRIC_ABS_ERR_WHITE] = psnr->compute_abs_error(original_frame, processed_frame, white_x, white_y, white_width, white_height);
    }

        if(result_file[METRIC_HIST] != NULL) {
            // New: compute histogram of differences!
            psnr->compute_with_hist(original_frame, processed_frame, histBuffer);
        }

        if(result_file_hist_psnr[0] != NULL) {
            // New: compute histogram of differences of (MSE orig - processed) - (MSE orig - unwatermarked)! (per block)
            // (extra = unwatermarked)
            psnr->compute_with_hist_sub(original_frame, processed_frame, extra_frame, histSubBuffers, amountOfPSNRBins, blockSizes, HISTS_SIZE);
        }

		// Compute SSIM and MS-SSIM
		if (result_file[METRIC_SSIM] != NULL && result_file[METRIC_MSSSIM] == NULL) {
			result[METRIC_SSIM] = ssim->compute(original_frame, processed_frame);
		}
    if (result_file[METRIC_SSIM_U] != NULL) {
      result[METRIC_SSIM_U] = ssim->compute(original_frame_chroma_u, processed_frame_chroma_u);
    }
    if (result_file[METRIC_SSIM_V] != NULL) {
      result[METRIC_SSIM_V] = ssim->compute(original_frame_chroma_v, processed_frame_chroma_v);
    }

		if (result_file[METRIC_MSSSIM] != NULL) {
			msssim->compute(original_frame, processed_frame);
			if (result_file[METRIC_SSIM] != NULL) {
				result[METRIC_SSIM] = msssim->getSSIM();
			}
			result[METRIC_MSSSIM] = msssim->getMSSSIM();
		}

        if(result_file_hist_ssim[0] != NULL) {
            // New: compute histogram of SSIM differences of (SSIM orig - processed) - (SSIM orig - unwatermarked)! (per block)
            // (extra_frame = unwatermarked)
            ssim->compute_with_hist_sub(original_frame, processed_frame, extra_frame, histSSIMSubBuffers, amountOfSSIMBins, blockSizes, HISTS_SIZE);
        }

		// Compute VIFp
		if (result_file[METRIC_VIFP] != NULL) {
			result[METRIC_VIFP] = vifp->compute(original_frame, processed_frame);
		}

    if (result_file[METRIC_VIFP_U] != NULL) {
      result[METRIC_VIFP_U] = vifp_u->compute(original_frame_chroma_u, processed_frame_chroma_u);
    }
    if (result_file[METRIC_VIFP_V] != NULL) {
      result[METRIC_VIFP_V] = vifp_u->compute(original_frame_chroma_v, processed_frame_chroma_v);
    }

		// Compute PSNR-HVS and PSNR-HVS-M
		if (result_file[METRIC_PSNRHVS] != NULL || result_file[METRIC_PSNRHVSM] != NULL) {
			phvs->compute(original_frame, processed_frame);
			if (result_file[METRIC_PSNRHVS] != NULL) {
				result[METRIC_PSNRHVS] = phvs->getPSNRHVS();
			}
			if (result_file[METRIC_PSNRHVSM] != NULL) {
				result[METRIC_PSNRHVSM] = phvs->getPSNRHVSM();
			}
		}

        if(result_file[METRIC_CORRELATION_LIN] != NULL) {
            // New: compute linear correlation
            result[METRIC_CORRELATION_LIN] = correlation->compute_correlation_linear_subtract(original_frame, processed_frame, extra_frame);
        }

        if(result_file[METRIC_CORRELATION_NORM] != NULL) {
            // New: compute normalized correlation
            result[METRIC_CORRELATION_NORM] = correlation->compute_correlation_normalized_subtract(original_frame, processed_frame, extra_frame);
        }

        if(result_file[METRIC_CORRELATION_COEF] != NULL) {
            // New: compute correlation coefficient
            result[METRIC_CORRELATION_COEF] = correlation->compute_correlation_coefficient_subtract(original_frame, processed_frame, extra_frame);
        }

        if(result_file[METRIC_CORRELATION_LIN_NO_SUB] != NULL) {
            // New: compute linear correlation (no sub)
            result[METRIC_CORRELATION_LIN_NO_SUB] = correlation->compute_correlation_linear(original_frame, processed_frame);
        }

        if(result_file[METRIC_CORRELATION_NORM_NO_SUB] != NULL) {
            // New: compute normalized correlation (no sub)
            result[METRIC_CORRELATION_NORM_NO_SUB] = correlation->compute_correlation_normalized(original_frame, processed_frame);
        }

        if(result_file[METRIC_CORRELATION_COEF_NO_SUB] != NULL) {
            // New: compute correlation coefficient (no sub)
            result[METRIC_CORRELATION_COEF_NO_SUB] = correlation->compute_correlation_coefficient(original_frame, processed_frame);
        }
        if (result_file[METRIC_CORRELATION_COEF_NO_SUB_WCHROMA] != NULL) {
          // New: compute correlation coefficient (no sub), with chroma
          float corr_luma = correlation->compute_correlation_coefficient(original_frame, processed_frame);
          float corr_chroma_u = correlation_chroma_u->compute_correlation_coefficient(original_frame_chroma_u, processed_frame_chroma_u);
          float corr_chroma_v = correlation_chroma_v->compute_correlation_coefficient(original_frame_chroma_v, processed_frame_chroma_v);
          result[METRIC_CORRELATION_COEF_NO_SUB_WCHROMA] = (corr_luma*4 + corr_chroma_u + corr_chroma_v) / (4 + 1 + 1);
        }
        if (result_file[METRIC_CORRELATION_COEF_NO_SUB_WHITE] != NULL) {
          // New: compute correlation coefficient (no sub) WHITE
          result[METRIC_CORRELATION_COEF_NO_SUB_WHITE] = correlation->compute_correlation_coefficient(original_frame, processed_frame, white_x, white_y, white_width, white_height);
        }

        if (result_file[METRIC_CORRELATION_COEF_NO_SUB_WCHROMA_WHITE] != NULL) {
          // New: compute correlation coefficient (no sub), witch chroma and WHITE
          float corr_luma = correlation->compute_correlation_coefficient(original_frame, processed_frame, white_x, white_y, white_width, white_height);
          float corr_chroma_u = correlation_chroma_u->compute_correlation_coefficient(original_frame_chroma_u, processed_frame_chroma_u, white_x, white_y, white_width, white_height);
          float corr_chroma_v = correlation_chroma_v->compute_correlation_coefficient(original_frame_chroma_v, processed_frame_chroma_v, white_x, white_y, white_width, white_height);
          result[METRIC_CORRELATION_COEF_NO_SUB_WCHROMA_WHITE] = (corr_luma * 4 + corr_chroma_u + corr_chroma_v) / (4 + 1 + 1);
        }

        if (result_file[METRIC_CORRELATION_COEF_NO_SUB_LOWPASS] != NULL) {
          // New: compute correlation coefficient (no sub) LOWPASS
          result[METRIC_CORRELATION_COEF_NO_SUB_LOWPASS] = correlation->compute_correlation_coefficient_lowpass(original_frame, processed_frame, white_height, white_width);
        }

        if (result_file[METRIC_CORRELATION_COEF_LOWPASS] != NULL) {
          // New: compute correlation coefficient LOWPASS
          result[METRIC_CORRELATION_COEF_LOWPASS] = correlation->compute_correlation_coefficient_subtract_lowpass(original_frame, processed_frame, extra_frame, white_height, white_width);
        }

        if (result_file[METRIC_CORRELATION_COEF_LOWPASS_BLOCKS] != NULL) {
          // New: compute correlation coefficient LOWPASS BLOCKS
          result[METRIC_CORRELATION_COEF_LOWPASS_BLOCKS] = correlation->compute_correlation_coefficient_subtract_lowpass_blocks(original_frame, processed_frame, extra_frame, white_height, white_width, white_x, white_y);
        }

        if (result_file[METRIC_CORRELATION_COEF_NO_SUB_LOWPASS_BLOCKS] != NULL) {
          // New: compute correlation coefficient no sub  LOWPASS BLOCKS
          result[METRIC_CORRELATION_COEF_NO_SUB_LOWPASS_BLOCKS] = correlation->compute_correlation_coefficient_lowpass_blocks(original_frame, processed_frame, white_height, white_width, white_x, white_y);
        }

        //float factor_threshold_std = 0.0; // Set to 0 if want to keep all elements
        float factor_threshold_std = 0.25;
        if (result_file[METRIC_CORRELATION_COEF_NO_SUB_BINARIZED] != NULL) {
          // New
          result[METRIC_CORRELATION_COEF_NO_SUB_BINARIZED] = correlation->compute_correlation_coefficient_binarized(original_frame, processed_frame, factor_threshold_std);
        }

        if (result_file[METRIC_CORRELATION_COEF_BINARIZED] != NULL) {
          // New
          result[METRIC_CORRELATION_COEF_BINARIZED] = correlation->compute_correlation_coefficient_subtract_binarized(original_frame, processed_frame, extra_frame, factor_threshold_std);
        }

        if (result_file[METRIC_EUCL_DIST_SQ] != NULL) {
          result[METRIC_EUCL_DIST_SQ] = correlation->compute_eucl_dist_sq_subtract(original_frame, processed_frame, extra_frame);
        }

        if (result_file[METRIC_EUCL_DIST_SQ_NO_SUB] != NULL) {
          result[METRIC_EUCL_DIST_SQ_NO_SUB] = correlation->compute_eucl_dist_sq(original_frame, processed_frame);
        }


        //printf("0\n");
        if (result_file[METRIC_YUV_DIFF] != NULL) {
          // New: write difference YUV
          int mse_or_psnr = 1; // psnr
          yuvDiff->calculate_and_map_differences(original_frame, processed_frame, extra_frame, y, u, v, mse_or_psnr);
          yuvDiff->write_yuv(result_file[METRIC_YUV_DIFF], y, u, v);
        }

        if (result_file[METRIC_YUV_DIFF_U] != NULL || result_file[METRIC_YUV_DIFF_V] != NULL) {
          // New: write difference YUV
          int mse_or_psnr = 1; // psnr
          cv::Mat original_frame_u(u_height, u_width, CV_32F), processed_frame_u(u_height, u_width, CV_32F), extra_frame_u(u_height, u_width, CV_32F);
          cv::Mat y_small(u_height, u_width, CV_32F);
          cv::Mat u_small(u_height/2, u_width/2, CV_32F), v_small(v_height/2, v_width/2, CV_32F);

          if (result_file[METRIC_YUV_DIFF_U] != NULL) {
            original->getChroma0(original_frame_u, CV_32F);
            processed->getChroma0(processed_frame_u, CV_32F);
            extra->getChroma0(extra_frame_u, CV_32F);

            yuvDiff_uv->calculate_and_map_differences(original_frame_u, processed_frame_u, extra_frame_u, y_small, u_small, v_small, mse_or_psnr);
            yuvDiff_uv->write_yuv(result_file[METRIC_YUV_DIFF_U], y_small, u_small, v_small);
          }
          if (result_file[METRIC_YUV_DIFF_V] != NULL) {
            original->getChroma1(original_frame_u, CV_32F);
            processed->getChroma1(processed_frame_u, CV_32F);
            extra->getChroma1(extra_frame_u, CV_32F);

            yuvDiff_uv->calculate_and_map_differences(original_frame_u, processed_frame_u, extra_frame_u, y_small, u_small, v_small, mse_or_psnr);
            yuvDiff_uv->write_yuv(result_file[METRIC_YUV_DIFF_V], y_small, u_small, v_small);
          }
        }


        if (result_file[METRIC_YUV_DIFF_MSE] != NULL) {
          // New: write difference YUV MSE
          int mse_or_psnr = 0; // mse
          yuvDiff->calculate_and_map_differences(original_frame, processed_frame, extra_frame, y, u, v, mse_or_psnr);
          yuvDiff->write_yuv(result_file[METRIC_YUV_DIFF_MSE], y, u, v);
        }
        if (result_file[METRIC_YUV_DIFF_CORR_COEF] != NULL) {
          // New: write difference YUV MSE
          int mse_or_psnr = 2; // corr_coef
          yuvDiff->calculate_and_map_differences(original_frame, processed_frame, extra_frame, y, u, v, mse_or_psnr);
          yuvDiff->write_yuv(result_file[METRIC_YUV_DIFF_CORR_COEF], y, u, v);
        }

        if (result_file[METRIC_YUV_SUBTRACT_DIFF] != NULL) {
          // New: write the subtract difference YUV
          
          // Get 8bit y component as 8 bit unsigned instead of 32bit floats
          cv::Mat original_frame_8b(height, width, CV_8U), processed_frame_8b(height, width, CV_8U);
          cv::Mat extra_frame_8b(height, width, CV_8U);
          original->getLuma(original_frame_8b, CV_8U);
          processed->getLuma(processed_frame_8b, CV_8U);
          extra->getLuma(extra_frame_8b, CV_8U);

          // Also grab u and v components
          cv::Mat original_frame_u(u_height, u_width, CV_8U), processed_frame_u(u_height, u_width, CV_8U), extra_frame_u(u_height, u_width, CV_8U);
          cv::Mat original_frame_v(v_height, v_width, CV_8U), processed_frame_v(v_height, v_width, CV_8U), extra_frame_v(v_height, v_width, CV_8U);

          original->getChroma0(original_frame_u, CV_8U);
          processed->getChroma0(processed_frame_u, CV_8U);
          extra->getChroma0(extra_frame_u, CV_8U);
          original->getChroma1(original_frame_v, CV_8U);
          processed->getChroma1(processed_frame_v, CV_8U);
          extra->getChroma1(extra_frame_v, CV_8U);
          
          // Calculate and subtract differences
          yuvDiff->calculate_and_subtract_differences(original_frame_8b, processed_frame_8b, extra_frame_8b, y);
          yuvDiff->calculate_and_subtract_differences(original_frame_u, processed_frame_u, extra_frame_u, u);
          yuvDiff->calculate_and_subtract_differences(original_frame_v, processed_frame_v, extra_frame_v, v);

          // Write resulting yuv file
          yuvDiff->write_yuv(result_file[METRIC_YUV_SUBTRACT_DIFF], y, u, v);
        }

        if (result_file[METRIC_YUV_CHANGE] != NULL || result_file[METRIC_YUV_CHANGE_Y] != NULL ||  result_file[METRIC_YUV_CHANGE_U] != NULL || result_file[METRIC_YUV_CHANGE_V] != NULL || 
          result_file[METRIC_YUV_CHANGE_NS] != NULL || result_file[METRIC_YUV_CHANGE_NS_Y] != NULL || result_file[METRIC_YUV_CHANGE_NS_U] != NULL || result_file[METRIC_YUV_CHANGE_NS_V] != NULL) {

          // Grab 8 bit y, u and v components
          cv::Mat original_frame_y_8b(height, width, CV_8U);
          original->getLuma(original_frame_y_8b, CV_8U);
          cv::Mat original_frame_u(u_height, u_width, CV_8U);
          original->getChroma0(original_frame_u, CV_8U);
          cv::Mat original_frame_v(v_height, v_width, CV_8U);
          original->getChroma1(original_frame_v, CV_8U);

          // Different noise for every frame
          //int seed = white_x + frame;
          // Same noise for every frame
          int seed = white_x ;
          float mean = 0.0;
          float stdev = float(white_y);

          if (result_file[METRIC_YUV_CHANGE] != NULL) {
            cv::Mat new_frame_y(height, width, CV_8U);
            yuvDiff->randomly_change(original_frame_y_8b, seed, mean, stdev, new_frame_y);
            cv::Mat new_frame_u(u_height, u_width, CV_8U);
            yuvDiff->randomly_change(original_frame_u, seed+42, mean, stdev, new_frame_u);
            cv::Mat new_frame_v(v_height, v_width, CV_8U);
            yuvDiff->randomly_change(original_frame_v, seed-42, mean, stdev, new_frame_v);

            // Write resulting yuv file
            yuvDiff->write_yuv(result_file[METRIC_YUV_CHANGE], new_frame_y, new_frame_u, new_frame_v);
          }

          if (result_file[METRIC_YUV_CHANGE_Y] != NULL) {
            cv::Mat new_frame_y(height, width, CV_8U);
            yuvDiff->randomly_change(original_frame_y_8b, seed, mean, stdev, new_frame_y);

            // Write resulting yuv file
            yuvDiff->write_yuv(result_file[METRIC_YUV_CHANGE_Y], new_frame_y, original_frame_u, original_frame_v);
          }

          if (result_file[METRIC_YUV_CHANGE_U] != NULL) {
            cv::Mat new_frame_u(u_height, u_width, CV_8U);
            yuvDiff->randomly_change(original_frame_u, seed, mean, stdev, new_frame_u);

            // Write resulting yuv file
            yuvDiff->write_yuv(result_file[METRIC_YUV_CHANGE_U], original_frame_y_8b, new_frame_u, original_frame_v);
          }

          if (result_file[METRIC_YUV_CHANGE_V] != NULL) {
            cv::Mat new_frame_v(v_height, v_width, CV_8U);
            yuvDiff->randomly_change(original_frame_v, seed, mean, stdev, new_frame_v);

            // Write resulting yuv file
            yuvDiff->write_yuv(result_file[METRIC_YUV_CHANGE_V], original_frame_y_8b, original_frame_u, new_frame_v);
          }

          if (result_file[METRIC_YUV_CHANGE_NS] != NULL) {
            cv::Mat new_frame_y(height, width, CV_8U);
            yuvDiff->randomly_change_non_stationary(original_frame_y_8b, seed, mean, stdev, new_frame_y);
            cv::Mat new_frame_u(u_height, u_width, CV_8U);
            yuvDiff->randomly_change_non_stationary(original_frame_u, seed+42, mean, stdev, new_frame_u);
            cv::Mat new_frame_v(v_height, v_width, CV_8U);
            yuvDiff->randomly_change_non_stationary(original_frame_v, seed-42, mean, stdev, new_frame_v);

            // Write resulting yuv file
            yuvDiff->write_yuv(result_file[METRIC_YUV_CHANGE_NS], new_frame_y, new_frame_u, new_frame_v);
          }

          if (result_file[METRIC_YUV_CHANGE_NS_Y] != NULL) {
            cv::Mat new_frame_y(height, width, CV_8U);
            yuvDiff->randomly_change_non_stationary(original_frame_y_8b, seed, mean, stdev, new_frame_y);

            // Write resulting yuv file
            yuvDiff->write_yuv(result_file[METRIC_YUV_CHANGE_NS_Y], new_frame_y, original_frame_u, original_frame_v);
          }

          if (result_file[METRIC_YUV_CHANGE_NS_U] != NULL) {
            cv::Mat new_frame_u(u_height, u_width, CV_8U);
            yuvDiff->randomly_change_non_stationary(original_frame_u, seed, mean, stdev, new_frame_u);

            // Write resulting yuv file
            yuvDiff->write_yuv(result_file[METRIC_YUV_CHANGE_NS_U], original_frame_y_8b, new_frame_u, original_frame_v);
          }

          if (result_file[METRIC_YUV_CHANGE_NS_V] != NULL) {
            cv::Mat new_frame_v(v_height, v_width, CV_8U);
            yuvDiff->randomly_change_non_stationary(original_frame_v, seed, mean, stdev, new_frame_v);

            // Write resulting yuv file
            yuvDiff->write_yuv(result_file[METRIC_YUV_CHANGE_NS_V], original_frame_y_8b, original_frame_u, new_frame_v);
          }
        }

        if (result_file[METRIC_YUV_FIXED_Y] != NULL) {
          cv::Mat original_frame_y_8b(height, width, CV_8U);
          original->getLuma(original_frame_y_8b, CV_8U);
          cv::Mat original_frame_u(u_height, u_width, CV_8U);
          original->getChroma0(original_frame_u, CV_8U);
          cv::Mat original_frame_v(v_height, v_width, CV_8U);
          original->getChroma1(original_frame_v, CV_8U);

          int fixed_value = white_x;

          //cv::Mat new_frame_y(height, width, CV_8U);
          //yuvDiff->change_fixed(original_frame_y_8b, fixed_value, new_frame_y);

          original_frame_y_8b.setTo(cv::Scalar(fixed_value));
          //printf("Change to %d \n", fixed_value);

          // Write resulting yuv file
          yuvDiff->write_yuv(result_file[METRIC_YUV_FIXED_Y], original_frame_y_8b, original_frame_u, original_frame_v);
        }

        if (result_file[METRIC_YUV_ONLY_Y] != NULL || result_file[METRIC_YUV_ONLY_U] != NULL || result_file[METRIC_YUV_ONLY_V] != NULL || result_file[METRIC_YUV_U2Y] != NULL || result_file[METRIC_YUV_V2Y] != NULL) {
          cv::Mat original_frame_y_8b(height, width, CV_8U);
          original->getLuma(original_frame_y_8b, CV_8U);
          cv::Mat original_frame_u(u_height, u_width, CV_8U);
          original->getChroma0(original_frame_u, CV_8U);
          cv::Mat original_frame_v(v_height, v_width, CV_8U);
          original->getChroma1(original_frame_v, CV_8U);

          int fixed_value_other_1 = white_x;
          int fixed_value_other_2 = white_y;

          if (result_file[METRIC_YUV_ONLY_Y] != NULL) {
            cv::Mat new_frame_u(u_height, u_width, CV_8U);
            cv::Mat new_frame_v(v_height, v_width, CV_8U);

            new_frame_u.setTo(cv::Scalar(fixed_value_other_1));
            new_frame_v.setTo(cv::Scalar(fixed_value_other_2));

            yuvDiff->write_yuv(result_file[METRIC_YUV_ONLY_Y], original_frame_y_8b, new_frame_u, new_frame_v);
          }

          if (result_file[METRIC_YUV_ONLY_U] != NULL) {
            cv::Mat new_frame_y(height, width, CV_8U);
            cv::Mat new_frame_v(v_height, v_width, CV_8U);

            new_frame_y.setTo(cv::Scalar(fixed_value_other_1));
            new_frame_v.setTo(cv::Scalar(fixed_value_other_2));

            yuvDiff->write_yuv(result_file[METRIC_YUV_ONLY_U], new_frame_y, original_frame_u, new_frame_v);
          }

          if (result_file[METRIC_YUV_ONLY_V] != NULL) {
            cv::Mat new_frame_y(height, width, CV_8U);
            cv::Mat new_frame_u(u_height, u_width, CV_8U);
            new_frame_y.setTo(cv::Scalar(fixed_value_other_1));
            new_frame_u.setTo(cv::Scalar(fixed_value_other_2));

            yuvDiff->write_yuv(result_file[METRIC_YUV_ONLY_V], new_frame_y, new_frame_u, original_frame_v);
          }

          if (result_file[METRIC_YUV_U2Y] != NULL || result_file[METRIC_YUV_V2Y] != NULL) {
            //cv::Mat new_frame_y(u_height, u_height, CV_8U);
            cv::Mat new_frame_u(u_height/2, u_width/2, CV_8U);
            cv::Mat new_frame_v(v_height/2, v_width/2, CV_8U);

            new_frame_u.setTo(cv::Scalar(fixed_value_other_1));
            new_frame_v.setTo(cv::Scalar(fixed_value_other_2));

            if (result_file[METRIC_YUV_U2Y] != NULL) {
              //cv::resize(original_frame_u, new_frame_y, new_frame_y.size());
              yuvDiff->write_yuv(result_file[METRIC_YUV_U2Y], original_frame_u, new_frame_u, new_frame_v);
            }
            if (result_file[METRIC_YUV_V2Y] != NULL) {
              //cv::resize(original_frame_v, new_frame_y, new_frame_y.size());
              yuvDiff->write_yuv(result_file[METRIC_YUV_V2Y], original_frame_v, new_frame_u, new_frame_v);
            }
          }

        }

        if (result_file[METRIC_YUV_AVG]) { // if(is_create_average)
           // reset original_frame
          original_frame = cv::Mat(height, width, CV_16U);
          original_frame_chroma_u = cv::Mat(u_height, u_width, CV_32F);
          original_frame_chroma_v = cv::Mat(v_height, v_width, CV_32F);
          for (int i = 0; i < nr_of_videos_to_average; i++) {
            if (!videosToAverage.at(i)->readOneFrame()) exit(EXIT_FAILURE);
            videosToAverage.at(i)->getLuma(processed_frame, CV_16U);
            yuvDiff->add_to_average_video(original_frame, processed_frame);

            videosToAverage.at(i)->getChroma0(processed_frame_chroma_u, CV_32F);
            yuvDiff->add_to_average_video(original_frame_chroma_u, processed_frame_chroma_u);

            videosToAverage.at(i)->getChroma1(processed_frame_chroma_v, CV_32F);
            yuvDiff->add_to_average_video(original_frame_chroma_v, processed_frame_chroma_v);

            //std::cout << "Average added " << frame << " \n";
          }

          yuvDiff->finish_average_video(original_frame, nr_of_videos_to_average);
          yuvDiff->finish_average_video(original_frame_chroma_u, nr_of_videos_to_average);
          yuvDiff->finish_average_video(original_frame_chroma_v, nr_of_videos_to_average);

          original_frame.convertTo(original_frame, CV_8U);
          original_frame_chroma_u.convertTo(original_frame_chroma_u, CV_8U);
          original_frame_chroma_v.convertTo(original_frame_chroma_v, CV_8U);

          // For now: get u and v from first video
          yuvDiff->write_yuv(result_file[METRIC_YUV_AVG], original_frame, original_frame_chroma_u, original_frame_chroma_v);
        }
		// Print quality index to file
        for(int m = 0; m<METRIC_SIZE_1_VALUE; m++) {
			    if (result_file[m] != NULL) {
            if (isinf(result[m])) { // Hannes: cap (PSNR) max value to 100. Otherwise if 1 frame is inf, then whole average is inf.
              float cap_value = 100;
              result_avg[m] += cap_value;
            }
            else {
              result_non_inf[m] = true;
              result_avg[m] += result[m];
            }
				    //fprintf(result_file[m], "%d,%.6f\n", frame, static_cast<double>(result[m]));
            fprintf(result_file[m], "%.6f\n", static_cast<double>(result[m]));
			    }
		    }

        // Extra: print quality histogram to file
        if(result_file[METRIC_HIST] != NULL) {
            //fprintf(result_file[METRIC_HIST], "%d", frame);
            for(int i = 0; i < 256; i++) {
                fprintf(result_file[METRIC_HIST], ",%d", histBuffer[i]);
                // Keep max
                if(histBuffer[i] > maxHistBuffer[i]) maxHistBuffer[i] = histBuffer[i];
            }
            fprintf(result_file[METRIC_HIST], "\n");
        }

        // Extra: print quality histograms sub to files
        for(int i = 0; i < HISTS_SIZE; i++) {
            //PSNR
            if(result_file_hist_psnr[i] != NULL) {
                //fprintf(result_file_hist_psnr[i], "%d", frame);
                for(int j = 0; j < amountOfPSNRBins * 2; j++) {
                    if(histSubBuffers[i][j] == 0) {
                        fprintf(result_file_hist_psnr[i], ",");
                    } else {
                        fprintf(result_file_hist_psnr[i], ",%d", histSubBuffers[i][j]);
                    }
                    // Keep max
                    if(histSubBuffers[i][j] > maxHistSubBuffers[i][j]) maxHistSubBuffers[i][j] = histSubBuffers[i][j];
                }
                fprintf(result_file_hist_psnr[i], "\n");
            }

            // SSIM
            if(result_file_hist_ssim[i] != NULL) {
                //fprintf(result_file_hist_ssim[i], "%d", frame);
                for(int j = 0; j < amountOfSSIMBins * 2; j++) {
                    if(histSSIMSubBuffers[i][j] == 0) {
                        fprintf(result_file_hist_ssim[i], ",");
                    } else {
                        fprintf(result_file_hist_ssim[i], ",%d", histSSIMSubBuffers[i][j]);
                    }
                    // Keep max
                    if(histSSIMSubBuffers[i][j] > maxHistSSIMSubBuffers[i][j]) maxHistSSIMSubBuffers[i][j] = histSSIMSubBuffers[i][j];
                }
                fprintf(result_file_hist_ssim[i], "\n");
            }
        }
	}

    //std::cout << "Done with loop!" << "\n";

	// Print average quality index to file
    for(int m = 0; m < METRIC_SIZE_1_VALUE; m++) {
		if (result_file[m] != NULL) {
      if (result_non_inf[m]) {
        result_avg[m] /= static_cast<float>(nbframes);
      }
      else {
        //printf("TO INFINITY AND BEYOND");
        result_avg[m] = INFINITY;
      }
			fprintf(result_file[m], "average,%.6f", static_cast<double>(result_avg[m]));
      // Also print to stdout
      printf("%.6f\n", static_cast<double>(result_avg[m]));
			fclose(result_file[m]);
		}
	}


    // Extra: print max of quality histograms
    if(result_file[METRIC_HIST] != NULL) {
        fprintf(result_file[METRIC_HIST], "max");
        for(int i = 0; i < 256; i++) {
            fprintf(result_file[METRIC_HIST], ",%d", maxHistBuffer[i]);
        }
        fprintf(result_file[METRIC_HIST], "\n");
        fclose(result_file[METRIC_HIST]);
    }

    // Extra: print max of quality histograms sub
    for(int i = 0; i < HISTS_SIZE; i++) {
        //PSNR
        if(result_file_hist_psnr[i] != NULL) {
            fprintf(result_file_hist_psnr[i], "max");
            for(int j = 0; j < amountOfPSNRBins * 2; j++) {
                if(maxHistSubBuffers[i][j] == 0) {
                    fprintf(result_file_hist_psnr[i], ",");
                } else {
                    fprintf(result_file_hist_psnr[i], ",%d", maxHistSubBuffers[i][j]);
                }
            }
            fprintf(result_file_hist_psnr[i], "\n");
            fclose(result_file_hist_psnr[i]);
        }

        // SSIM
        if(result_file_hist_ssim[i] != NULL) {
            fprintf(result_file_hist_ssim[i], "max");
            for(int j = 0; j < amountOfSSIMBins * 2; j++) {
                if(maxHistSSIMSubBuffers[i][j] == 0) {
                    fprintf(result_file_hist_ssim[i], ",");
                } else {
                    fprintf(result_file_hist_ssim[i], ",%d", maxHistSSIMSubBuffers[i][j]);
                }
            }
            fprintf(result_file_hist_ssim[i], "\n");
            fclose(result_file_hist_ssim[i]);
        }
    }

    for (int m = METRIC_SIZE_YUV_BEGIN + 1; m < METRIC_SIZE_1_VALUE; m++) {
      if (result_file[m] != NULL) {
        fclose(result_file[m]);
      }
    }

    // Delete created pointers
    for(int i = 0; i < HISTS_SIZE; i++) {
        delete[] histSubBuffers[i];
        delete[] maxHistSubBuffers[i];
        delete[] histSSIMSubBuffers[i];
        delete[] maxHistSSIMSubBuffers[i];
    }

    if (is_create_average) {
      for (int i = nr_video_to_average_begin; i <= nr_video_to_average_end; i++) {
        //delete videosToAverage.at(i);
      }
    }
    else {
      delete original;
      delete processed;
    }

	delete psnr;
	delete ssim;
	delete msssim;
	delete vifp;
	delete phvs;

  delete correlation;
  delete yuvDiff;


	duration = static_cast<double>(cv::getTickCount())-duration;
	duration /= cv::getTickFrequency();
	//printf("Time: %0.3fs\n", duration);

	return EXIT_SUCCESS;
}
