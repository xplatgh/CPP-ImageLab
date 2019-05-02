#include "filters.h"

IplImage* add_noise(int type, double para1, double para2, int save)
{
    IplImage *src = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	char filename[50];
	char *saveName[] = {"salt-pepper.jpg", "gaussian.jpg"};
	int i,j,k,rgb;
	
	src=get_img(filename);
    dst_cvsize.width = (int)(src->width);
    dst_cvsize.height = (int)(src->height);
    dst=cvCreateImage(dst_cvsize, src->depth, src->nChannels);

	for(i=0;i<dst_cvsize.height;i++) 
		for(j=0;j<dst_cvsize.width;j++)
		{
			s=cvGet2D(src,i,j);
			for(k=0;k<3;k++)
			{
				rgb=int(s.val[k]);
				if(type) 
				{
					double u=(rand()/double(32768)); 
					double v=(rand()/double(32768));
					rgb=(rgb+abs(int(para2*255*sqrt(-2*log(u))*cos(2*PI*v)+para1*255)))%256;
				}
				else
					rgb=(rgb+int(rand()*(para2-para1)*255/double(32768)+para1*255))%256;
				s.val[k]=rgb;
			}
			cvSet2D(dst,i,j,s);
		}
	
	if(save)
	{
		cvSaveImage(saveName[type],dst);
		printf("%s has been saved as \"%s\".\n",saveName[type],saveName[type]);
	}

    return dst;
}

IplImage* ave_filter(int save)
{
    IplImage *src = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k,m,n;
	char filename[50];
	double ave[3][3]={{1/9.0,1/9.0,1/9.0},{1/9.0,1/9.0,1/9.0},{1/9.0,1/9.0,1/9.0}};
	double rgb;
	
	src=get_img(filename);
	dst_cvsize.width = (int)(src->width);
	dst_cvsize.height = (int)(src->height);
	dst=cvCreateImage(dst_cvsize, src->depth, src->nChannels);

	for(i=0;i<dst_cvsize.height;i++)
		for(j=0;j<dst_cvsize.width;j++)
		{
			for(k=0;k<3;k++)
			{
			if(i>0 && i<dst_cvsize.height-1 && j>0 && j<dst_cvsize.width-1)
			{
				rgb=0.0;
				for(m=0;m<3;m++) for(n=0;n<3;n++) rgb+=cvGet2D(src,i+m-1,j+n-1).val[k]*ave[m][n];
				s.val[k]=rgb;
			}
			else
				s.val[k]=cvGet2D(src,i,j).val[k];
			}
			cvSet2D(dst,i,j,s);
		}

	if(save)
	{
		cvSaveImage("mean.jpg",dst);
		printf("%s has been saved as \"%s\".\n","mean_img","mean.jpg");
	}

	return dst;
}

IplImage* med_filter(int save) 
{
    IplImage *src = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,vk,k,kk,m,n,mc;
	char filename[50];
	double rgb,sort[9];
	
	src=get_img(filename);
	dst_cvsize.width = (int)(src->width);
	dst_cvsize.height = (int)(src->height);
	dst=cvCreateImage(dst_cvsize, src->depth, src->nChannels);

	for(i=0;i<dst_cvsize.height;i++)
		for(j=0;j<dst_cvsize.width;j++)
		{
			for(vk=0;vk<3;vk++)
			{
			mc=1;
			sort[0]=0.0;
			for(m=0;m<3;m++) 
				for(n=0;n<3;n++)
					if(i+m-1>=0 && i+m-1<dst_cvsize.height && j+n-1>=0 && j+n-1<dst_cvsize.width)
					{
						for(k=0;k<mc;k++) if(cvGet2D(src,i+m-1,j+n-1).val[vk]>sort[k]) break;
						for(kk=mc-1;kk>=k;kk--) sort[kk+1]=sort[kk];
						sort[k]=cvGet2D(src,i+m-1,j+n-1).val[vk];
						mc++;
					}
			mc--;
			rgb=(mc%2?sort[(mc-1)/2]:(sort[mc/2]+sort[mc/2-1])/2.0);
			s.val[vk]=rgb;
			}
			cvSet2D(dst,i,j,s);
		}

	if(save)
	{
		cvSaveImage("med.jpg",dst);
		printf("%s has been saved as \"%s\".\n","med_img","med.jpg");
	}

	return dst;
}

