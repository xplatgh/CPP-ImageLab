#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>

#include "operations.h"

IplImage* gray_img(IplImage *src=0);
IplImage* GT_img(int GT_type=1,int save=0);
//[GT_type:type]:[1:linearise],[2:log],[3:invert],[4:binarize];
