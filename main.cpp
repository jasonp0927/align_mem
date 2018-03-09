#include <stdio.h>
#include <stdlib.h>

//#include <opencv2/opencv.hpp>
//using namespace cv;

extern "C" void neonMemCopy(unsigned char *dst,unsigned char *src,int num_bytes);

unsigned char *alignPtr(unsigned char* ptr,int n)
{
    return (unsigned char*)(((unsigned int)ptr + n-1) & -n);
}

void *prk_align_malloc(int size,int align_bytes)
{
        unsigned char* udata = (unsigned char*)malloc(size + sizeof(void*) + align_bytes);
        if(!udata)
        {
                printf("prk_malloc failed\n\r");
                return NULL;
        }
        printf("Prk_malloc Before  = %p\n",udata);
        unsigned char** adata = (unsigned char**)alignPtr((unsigned char*)udata + 1, align_bytes);
        adata[-1] = udata;
        printf("Prk_malloc after  = %p %p\n",adata,adata[-1]);
        return adata;
}

void prk_align_free(void *buf)
{
    if(buf)
    {
        printf("Prk_free bf  = %p\n",buf);
        unsigned char* udata = ((unsigned char**)buf)[-1];
        printf("Prk_free after  = %p\n",udata);
        free(udata);
    }
}

int main()
{
        unsigned char *psrc = NULL;
		unsigned char *pdst = NULL;
		
//		Mat bgr = imread("test.jpg");
		
		psrc = prk_align_malloc(bgr.rows*bgr.cols*bgr.channels(),16);
		pdst = prk_align_malloc(bgr.rows*bgr.cols*bgr.channels(),16);
		
		memcpy(psrc,bgr.data,bgr.rows*bgr.cols*bgr.channels());

		neonMemCopy(pdst,psrc,bgr.rows*bgr.cols*bgr.channels());
		
//		Mat result = Mat(bgr.rows,bgr.cols,CV_8UC3,pdst);
		
//		imwrite("result.jpg",result);
		
        prk_align_free(psrc);
		prk_align_free(pdst);
		
        return 0;
}
