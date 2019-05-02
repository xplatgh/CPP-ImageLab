#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>

#include "filters.h"

#define CANNY_T1 0.7
#define CANNY_T2 0.3
#define HOUGH_STEP1 500
#define HOUGH_STEP2 180
#define HOUGH_T 0.8
#define HOUGH_C 3
#define HOUGH_A 1
#define HOUGH_L 10000

IplImage* edge_sobel(int save=0, int canny=0);
//canny:used for edge_canny();
IplImage* edge_roberts(int save=0);
IplImage* edge_prewitt(int save=0);
IplImage* edge_log(int save=0);
IplImage* gaussian_blur(int save=0);
IplImage* edge_canny(int save=0);
IplImage* edge_hough(int save=0, int step1=HOUGH_STEP1, int step2=HOUGH_STEP2);
//step1:the number of segments of length
//step2:the number of angular slices
