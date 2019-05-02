#include "edge_detectors.h"

IplImage* edge_sobel(int save, int canny)
{
    IplImage *src = 0;
    IplImage *mid = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k,m,n;
	char filename[50];
	double sobel_x[3][3]={{-1.0,0.0,1.0},{-2.0,0.0,2.0},{-1.0,0.0,1.0}};
	double sobel_y[3][3]={{-1.0,-2.0,-1.0},{0.0,0.0,0.0},{1.0,2.0,1.0}};
	double rgb,rgb_x,rgb_y;
	FILE *grad;
	
	src=get_img(filename);
	dst_cvsize.width = (int)(src->width);
	dst_cvsize.height = (int)(src->height);
	dst=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
	mid=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
	mid=gray_img(src);

	if(canny) grad=fopen("sobel_grad.txt","w");
	for(i=0;i<dst_cvsize.height;i++)
	{
		for(j=0;j<dst_cvsize.width;j++)
		{
			if(i>0 && i<dst_cvsize.height-1 && j>0 && j<dst_cvsize.width-1)
			{
				rgb=0.0;rgb_x=0.0;rgb_y=0.0;
				for(m=0;m<3;m++) for(n=0;n<3;n++) rgb_x+=cvGet2D(mid,i+m-1,j+n-1).val[0]*sobel_x[m][n];
				for(m=0;m<3;m++) for(n=0;n<3;n++) rgb_y+=cvGet2D(mid,i+m-1,j+n-1).val[0]*sobel_y[m][n];
				rgb=sqrt(rgb_x*rgb_x+rgb_y*rgb_y);
				for(k=0;k<3;k++) s.val[k]=rgb;
				if(canny) 
				{
					if(rgb_x<0 && rgb_y>=0) fprintf(grad,"%lf ",atan(rgb_y/rgb_x)+PI);
					else if(rgb_x<0 && rgb_y<0) fprintf(grad,"%lf ",atan(rgb_y/rgb_x)-PI);
					else if(rgb_x==0 && rgb_y>=0) fprintf(grad,"%lf ",PI/2);
					else if(rgb_x==0 && rgb_y<0) fprintf(grad,"%lf ",-PI/2);
					else fprintf(grad,"%lf ",atan(rgb_y/rgb_x));
				}
			}
			else
				for(k=0;k<3;k++) s.val[k]=0.0;
			cvSet2D(dst,i,j,s);
		}
	}
	if(canny) fclose(grad);

	if(save)
	{
		cvSaveImage("edge_sobel.jpg",dst);
		printf("%s has been saved as \"%s\".\n","edge_sobel_img","edge_sobel.jpg");
	}

	return dst;
}

IplImage* edge_roberts(int save)    
{
    IplImage *src = 0;
    IplImage *mid = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k;
	char filename[50];
	double rgb;
	
	src=get_img(filename);
	dst_cvsize.width = (int)(src->width);
	dst_cvsize.height = (int)(src->height);
	dst=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
	mid=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
	mid=gray_img(src);

	for(i=0;i<dst_cvsize.height;i++)
	{
		for(j=0;j<dst_cvsize.width;j++)
		{
			if(i<dst_cvsize.height-1 && j<dst_cvsize.width-1)
			{
				rgb=fabs(cvGet2D(mid,i,j).val[0]-cvGet2D(mid,i+1,j+1).val[0])+fabs(cvGet2D(mid,i+1,j).val[0]-cvGet2D(mid,i,j+1).val[0]);
				if(rgb>255.0) rgb=255;
				for(k=0;k<3;k++) s.val[k]=rgb;
			}
			else
				s=cvGet2D(mid,i,j);
	
		cvSet2D(dst,i,j,s);
		}
	}

	if(save)
	{
		cvSaveImage("edge_roberts.jpg",dst);
		printf("%s has been saved as \"%s\".\n","edge_roberts_img","edge_roberts.jpg");
	}

	return dst;
}

