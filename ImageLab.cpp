#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <time.h>

#include "edge_detectors.h"

int save()
{
	int sav=0;
	printf("Do you want to save the result?\n");
	printf("[1 : Yes]\n");
	printf("[0 : No]\n");
	scanf("%d",&sav);
	return sav;
}

void show(char *showname="src", IplImage *src=0)
{
		cvNamedWindow(showname);
		cvShowImage(showname,src);
		cvWaitKey(0); 
		cvDestroyWindow(showname);
		cvReleaseImage(&src);
}

int main(int argc, char* argv[])
{
	int sig=1,sel=1;
	srand(time(NULL));

	printf("--------------------Image Lab--------------------\n");
	printf("-------------------------------------------------\n");
	while(sig)
	{
		printf("--------------------main menu--------------------\n");
		printf("Please select:\n");
		printf("[1 : basic operations]\n");
		printf("[2 : gray processing]\n");
		printf("[3 : DFT & DCT]\n");
		printf("[4 : filters]\n");
		printf("[5 : edge detection]\n");
		printf("[6 : noise & blur]\n");
		printf("[0 : exit]\n");
		scanf("%d",&sig);
		switch(sig)
		{
		case 1:
			{
				printf("--basic operations--\n");
				printf("Please select:\n");
				printf("[1 : addition]\n");
				printf("[2 : subtraction]\n");
				printf("[3 : multiplication]\n");
				printf("[4 : cutting]\n");
				printf("[5 : rotation]\n");
				printf("[0 : return]\n");
				scanf("%d",&sel);
				switch(sel)
				{
				case 1:show("addition",add_img(save()));break;
				case 2:show("subtraction",sub_img(save()));break;
				case 3:show("multiplication",mul_img(save()));break;
				case 4:show("cutting",cut_img(save()));break;
				case 5:show("rotation",rot_img(save()));break;
				default:break;
				}
				break;
			}
		case 2:
			{
				char *showname[4]={"linearise","log","invert","binarize"};
				printf("--gray processing--\n");
				printf("Please select:\n");
				printf("[1 : linearise]\n");
				printf("[2 : log]\n");
				printf("[3 : invert]\n");
				printf("[4 : binarize]\n");
				printf("[0 : return]\n");
				scanf("%d",&sel);
				if(sel) show(showname[sel-1],GT_img(sel,save()));
				break;
			}
		case 3:
			{
				printf("--DFT & DCT--\n");
				printf("Please select:\n");
				printf("[1 : DFT]\n");
				printf("[2 : IDFT]\n");
				printf("[3 : DCT]\n");
				printf("[4 : IDCT]\n");
				printf("[0 : return]\n");
				scanf("%d",&sel);
				switch(sel)
				{
				case 1:show("DFT",DFT_img(save()));break;
				case 2:show("IDFT",IDFT_img(save()));break;
				case 3:show("DCT",DCT_img(save()));break;
				case 4:show("IDCT",IDCT_img(save()));break;
				default:break;
				}
				break;
			}
		case 4:
			{
				printf("--filters--\n");
				printf("Please select:\n");
				printf("[1 : mean filter]\n");
				printf("[2 : median filter]\n");
				printf("[3 : BLPF]\n");
				printf("[4 : ELPF]\n");
				printf("[5 : TLPF]\n");
				printf("[6 : Sobel filter]\n");
				printf("[7 : Prewitt filter]\n");
				printf("[8 : Laplace filter]\n");
				printf("[9 : Homomorphic filter]\n");
				printf("[0 : return]\n");
				scanf("%d",&sel);
				switch(sel)
				{
				case 1:show("mean",ave_filter(save()));break;
				case 2:show("median",med_filter(save()));break;
				case 3:show("BLPF",blpf(save()));break;
				case 4:show("ELPF",elpf(save()));break;
				case 5:show("TLPF",tlpf(save()));break;
				case 6:show("Sobel",sobel_filter(save()));break;
				case 7:show("Prewitt",prewitt_filter(save()));break;
				case 8:show("Laplace",lap_filter(save()));break;
				case 9:show("Homomorphic",hmmph_filter(save()));break;
				default:break;
				}
				break;
			}
		case 5:
			{
				printf("--edge detection--\n");
				printf("Please select:\n");
				printf("[1 : Sobel]\n");
				printf("[2 : Roberts]\n");
				printf("[3 : Prewitt]\n");
				printf("[4 : LoG]\n");
				printf("[5 : Canny]\n");
				printf("[6 : Hough]\n");
				printf("[0 : return]\n");
				scanf("%d",&sel);
				switch(sel)
				{
				case 1:show("Sobel",edge_sobel(save()));break;
				case 2:show("Roberts",edge_roberts(save()));break;
				case 3:show("Prewitt",edge_prewitt(save()));break;
				case 4:show("LoG",edge_log(save()));break;
				case 5:show("Canny",edge_canny(save()));break;
				case 6:show("Hough",edge_hough(save()));break;
				default:break;
				}
				break;
			}
		case 6:
			{
				double para1,para2;
				printf("--noise & blur--\n");
				printf("Please select:\n");
				printf("[1 : Salt & Pepper noise]\n");
				printf("[2 : Gaussian noise]\n");
				printf("[3 : Gaussian blur]\n");
				printf("[0 : return]\n");
				scanf("%d",&sel);
				if(sel==1)
				{
					printf("Please input the lower and upper bounds:([0.0,1.0]) separated by \",\":\n");
					scanf("%lf,%lf",&para1,&para2);
					show("Salt & Pepper",add_noise(0,para1,para2,save()));
				}
				if(sel==2)
				{
					printf("Please input the mean and variance:([0.0,1.0]) separated by \",\":\n");
					scanf("%lf,%lf",&para1,&para2);
					show("Gaussian noise",add_noise(1,para1,para2,save()));
				}
				if(sel==3)
					show("Gaussian blur",gaussian_blur(save()));
				break;
			}
		default:break;
		}
	}
	printf("-------------------------------------------------\n");
	printf("--------------------Goodbye !--------------------\n");

    return 0;
}
