# CPP-ImageLab


This project consists of some implementation of several commonly used algorithms in the field of image processing, such as addition, rotation, 2D-DFT, 2D-DCT, gray transformation, spatial domain and frequency domain filters, edge detection, etc. All of the functions are implemented in C/C++ language, resulting in a Win32 console application. All of the results can be saved as pictures in .jpg format.





All of the implemented algorithms are as follows:


    1.Basic operations: addition, subtraction, multiplication, cutting, rotation; 

    2.Gray transformation(gray processing): linearise, log, invert, binarize; 

    3.DFT and DCT: DFT, IDFT, DCT, IDCT; 

    4.Filters: mean filter, median filter, BLPF(Butterworth Low-Pass Filter), ELPF(Exponential LPF), TLPF(Trapezoidal LPF), Sobel filter, Prewitt filter, Laplace filter, Homomorphic filter; 

    5.Edge detection: Sobel, Roberts, Prewitt, LoG(Laplacian-of-Gaussian), Canny, Hough; 

    6.Noise and blur: salt and pepper noise, Gaussian noise, Gaussian blur.




Only the following OpenCV library functions are used in this project:


    IplImage* cvLoadImage(const char* filename, int flags=CV_LOAD_IMAGE_COLOR); 

    IplImage* cvCreateImage(CvSize size, int depth, int channels); 

    void cvResize(const CvArr* src, CvArr* dst, int interpolation=CV_INTER_LINEAR); 

    CvScalar cvGet2D(const CvArr* arr, int idx0, int idx1); 

    void cvSet2D(CvArrarr, int idx0, int idx1, CvScalar value); 

    int cvSaveImage(const char filename, const CvArr* image); 

    int cvNamedWindow(const char* name, int flags=CV_WINDOW_AUTOSIZE); 

    void cvShowImage(const charname, const CvArrimage); 

    int cvWaitKey(int delay=0); 

    void cvDestroyWindow(const char* name); 

    void cvReleaseImage(IplImage* image);




Environment and configuration:


    Windows 8.1 

    Microsoft Visual C++ 6.0 

    OpenCV 1.0




Note that the "lena.jpg" and the "fruits.jpg" used in this project are from the folder “./samples/c/” in the OpenCV installation directory.