IplImage* edge_prewitt(int save)    
{
    IplImage *src = 0;
    IplImage *mid = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k,m,n;
	char filename[50];
	double prewitt_x[3][3]={{-1.0,0.0,1.0},{-1.0,0.0,1.0},{-1.0,0.0,1.0}};
	double prewitt_y[3][3]={{-1.0,-1.0,-1.0},{0.0,0.0,0.0},{1.0,1.0,1.0}};
	double rgb,rgb_x,rgb_y;
	
	src=get_img(filename);
	dst_cvsize.width = (int)(src->width);
	dst_cvsize.height = (int)(src->height);
	dst=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
	mid=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
	mid=gray_img(src);

	for(i=0;i<dst_cvsize.height;i++)
	{
		for(j=0;j<dst_cvsize.width;j++)
		{
			
			if(i>0 && i<dst_cvsize.height-1 && j>0 && j<dst_cvsize.width-1)
			{
				rgb=0.0;rgb_x=0.0;rgb_y=0.0;
				for(m=0;m<3;m++) for(n=0;n<3;n++) rgb_x+=cvGet2D(mid,i+m-1,j+n-1).val[0]*prewitt_x[m][n];
				for(m=0;m<3;m++) for(n=0;n<3;n++) rgb_y+=cvGet2D(mid,i+m-1,j+n-1).val[0]*prewitt_y[m][n];
				if(PRE_MODE) 
				{
					if(rgb_x+rgb_y>PRE_T*255.0) rgb=cvGet2D(mid,i,j).val[0];else rgb=0;
				}
				else 
				{
					if(rgb_x>PRE_T*255.0 || rgb_y>PRE_T*255.0) rgb=cvGet2D(mid,i,j).val[0];else rgb=0;
				}
				for(k=0;k<3;k++) s.val[k]=rgb;
			}
			else
				s=cvGet2D(mid,i,j);
			cvSet2D(dst,i,j,s);
		}
	}

	if(save)
	{
		cvSaveImage("edge_prewitt.jpg",dst);
		printf("%s has been saved as \"%s\".\n","edge_prewitt_img","edge_prewitt.jpg");
	}

	return dst;
}

IplImage* edge_log(int save)    
{
    IplImage *src = 0;
    IplImage *mid = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k,m,n;
	char filename[50];
	double lg[5][5]={{0.0,0.0,-1.0,0.0,0.0},{0.0,-1.0,-2.0,-1.0,0.0},{-1.0,-2.0,16.0,-2.0,-1.0},{0.0,-1.0,-2.0,-1.0,0.0},{0.0,0.0,-1.0,0.0,0.0}};
	double rgb;
	
	src=get_img(filename);
	dst_cvsize.width = (int)(src->width);
	dst_cvsize.height = (int)(src->height);
	dst=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
	mid=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
	mid=gray_img(src);

	for(i=0;i<dst_cvsize.height;i++)
	{
		for(j=0;j<dst_cvsize.width;j++)
		{
			
				if(i>1 && i<dst_cvsize.height-2 && j>1 && j<dst_cvsize.width-2)
				{
					rgb=0.0;
					for(m=0;m<5;m++) for(n=0;n<5;n++) rgb+=cvGet2D(mid,i+m-2,j+n-2).val[0]*lg[m][n];
					//if(rgb*cvGet2D(dst,i-1,j).val[0]>=0 && rgb*cvGet2D(dst,i,j-1).val[0]>=0) rgb=0.0; //zero-crossing detection
					for(k=0;k<3;k++) s.val[k]=rgb;
				}
				else
					for(k=0;k<3;k++) s.val[k]=0.0;
			cvSet2D(dst,i,j,s);
		}
	}

	if(save)
	{
		cvSaveImage("edge_log.jpg",dst);
		printf("%s has been saved as \"%s\".\n","edge_log_img","edge_log.jpg");
	}

	return dst;
}