IplImage* blpf(int save)
{
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k,fi,fj;
	double mag,pha,rp,ip,expo,fx,fy;
	FILE *ft,*lpf,*ph;

	dst_cvsize.width = DFT_X;
	dst_cvsize.height = DFT_Y;
	dst=cvCreateImage(dst_cvsize, IPL_DEPTH_8U, 3);

	ft=fopen("dftma.txt","r");
	lpf=fopen("lpf.txt","w");
	for(i=0;i<DFT_Y;i++)
	{
		for(j=0;j<DFT_X;j++)
		{
			fx=(DFT_X-1)/2.0-fabs(j-(DFT_X-1)/2.0);
			fy=(DFT_Y-1)/2.0-fabs(j-(DFT_Y-1)/2.0);
			fscanf(ft,"%lf ",&mag);
			mag=mag/((1+(sqrt(2)-1)*pow(fx/D0,2*ORDER))*(1+(sqrt(2)-1)*pow(fy/D0,2*ORDER)));
			fprintf(lpf,"%lf ",mag);
		}
	}
	fclose(ft);
	fclose(lpf);

	lpf=fopen("lpf.txt","r");
	ph=fopen("dftph.txt","r");
	printf("Processing     0%...");
	for(i=0;i<DFT_Y;i++)
		for(j=0;j<DFT_X;j++)
		{
			printf("\b\b\b\b\b\b\b%3d%%...",int((i*DFT_X+j+1)*100/(DFT_Y*DFT_X)));
			rp=0.0;
			ip=0.0;
			rewind(lpf);
			rewind(ph);
			for(fi=0;fi<DFT_Y;fi++)
				for(fj=0;fj<DFT_X;fj++)
				{
					fscanf(lpf,"%lf ",&mag);
					fscanf(ph,"%lf ",&pha);
					expo=2*PI*(j*fj/double(DFT_X)+i*fi/double(DFT_Y));
					rp=rp+mag*cos(pha+expo);
					ip=ip+mag*sin(pha+expo);
				}
			mag=sqrt(rp*rp+ip*ip)/(DFT_X*DFT_Y);
			for(k=0;k<3;k++) s.val[k]=mag;
			cvSet2D(dst,i,j,s);
		}
	printf("\b\b  \n");
	fclose(lpf);
	fclose(ph);

	if(save)
	{
		cvSaveImage("blpf.jpg",dst);
		printf("%s has been saved as \"%s\".\n","blpf_img","blpf.jpg");
	}

	return dst;
}

