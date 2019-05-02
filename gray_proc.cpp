#include "gray_proc.h"

IplImage* gray_img(IplImage *src)
{
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j;
	
    dst_cvsize.width = (int)(src->width);
    dst_cvsize.height = (int)(src->height);
	
    dst=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
	
	for(i=0;i<dst_cvsize.height;i++)
	{
		for(j=0;j<dst_cvsize.width;j++)
		{
			s=cvGet2D(src,i,j);
			s.val[0]=(s.val[0]+s.val[1]+s.val[2])/3;
			s.val[1]=s.val[0];
			s.val[2]=s.val[0];
			cvSet2D(dst,i,j,s);
		}
	}

	return dst;
}

IplImage* GT_img(int GT_type,int save)
{
    IplImage *src = 0;
    IplImage *mid = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k;
	char filename[50];
	src=get_img(filename);
	char *saveName[] = {"GT1.jpg", "GT2.jpg", "GT3.jpg", "GT4.jpg"};
	double a,b,c,d,rgb;
	
    dst_cvsize.width = (int)(src->width);
    dst_cvsize.height = (int)(src->height);
	
    dst=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
    mid=cvCreateImage(dst_cvsize, src->depth, src->nChannels);

	mid=gray_img(src);

	switch(GT_type)
	{
	case 1:printf("Please input a,b,c,d([0.0,1.0]) separated by \",\":\n");scanf("%lf,%lf,%lf,%lf",&a,&b,&c,&d);break;
	case 2:printf("Please input c:\n");scanf("%lf",&a);break;
	case 4:printf("Please input level([0.0,1.0]):\n");scanf("%lf",&a);break;
	default:break;
	}

	for(i=0;i<dst_cvsize.height;i++)
	{
		for(j=0;j<dst_cvsize.width;j++)
		{
			s=cvGet2D(mid,i,j);
			rgb=s.val[0]/255.0;
			switch(GT_type)
			{
			case 1:if(rgb>b) rgb=d;else if (rgb<a) rgb=c;else rgb=(d-c)*(rgb-a)/(b-a)+c;break;
			case 2:rgb=a*log10(rgb+1);if(rgb>1.0) rgb=1.0;break;
			case 3:rgb=1.0-rgb;break;
			case 4:rgb=(rgb>a?1.0:0.0);break;
			default:break;
			}
			for(k=0;k<3;k++) s.val[k]=255.0*rgb;
			cvSet2D(dst,i,j,s);
		}
	}
	
	if(save)
	{
		cvSaveImage(saveName[GT_type-1],dst);
		printf("GT%d_img has been saved as \"%s\".\n",GT_type,saveName[GT_type-1]);
	}

    return dst;
}
