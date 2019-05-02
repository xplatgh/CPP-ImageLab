#include "DCT.H"

IplImage* DCT_img(int save,int comp)
{
    IplImage *src = 0;
    IplImage *mid = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k,fi,fj;
	char filename[50];
	src=get_img(filename);
	double mag,max=0.0;
	FILE *ft;

    dst_cvsize.width = DCT_N;
    dst_cvsize.height = DCT_N;
	
    mid=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
    dst=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
	cvResize(src,mid);

	mid=gray_img(mid);

	printf("Processing     0%...");
	for(i=0;i<DCT_N;i++)
		for(j=0;j<DCT_N;j++)
		{
			printf("\b\b\b\b\b\b\b%3d%%...",int((i*DCT_N+j+1)*100/(DCT_N*DCT_N*2)));
			mag=0.0;
			for(fi=0;fi<DCT_N;fi++)
				for(fj=0;fj<DCT_N;fj++)
					mag=mag+cvGet2D(mid,fi,fj).val[0]*cos((2*j+1)*fj*PI/(2*DCT_N))*cos((2*i+1)*fi*PI/(2*DCT_N));
			mag=sqrt((i?2:1)/(double)DCT_N)*sqrt((j?2:1)/(double)DCT_N)*fabs(mag);
			if(mag>max) max=mag;
		}

	ft=fopen("dct.txt","w");
	for(i=0;i<DCT_N;i++)
		for(j=0;j<DCT_N;j++)
		{
			printf("\b\b\b\b\b\b\b%3d%%...",int((i*DCT_N+j+1)*100/(DCT_N*DCT_N*2))+50);
			mag=0.0;
			for(fi=0;fi<DCT_N;fi++)
				for(fj=0;fj<DCT_N;fj++)
					mag=mag+cvGet2D(mid,fi,fj).val[0]*cos((2*fj+1)*j*PI/(2*DCT_N))*cos((2*fi+1)*i*PI/(2*DCT_N));
			mag=sqrt((i?2:1)/(double)DCT_N)*sqrt((j?2:1)/(double)DCT_N)*mag;
			if(comp && fabs(mag)<DCT_B) mag=0.0;
			fprintf(ft,"%lf ",mag);
			for(k=0;k<3;k++) s.val[k]=255*log(1+fabs(mag))/log(1+max);
			cvSet2D(dst,i,j,s);
		}
	printf("\b\b  \n");
	fclose(ft);
	printf("\"dct.txt\" has been saved.\n");

	if(save)
	{
		cvSaveImage("DCT.jpg",dst);
		printf("DCT_image has been saved as \"%s\".\n","DCT.jpg");
	}

    return dst;
}


IplImage* IDCT_img(int save,char *mag_file)
{
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k,fi,fj;
	double mag,mag0;
	FILE *ft;

    dst_cvsize.width = DCT_N;
    dst_cvsize.height = DCT_N;
	
    dst=cvCreateImage(dst_cvsize, IPL_DEPTH_8U, 3);

	ft=fopen(mag_file,"r");
	printf("Processing     0%...");
	for(i=0;i<DCT_N;i++)
		for(j=0;j<DCT_N;j++)
		{
			printf("\b\b\b\b\b\b\b%3d%%...",int((i*DCT_N+j+1)*100/(DCT_N*DCT_N)));
			rewind(ft);
			mag=0.0;
			for(fi=0;fi<DCT_N;fi++)
				for(fj=0;fj<DCT_N;fj++)
				{
					fscanf(ft,"%lf ",&mag0);
					mag0=mag0*cos((2*j+1)*fj*PI/(2*DCT_N))*cos((2*i+1)*fi*PI/(2*DCT_N));
					mag=mag+sqrt((fi?2:1)/(double)DCT_N)*sqrt((fj?2:1)/(double)DCT_N)*mag0;
				}
			for(k=0;k<3;k++) s.val[k]=mag;
			cvSet2D(dst,i,j,s);
		}
	printf("\b\b  \n");
	fclose(ft);
	
	if(save)
	{
		cvSaveImage("IDCT.jpg",dst);
		printf("IDCT_image has been saved as \"%s\".\n","IDCT.jpg");
	}

    return dst;
}