IplImage* elpf(int save) 
{
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k,fi,fj;
	double mag,pha,rp,ip,expo,fx,fy;
	FILE *ft,*lpf,*ph;
	
	dst_cvsize.width = DFT_X;
	dst_cvsize.height = DFT_Y;
	dst=cvCreateImage(dst_cvsize, IPL_DEPTH_8U, 3);

	ft=fopen("dftma.txt","r");
	lpf=fopen("lpf.txt","w");
	for(i=0;i<DFT_Y;i++)
	{
		for(j=0;j<DFT_X;j++)
		{
			fx=(DFT_X-1)/2.0-fabs(j-(DFT_X-1)/2.0);
			fy=(DFT_Y-1)/2.0-fabs(j-(DFT_Y-1)/2.0);
			fscanf(ft,"%lf ",&mag);
			mag=mag*exp(log(1/sqrt(2.0))*pow(fx/D0,ORDER))*exp(log(1/sqrt(2.0))*pow(fy/D0,ORDER));
			fprintf(lpf,"%lf ",mag);
		}
	}
	fclose(ft);
	fclose(lpf);

	lpf=fopen("lpf.txt","r");
	ph=fopen("dftph.txt","r");
	printf("Processing     0%...");
	for(i=0;i<DFT_Y;i++)
		for(j=0;j<DFT_X;j++)
		{
			printf("\b\b\b\b\b\b\b%3d%%...",int((i*DFT_X+j+1)*100/(DFT_Y*DFT_X)));
			rp=0.0;
			ip=0.0;
			rewind(lpf);
			rewind(ph);
			for(fi=0;fi<DFT_Y;fi++)
				for(fj=0;fj<DFT_X;fj++)
				{
					fscanf(lpf,"%lf ",&mag);
					fscanf(ph,"%lf ",&pha);
					expo=2*PI*(j*fj/double(DFT_X)+i*fi/double(DFT_Y));
					rp=rp+mag*cos(pha+expo);
					ip=ip+mag*sin(pha+expo);
				}
			mag=sqrt(rp*rp+ip*ip)/(DFT_X*DFT_Y);
			for(k=0;k<3;k++) s.val[k]=mag;
			cvSet2D(dst,i,j,s);
		}
	printf("\b\b  \n");
	fclose(lpf);
	fclose(ph);

	if(save)
	{
		cvSaveImage("elpf.jpg",dst);
		printf("%s has been saved as \"%s\".\n","elpf_img","elpf.jpg");
	}

	return dst;
}

IplImage* tlpf(int save)
{
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k,fi,fj;
	double mag,pha,rp,ip,expo,fx,fy;
	FILE *ft,*lpf,*ph;
	
	{
		dst_cvsize.width = DFT_X;
		dst_cvsize.height = DFT_Y;
		dst=cvCreateImage(dst_cvsize, IPL_DEPTH_8U, 3);
	}


	ft=fopen("dftma.txt","r");
	lpf=fopen("lpf.txt","w");
	for(i=0;i<DFT_Y;i++)
	{
		for(j=0;j<DFT_X;j++)
		{
			fx=(DFT_X-1)/2.0-fabs(j-(DFT_X-1)/2.0);
			fy=(DFT_Y-1)/2.0-fabs(j-(DFT_Y-1)/2.0);
			fscanf(ft,"%lf ",&mag);
			if(fx<D0) mag=mag;else if(fx>D1) mag=0;else mag=mag*(fx-D1)/(D0-D1);
			if(fy<D0) mag=mag;else if(fy>D1) mag=0;else mag=mag*(fy-D1)/(D0-D1);
			fprintf(lpf,"%lf ",mag);
		}
	}
	fclose(ft);
	fclose(lpf);

	lpf=fopen("lpf.txt","r");
	ph=fopen("dftph.txt","r");
	printf("Processing     0%...");
	for(i=0;i<DFT_Y;i++)
		for(j=0;j<DFT_X;j++)
		{
			printf("\b\b\b\b\b\b\b%3d%%...",int((i*DFT_X+j+1)*100/(DFT_Y*DFT_X)));
			rp=0.0;
			ip=0.0;
			rewind(lpf);
			rewind(ph);
			for(fi=0;fi<DFT_Y;fi++)
				for(fj=0;fj<DFT_X;fj++)
				{
					fscanf(lpf,"%lf ",&mag);
					fscanf(ph,"%lf ",&pha);
					expo=2*PI*(j*fj/double(DFT_X)+i*fi/double(DFT_Y));
					rp=rp+mag*cos(pha+expo);
					ip=ip+mag*sin(pha+expo);
				}
			mag=sqrt(rp*rp+ip*ip)/(DFT_X*DFT_Y);
			for(k=0;k<3;k++) s.val[k]=mag;
			cvSet2D(dst,i,j,s);
		}
	printf("\b\b  \n");
	fclose(lpf);
	fclose(ph);

	if(save)
	{
		cvSaveImage("tlpf.jpg",dst);
		printf("%s has been saved as \"%s\".\n","tlpf_img","tlpf.jpg");
	}

	return dst;
}

