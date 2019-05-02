#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>

#include "DFT.H"

#define DCT_N 256
#define DCT_B 10.0
#define DCT_C 0

IplImage* DCT_img(int save=0,int comp=DCT_C);
//comp:compare with DCT_B;
IplImage* IDCT_img(int save=0,char *mag_file="dct.txt");
