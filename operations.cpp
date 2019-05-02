#include "operations.h"

IplImage* get_img(char *filename)
{
    IplImage *src = 0;
	printf("Please input the filename of the source image:\n");
	while(1)
	{
		scanf("%s",filename);
		src=cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
		if(src) return src;
		printf("File Not Found! Please try again:\n");
	}
}

IplImage* add_img(int save)
{
    IplImage *src1 = 0;
    IplImage *src2 = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s1,s2;
	int i,j;
	char filename1[50],filename2[50];
	src1=get_img(filename1);
	src2=get_img(filename2);
	
    dst_cvsize.width = (int)(src1->width);
    dst_cvsize.height = (int)(src1->height);
	
    dst=cvCreateImage(dst_cvsize, src1->depth, src1->nChannels);
	cvResize(src2,dst);
	
	for(i=0;i<dst_cvsize.height;i++)
	{
		for(j=0;j<dst_cvsize.width;j++)
		{
			s1=cvGet2D(src1,i,j);
			s2=cvGet2D(dst,i,j);
			s2.val[0]=(s1.val[0]+s2.val[0])/2;
			s2.val[1]=(s1.val[1]+s2.val[1])/2;
			s2.val[2]=(s1.val[2]+s2.val[2])/2;
			cvSet2D(dst,i,j,s2);
		}
	}
	
	if(save) 
	{
		cvSaveImage("add.jpg",dst);
		printf("Add_image has been saved as \"%s\".\n","add.jpg");
	}

	return dst;
}

IplImage* sub_img(int save)
{
    IplImage *src1 = 0;
    IplImage *src2 = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s1,s2;
	int i,j,k;
	char filename1[50],filename2[50];
	src1=get_img(filename1);
	src2=get_img(filename2);
	
    dst_cvsize.width = (int)(src1->width);
    dst_cvsize.height = (int)(src1->height);
	
    dst=cvCreateImage(dst_cvsize, src1->depth, src1->nChannels);
	cvResize(src2,dst);
	
	for(i=0;i<dst_cvsize.height;i++)
	{
		for(j=0;j<dst_cvsize.width;j++)
		{
			s1=cvGet2D(src1,i,j);
			s2=cvGet2D(dst,i,j);
			s2.val[0]=s1.val[0]*2-s2.val[0];
			s2.val[1]=s1.val[1]*2-s2.val[1];
			s2.val[2]=s1.val[2]*2-s2.val[2];
			for(k=0;k<3;k++)
				if(s2.val[k]<0) s2.val[k]=0;
			cvSet2D(dst,i,j,s2);
		}
	}
	
	if(save) 
	{
		cvSaveImage("sub.jpg",dst);
		printf("Sub_image has been saved as \"%s\".\n","sub.jpg");
	}

    return dst;
}

IplImage* mul_img(int save)
{
    IplImage *src1 = 0;
    IplImage *src2 = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s1,s2;
	int i,j;
	char filename1[50],filename2[50];
	src1=get_img(filename1);
	src2=get_img(filename2);
	
    dst_cvsize.width = (int)(src1->width);
    dst_cvsize.height = (int)(src1->height);
	
    dst=cvCreateImage(dst_cvsize, src1->depth, src1->nChannels);
	cvResize(src2,dst);
	
	for(i=0;i<dst_cvsize.height;i++)
	{
		for(j=0;j<dst_cvsize.width;j++)
		{
			s1=cvGet2D(src1,i,j);
			s2=cvGet2D(dst,i,j);
			s2.val[0]=sqrt(s1.val[0])*sqrt(s2.val[0]);
			s2.val[1]=sqrt(s1.val[1])*sqrt(s2.val[1]);
			s2.val[2]=sqrt(s1.val[2])*sqrt(s2.val[2]);
			cvSet2D(dst,i,j,s2);
		}
	}
	
	if(save) 
	{
		cvSaveImage("mul.jpg",dst);
		printf("Mul_image has been saved as \"%s\".\n","mul.jpg");
	}

    return dst;
}

IplImage* cut_img(int save)
{
    IplImage *src = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j;
	char filename[50];
	src=get_img(filename);
	double x1=0.0,x2=1.0,y1=0.0,y2=1.0;
	
	printf("Please input the displayed proportion([0.0,1.0]) separated by \",\":\n");
	scanf("%lf,%lf,%lf,%lf",&x1,&x2,&y1,&y2);

    dst_cvsize.width = (int)(src->width);
    dst_cvsize.height = (int)(src->height);
	
    dst=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
	
	for(i=0;i<dst_cvsize.height;i++)
	{
		for(j=0;j<dst_cvsize.width;j++)
		{
			s=cvGet2D(src,i,j);
			if(i<y1*dst_cvsize.height || i>y2*dst_cvsize.height || j<x1*dst_cvsize.width || j>x2*dst_cvsize.width)
			{
				s.val[0]=0.0;
				s.val[1]=0.0;
				s.val[2]=0.0;
			}
			cvSet2D(dst,i,j,s);
		}
	}
	
	if(save)
	{
		cvSaveImage("part.jpg",dst);
		printf("Part_image has been saved as \"%s\".\n","part.jpg");
	}

    return dst;
}

IplImage* rot_img(int save)
{
    IplImage *src = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k,mi,mj;
	char filename[50];
	src=get_img(filename);
	double theta=0.0;
	
	printf("Please input the angle of clockwise rotation:\n");
	scanf("%lf",&theta);

    dst_cvsize.width = (int)(src->width);
    dst_cvsize.height = (int)(src->height);
	
    dst=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
	
	for(i=0;i<dst_cvsize.height;i++)
		for(j=0;j<dst_cvsize.width;j++)
		{
			for(k=0;k<3;k++)
				s.val[k]=0.0;
			cvSet2D(dst,i,j,s);
		}

	for(i=0;i<dst_cvsize.height;i++)
	{
		for(j=0;j<dst_cvsize.width;j++)
		{
			mi=int(dst_cvsize.height*0.5+(i-dst_cvsize.height*0.5)*cos(theta*PI/180)-(j-dst_cvsize.width*0.5)*sin(theta*PI/180)+0.5);
			mj=int(dst_cvsize.width*0.5+(j-dst_cvsize.width*0.5)*cos(theta*PI/180)+(i-dst_cvsize.height*0.5)*sin(theta*PI/180)+0.5);
			if(mi>=0 && mi<dst_cvsize.height && mj>=0 && mj<dst_cvsize.width)
				cvSet2D(dst,i,j,cvGet2D(src,mi,mj));
		}
	}
	
	if(save)
	{
		cvSaveImage("rot.jpg",dst);
		printf("Rot_image has been saved as \"%s\".\n","rot.jpg");
	}

    return dst;
}