IplImage* sobel_filter(int save)
{
    IplImage *src = 0;
    IplImage *mid = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k,m,n;
	char filename[50];
	double sobel_x[3][3]={{1.0,0.0,-1.0},{2.0,0.0,-2.0},{1.0,0.0,-1.0}};
	double sobel_y[3][3]={{-1.0,-2.0,-1.0},{0.0,0.0,0.0},{1.0,2.0,1.0}};
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
				for(m=0;m<3;m++) for(n=0;n<3;n++) rgb_x+=cvGet2D(mid,i+m-1,j+n-1).val[0]*sobel_x[m][n];
					for(m=0;m<3;m++) for(n=0;n<3;n++) rgb_y+=cvGet2D(mid,i+m-1,j+n-1).val[0]*sobel_y[m][n];
					rgb=sqrt(rgb_x*rgb_x+rgb_y*rgb_y);
				
				for(k=0;k<3;k++) s.val[k]=rgb;
			}
			else
				s=cvGet2D(mid,i,j);
			cvSet2D(dst,i,j,s);
		}
	}

	if(save)
	{
		cvSaveImage("sobel.jpg",dst);
		printf("%s has been saved as \"%s\".\n","sobel_img","sobel.jpg");
	}

	return dst;
}

IplImage* prewitt_filter(int save)
{
    IplImage *src = 0;
    IplImage *mid = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k,m,n;
	char filename[50];
	double prewitt_x[3][3]={{1.0,0.0,-1.0},{1.0,0.0,-1.0},{1.0,0.0,-1.0}};
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
						if(rgb_x+rgb_y>PRE_T*255.0) rgb=cvGet2D(mid,i,j).val[0];
						else rgb=0;
					}
					else 
					{
						if(rgb_x>PRE_T*255.0 || rgb_y>PRE_T*255.0) rgb=cvGet2D(mid,i,j).val[0];
						else rgb=0;
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
		cvSaveImage("prewitt.jpg",dst);
		printf("%s has been saved as \"%s\".\n","prewitt_img","prewitt.jpg");
	}

	return dst;
}

IplImage* lap_filter(int save)
{
    IplImage *src = 0;
    IplImage *mid = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k,m,n;
	char filename[50];
	double lap[3][3]={{0.0,1.0,0.0},{1.0,-4.0,1.0},{0.0,1.0,0.0}};
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
				for(m=0;m<3;m++) for(n=0;n<3;n++) rgb+=cvGet2D(mid,i+m-1,j+n-1).val[0]*lap[m][n];
				for(k=0;k<3;k++) s.val[k]=rgb;
			}
			else
				s=cvGet2D(mid,i,j);
			cvSet2D(dst,i,j,s);
		}
	}

	if(save)
	{
		cvSaveImage("lap.jpg",dst);
		printf("%s has been saved as \"%s\".\n","lap_img","lap.jpg");
	}

	return dst;
}

