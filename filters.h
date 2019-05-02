#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>

#include "DCT.H"

#define D0 25.0
#define D1 30.0
#define ORDER 1
#define RH 1.2
#define RL 0.2
#define PRE_T 0.4
#define PRE_MODE 0

IplImage* add_noise(int type=0, double para1=0.0, double para2=0.1, int save=0);
//type=0:CV_RAND_UNI,para1=lower bound,para2=upper bound;
//type=1:CV_RAND_NORMAL,para1=mean,para2=variance;
IplImage* ave_filter(int save=0);
IplImage* med_filter(int save=0);
IplImage* blpf(int save=0);
IplImage* elpf(int save=0);
IplImage* tlpf(int save=0);
IplImage* sobel_filter(int save=0);
IplImage* prewitt_filter(int save=0);
IplImage* lap_filter(int save=0);
IplImage* hmmph_filter(int save=0);