IplImage* gaussian_blur(int save)    
{
    IplImage *src = 0;
    IplImage *mid = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k,m,n;
	char filename[50];
	double gaussian_kernel[5][5]={{2.0/159,4.0/159,5.0/159,4.0/159,2.0/159},{4.0/159,9.0/159,12.0/159,9.0/159,4.0/159},\
								{5.0/159,12.0/159,15.0/159,12.0/159,5.0/159},{4.0/159,9.0/159,12.0/159,9.0/159,4.0/159},\
								{2.0/159,4.0/159,5.0/159,4.0/159,2.0/159}};
	double rgb;
	
	src=get_img(filename);
	dst_cvsize.width = (int)(src->width);
	dst_cvsize.height = (int)(src->height);
	dst=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
	mid=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
	mid=gray_img(src);

	for(i=0;i<dst_cvsize.height;i++)
	{
		for(j=0;j<dst_cvsize.width;j++)
		{
			
				if(i>1 && i<dst_cvsize.height-2 && j>1 && j<dst_cvsize.width-2)
				{
					rgb=0.0;
					for(m=0;m<5;m++) for(n=0;n<5;n++) rgb+=cvGet2D(mid,i+m-2,j+n-2).val[0]*gaussian_kernel[m][n];
					for(k=0;k<3;k++) s.val[k]=rgb;
				}
				else
					for(k=0;k<3;k++) s.val[k]=0.0;
			cvSet2D(dst,i,j,s);
		}
	}

	if(save)
	{
		cvSaveImage("gaussian_blur.jpg",dst);
		printf("%s has been saved as \"%s\".\n","gaussian_blur_img","gaussian_blur.jpg");
	}

	return dst;
}

IplImage* edge_canny(int save)    
{
    IplImage *src = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k,m,grd;
	double rgb,dgr;
	FILE *grad;
	
	src=edge_sobel(0,1);

	dst_cvsize.width = (int)(src->width);
	dst_cvsize.height = (int)(src->height);
	dst=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
	
	grad=fopen("sobel_grad.txt","r");
	for(i=0;i<dst_cvsize.height;i++)
	{
		for(j=0;j<dst_cvsize.width;j++)
		{
			
				if(i>0 && i<dst_cvsize.height-1 && j>0 && j<dst_cvsize.width-1)
				{
					fscanf(grad,"%lf ",&dgr);
					grd=int((dgr*180/PI+22.5)/45);
					switch(grd)
					{
					case 4:case -4:case 0:
						if(cvGet2D(src,i,j).val[0]>=cvGet2D(src,i,j-1).val[0] && cvGet2D(src,i,j).val[0]>=cvGet2D(src,i,j+1).val[0]) 
							rgb=cvGet2D(src,i,j).val[0];
						else
							rgb=0.0;break;
					case 1:case -3:
						if(cvGet2D(src,i,j).val[0]>=cvGet2D(src,i-1,j+1).val[0] && cvGet2D(src,i,j).val[0]>=cvGet2D(src,i+1,j-1).val[0]) 
							rgb=cvGet2D(src,i,j).val[0];
						else
							rgb=0.0;break;
					case 2:case -2:
						if(cvGet2D(src,i,j).val[0]>=cvGet2D(src,i-1,j).val[0] && cvGet2D(src,i,j).val[0]>=cvGet2D(src,i+1,j).val[0]) 
							rgb=cvGet2D(src,i,j).val[0];
						else
							rgb=0.0;break;
					case 3:case -1:
						if(cvGet2D(src,i,j).val[0]>=cvGet2D(src,i+1,j+1).val[0] && cvGet2D(src,i,j).val[0]>=cvGet2D(src,i-1,j-1).val[0]) 
							rgb=cvGet2D(src,i,j).val[0];
						else
							rgb=0.0;break;
					default:rgb=0.0;break;
					}
					for(k=0;k<3;k++) s.val[k]=rgb;
				}
				else
					for(k=0;k<3;k++) s.val[k]=0.0;
			cvSet2D(dst,i,j,s);
		}
	}
	fclose(grad);

	for(i=0;i<dst_cvsize.height;i++)
	{
		for(j=0;j<dst_cvsize.width;j++)
		{
			if(i>0 && i<dst_cvsize.height-1 && j>0 && j<dst_cvsize.width-1)
			{
				rgb=cvGet2D(dst,i,j).val[0];
				if(rgb<255*CANNY_T2)
					rgb=0.0;
				else if(rgb<255*CANNY_T1)
				{
					for(m=0;m<9;m++) if(cvGet2D(dst,i+(m/3)-1,j+(m%3)-1).val[0]>=255*CANNY_T1) break;
					if(m==9) rgb=0.0;
				}
				for(k=0;k<3;k++) s.val[k]=rgb;
			}
			else
				for(k=0;k<3;k++) s.val[k]=0.0;
			cvSet2D(dst,i,j,s);
		}
	}

	if(save)
	{
		cvSaveImage("edge_canny.jpg",dst);
		printf("%s has been saved as \"%s\".\n","edge_canny_img","edge_canny.jpg");
	}

	return dst;
}

