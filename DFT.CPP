#include "DFT.H"

IplImage* DFT_img(int save)
{
    IplImage *src = 0;
    IplImage *mid = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k,fi,fj;
	char filename[50];
	src=get_img(filename);
	double rp,ip,mag,exp,max=0.0;
	FILE *ft,*ph;

    dst_cvsize.width = DFT_X;
    dst_cvsize.height = DFT_Y;
	
    mid=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
    dst=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
	cvResize(src,mid);

	mid=gray_img(mid);

	printf("Processing     0%...");
	for(i=0;i<dst_cvsize.height;i++)
		for(j=0;j<dst_cvsize.width;j++)
		{
			printf("\b\b\b\b\b\b\b%3d%%...",int((i*dst_cvsize.width+j+1)*100/(dst_cvsize.height*dst_cvsize.width*2)));
			rp=0.0;
			ip=0.0;
			for(fi=0;fi<dst_cvsize.height;fi++)
				for(fj=0;fj<dst_cvsize.width;fj++)
				{
					mag=cvGet2D(mid,fi,fj).val[0];
					exp=2*PI*(j*fj/double(dst_cvsize.width)+i*fi/double(dst_cvsize.height));
					rp=rp+mag*cos(exp);
					ip=ip-mag*sin(exp);
				}
			mag=sqrt(rp*rp+ip*ip);
			if(mag>max) max=mag;
		}

	ft=fopen("dftma.txt","w");
	ph=fopen("dftph.txt","w");
	for(i=0;i<dst_cvsize.height;i++)
		for(j=0;j<dst_cvsize.width;j++)
		{
			printf("\b\b\b\b\b\b\b%3d%%...",int((i*dst_cvsize.width+j+1)*100/(dst_cvsize.height*dst_cvsize.width*2))+50);
			rp=0.0;
			ip=0.0;
			for(fi=0;fi<dst_cvsize.height;fi++)
				for(fj=0;fj<dst_cvsize.width;fj++)
				{
					mag=cvGet2D(mid,fi,fj).val[0];
					exp=2*PI*(j*fj/double(dst_cvsize.width)+i*fi/double(dst_cvsize.height));
					rp=rp+mag*cos(exp);
					ip=ip-mag*sin(exp);
				} 
			fprintf(ft,"%lf ",sqrt(rp*rp+ip*ip));
			if(rp<0 && ip>=0) fprintf(ph,"%lf ",atan(ip/rp)+PI);
			else if(rp<0 && ip<0) fprintf(ph,"%lf ",atan(ip/rp)-PI);
			else if(rp==0 && ip>=0) fprintf(ph,"%lf ",PI/2);
			else if(rp==0 && ip<0) fprintf(ph,"%lf ",-PI/2);
			else fprintf(ph,"%lf ",atan(ip/rp));
		}
	printf("\b\b  \n");
	fclose(ft);
	fclose(ph);
	printf("\"dftma.txt\" has been saved.\n");
	printf("\"dftph.txt\" has been saved.\n");

	ft=fopen("dftma.txt","r");
	for(i=0;i<dst_cvsize.height;i++)
		for(j=0;j<dst_cvsize.width;j++)
		{
			fscanf(ft,"%lf ",&mag);
			mag=255*log(1+mag)/log(1+max);
			for(k=0;k<3;k++) s.val[k]=mag;
			if(i<dst_cvsize.height/2 && j<dst_cvsize.width/2)
				cvSet2D(dst,i+dst_cvsize.height/2,j+dst_cvsize.width/2,s);
			else if(i<dst_cvsize.height/2 && j>=dst_cvsize.width/2)
				cvSet2D(dst,i+dst_cvsize.height/2,j-dst_cvsize.width/2,s);
			else if(i>=dst_cvsize.height/2 && j<dst_cvsize.width/2)
				cvSet2D(dst,i-dst_cvsize.height/2,j+dst_cvsize.width/2,s);
			else
				cvSet2D(dst,i-dst_cvsize.height/2,j-dst_cvsize.width/2,s);
		}
	fclose(ft);
	
	if(save)
	{
		cvSaveImage("DFT.jpg",dst);
		printf("DFT_image has been saved as \"%s\".\n","DFT.jpg");
	}

    return dst;
}

IplImage* IDFT_img(int save,char *mag_file,char *pha_file)
{
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k,fi,fj;
	double rp,ip,mag,pha,exp;
	FILE *ft,*ph;

    dst_cvsize.width = DFT_X;
    dst_cvsize.height = DFT_Y;
	
    dst=cvCreateImage(dst_cvsize, IPL_DEPTH_8U, 3);

	ft=fopen(mag_file,"r");
	ph=fopen(pha_file,"r");
	printf("Processing     0%...");
	for(i=0;i<dst_cvsize.height;i++)
		for(j=0;j<dst_cvsize.width;j++)
		{
			printf("\b\b\b\b\b\b\b%3d%%...",int((i*dst_cvsize.width+j+1)*100/(dst_cvsize.height*dst_cvsize.width)));
			rp=0.0;
			ip=0.0;
			rewind(ft);
			rewind(ph);
			for(fi=0;fi<dst_cvsize.height;fi++)
				for(fj=0;fj<dst_cvsize.width;fj++)
				{
					fscanf(ft,"%lf ",&mag);
					fscanf(ph,"%lf ",&pha);
					exp=2*PI*(j*fj/double(dst_cvsize.width)+i*fi/double(dst_cvsize.height));
					rp=rp+mag*cos(pha+exp);
					ip=ip+mag*sin(pha+exp);
				}
			mag=sqrt(rp*rp+ip*ip)/(dst_cvsize.height*dst_cvsize.width);
			for(k=0;k<3;k++) s.val[k]=mag;
			cvSet2D(dst,i,j,s);
		}
	printf("\b\b  \n");
	fclose(ft);
	fclose(ph);
	
	if(save)
	{
		cvSaveImage("IDFT.jpg",dst);
		printf("IDFT_image has been saved as \"%s\".\n","IDFT.jpg");
	}

    return dst;
}