IplImage* hmmph_filter(int save)
{
    IplImage *src = 0;
    IplImage *mid = 0;
    IplImage *dst = 0;
    CvSize dst_cvsize;
	CvScalar s;
	int i,j,k,fi,fj;
	char filename[50];
	double mag,pha,rp,ip,expo,gray[256];
	FILE *ft,*ph,*hpf,*im;
	
	src=get_img(filename);
	dst_cvsize.width = DFT_X;
	dst_cvsize.height = DFT_Y;
	dst=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
	mid=cvCreateImage(dst_cvsize, src->depth, src->nChannels);
	cvResize(src,mid);
	mid=gray_img(mid);

	im=fopen("im.txt","w");
	for(i=0;i<DFT_Y;i++) for(j=0;j<DFT_X;j++) fprintf(im,"%lf ",log(cvGet2D(mid,i,j).val[0]));
	fclose(im);

	printf("Processing     0%...");
	im=fopen("im.txt","r");
	ft=fopen("hpf0.txt","w");
	ph=fopen("hpfph.txt","w");
	for(i=0;i<DFT_Y;i++)
		for(j=0;j<DFT_X;j++)		
		{
			printf("\b\b\b\b\b\b\b%3d%%...",int((i*DFT_X+j+1.0)*35/(DFT_Y*DFT_X)));
			rp=0.0;
			ip=0.0;
			rewind(im);
			for(fi=0;fi<DFT_Y;fi++)
				for(fj=0;fj<DFT_X;fj++)
				{
					fscanf(im,"%lf ",&mag);
					expo=2*PI*(j*fj/double(DFT_X)+i*fi/double(DFT_Y));
					rp=rp+mag*cos(expo);
					ip=ip-mag*sin(expo);
				} 
			fprintf(ft,"%lf ",sqrt(rp*rp+ip*ip));
			if(rp<0 && ip>=0) fprintf(ph,"%lf ",atan(ip/rp)+PI);
			else if(rp<0 && ip<0) fprintf(ph,"%lf ",atan(ip/rp)-PI);
			else if(rp==0 && ip>=0) fprintf(ph,"%lf ",PI/2);
			else if(rp==0 && ip<0) fprintf(ph,"%lf ",-PI/2);
			else fprintf(ph,"%lf ",atan(ip/rp));
		}
	fclose(im);
	fclose(ft);
	fclose(ph);

	ft=fopen("hpf0.txt","r");
	hpf=fopen("hpf1.txt","w");
	for(i=0;i<DFT_Y;i++)
	{
		for(j=0;j<DFT_X;j++)
		{
			fscanf(ft,"%lf ",&mag);
			mag=mag*((RH-RL)*(1-exp(-4.0*((i-(DFT_X-1)/2.0)*(i-(DFT_X-1)/2.0)/(((i-(DFT_Y-1)/2.0)*(i-(DFT_X-1)/2.0))))))+RL);
			fprintf(hpf,"%lf ",mag);
		}
	}
	fclose(ft);
	fclose(hpf);

	hpf=fopen("hpf1.txt","r");
	ph=fopen("hpfph.txt","r");
	for(i=0;i<DFT_Y;i++)
		for(j=0;j<DFT_X;j++)
		{
			printf("\b\b\b\b\b\b\b%3d%%...",int((i*DFT_X+j+1.0)*65/(DFT_Y*DFT_X)+35));
			rp=0.0;
			ip=0.0;
			rewind(hpf);
			rewind(ph);
			for(fi=0;fi<DFT_Y;fi++)
				for(fj=0;fj<DFT_X;fj++)
				{
					fscanf(hpf,"%lf ",&mag);
					fscanf(ph,"%lf ",&pha);
					expo=2*PI*(j*fj/double(DFT_X)+i*fi/double(DFT_Y));
					rp=rp+mag*cos(pha+expo);
					ip=ip+mag*sin(pha+expo);
				}
			mag=exp(sqrt(rp*rp+ip*ip)/(DFT_X*DFT_Y));
			for(k=0;k<3;k++) s.val[k]=mag;
			cvSet2D(dst,i,j,s);
		}
	printf("\b\b  \n");
	fclose(hpf);
	fclose(ph);

	for(i=0;i<256;i++) gray[i]=0.0;
	for(i=0;i<DFT_Y;i++) for(j=0;j<DFT_X;j++) gray[int(cvGet2D(mid,i,j).val[0])]+=1.0;
	for(i=0;i<255;i++) gray[i+1]+=gray[i];
	for(i=0;i<DFT_Y;i++) for(j=0;j<DFT_X;j++) 
	{
		for(k=0;k<3;k++) 
			s.val[k]=255*(gray[int(cvGet2D(mid,i,j).val[k])]/gray[255]);
		cvSet2D(dst,i,j,s);
	}
	
	if(save)
	{
		cvSaveImage("homomorphic.jpg",dst);
		printf("%s has been saved as \"%s\".\n","Homomorphic_img","homomorphic.jpg");
	}

	return dst;
}