IplImage* edge_hough(int save, int step1, int step2)
{
    IplImage *src = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,x,y,flg,m,n,mc_,mc=0;
	double max;
	int **hDom;
	hDom=(int**)malloc(step1*sizeof(int*));
	for(i=0;i<step1;i++) hDom[i]=(int*)malloc(step2*sizeof(int));
	
	for(i=0;i<step1;i++) for(j=0;j<step2;j++) hDom[i][j]=0;

	src=edge_canny();

	dst_cvsize.width = (int)(src->width);
	dst_cvsize.height = (int)(src->height);
	dst=cvCreateImage(dst_cvsize, src->depth, src->nChannels);

	max=sqrt(dst_cvsize.width*dst_cvsize.width+dst_cvsize.height*dst_cvsize.height);
	
	for(i=0;i<dst_cvsize.height;i++)
		for(j=0;j<dst_cvsize.width;j++)
			if(cvGet2D(src,i,j).val[0]>127) 
				for(n=0;n<step2;n++)
				{
					m=int((j*cos(n*PI/step2)/(2*max)+i*sin(n*PI/step2)/(2*max)+0.5)*step1);
					if(hDom[m][n]<HOUGH_L) hDom[m][n]++;
				}

	for(i=0;i<step1;i++) for(j=0;j<step2;j++) if(hDom[i][j]>mc) mc=hDom[i][j];

	for(i=0;i<step1;i++) 
		for(j=0;j<step2;j++) 
		{
			mc_=hDom[i][j];
			flg=1;
			for(m=0;m<2*HOUGH_C+1 && flg;m++) for(n=0;n<2*HOUGH_C+1 && flg;n++) 
				if((i+m-HOUGH_C)>=0 && (i+m-HOUGH_C)<step1) 
				{
					if(mc_<hDom[i+m-HOUGH_C][(j+n-HOUGH_C+step2)%step2]) flg=0;
				}
				else continue;
			if(mc_>=mc*HOUGH_T && flg)
			{
				hDom[i][j]++;
				for(m=0;m<dst_cvsize.width;m++)
				{
					if(j)
					{
						n=int(((double(i)/step1-0.5)*2*max-m*cos(j*PI/step2))/sin(j*PI/step2));
						if(n>=0 && n<dst_cvsize.height)
						{
							flg=1;
							for(x=0;x<2*HOUGH_A+1 && flg;x++) for(y=0;y<2*HOUGH_A+1 && flg;y++) 
								if((n+x-HOUGH_A)>=0 && (n+x-HOUGH_A)<dst_cvsize.height && (m+y-HOUGH_A)>=0 && (m+y-HOUGH_A)<dst_cvsize.width) 
								{
									if(cvGet2D(src,n+x-HOUGH_A,m+y-HOUGH_A).val[0]>=127) flg=0;
								}
								else continue;
							if(!flg)
							{
								s.val[0]=0.0;
								s.val[1]=0.0;
								s.val[2]=255.0;
								cvSet2D(dst,n,m,s);
							}
						}
					}
					else if(m==int((double(i)/step1-0.5)*2*max))
						for(n=0;n<dst_cvsize.height;n++)
						{
							flg=1;
							for(x=0;x<2*HOUGH_A+1 && flg;x++) for(y=0;y<2*HOUGH_A+1 && flg;y++) 
								if((n+x-HOUGH_A)>=0 && (n+x-HOUGH_A)<dst_cvsize.height && (m+y-HOUGH_A)>=0 && (m+y-HOUGH_A)<dst_cvsize.width) 
								{
									if(cvGet2D(src,n+x-HOUGH_A,m+y-HOUGH_A).val[0]>=127) flg=0;
								}
								else continue;
							if(!flg)
							{
								s.val[0]=0.0;
								s.val[1]=0.0;
								s.val[2]=255.0;
								cvSet2D(dst,n,m,s);
							}
						}
				}
			}
		}

	if(save)
	{
		cvSaveImage("edge_hough.jpg",dst);
		printf("%s has been saved as \"%s\".\n","edge_hough_img","edge_hough.jpg");
	}

	for(i=0;i<step1;i++) free(hDom[i]);
	free(hDom);

	return dst;
}
