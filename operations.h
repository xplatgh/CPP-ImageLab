#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>

#define PI 3.1415926

IplImage* get_img(char *filename);
IplImage* add_img(int save=0);
IplImage* sub_img(int save=0);
IplImage* mul_img(int save=0);
IplImage* cut_img(int save=0);
IplImage* rot_img(int save=0);
