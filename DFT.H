#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>

#include "gray_proc.h"

#define DFT_X 256
#define DFT_Y 256

IplImage* DFT_img(int save=0);
IplImage* IDFT_img(int save=0,char *mag_file="dftma.txt",char *pha_file="dftph.txt");
