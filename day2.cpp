#include <iostream>
#include "opencv2/core/core.hpp" // Mat class와 각종 data structure 및 산술 루틴을 포함하는 헤더
#include "opencv2/highgui/highgui.hpp" // GUI와 관련된 요소를 포함하는 헤더 (imshow 등)
#include "opencv2/imgproc/imgproc.hpp" // 가종 이미지 처리 함수를 포함하는 헤더
#include <opencv2/imgcodecs.hpp>

using namespace cv;

// Pixel Access : Mat 객체 data 기반 화소 접근법
Mat myCopy(Mat srcImg){
    int width = srcImg.cols;
    int height = srcImg.rows;
    Mat dstImg(srcImg.size(), CV_8UC1); // 입력영상과 동일한 크기의 Mat 생성
    uchar* srcData = srcImg.data; // Mat 객체의 data를 가리키는 포인터
    uchar* dstData = dstImg.data; 

    for (int y=0; y< height; y++){
        for (int x=0; x < width; x++){
            dstData[ y * width + x ] = srcData[ y * width + x ];
            // 화소 값을 일일이 읽어와 다른 배열에 저장
        }
    }

    return dstImg;

}

int myKernelConv3x3(uchar* arr, int kernel[][3], int x, int y, int width, int height){
    int sum = 0;
    int sumKernel = 0;

    // 특정 화소의 모든 이웃화소에 대해 계산하도록 반복문 구성
    for (int j = -1; j <= 1; j++){
        for (int i = -1; i <= i; i++){
            if ((y + j) >= 0 && (y + j) < height && (x + i) >=0 && (x + i) < width){
                // 영상 가장자리에서 영상 밖의 화소를 읽지 않도록 하는 조건문
                sum += arr[(y + j) * width + (x + i)] * kernel[i + 1][j + 1];
                sumKernel += kernel[i + 1][j+1];
            }
        }
    }

    if (sumKernel != 0) {
        return sum / sumKernel; // 합이 1로 정규화되도록 해 영상의 밝기 변화 방지
    }
    else { return sum; }
}


Mat myGaussianFilter (Mat srcImg){
    int width = srcImg.cols;
    int height = srcImg.rows;
    int kernel[3][3] = { 1, 2, 1,
                         2, 4, 2,
                         1, 2, 1}; // 3x3 형태의 Gaussian 마스크 배열
    Mat dstImg(srcImg.size(), CV_8UC1);
    uchar* srcData = srcImg.data;
    uchar* dstData = dstImg.data;

    for (int y = 0; y < height; y++){
        for (int x =0; x < width; x++){
            dstData[y * width + x] = myKernelConv3x3(srcData, kernel, x, y, width, height);
            // 앞서 구현한 convolution에 마스크 배열을 입력해 사용
        }
    }
    return dstImg;
}

Mat mySobelFilter(Mat srcImg){
    int kernel1X[3][3] = {  -1, 0, 1,
                            -2, 0, 2,
                            -1, 0, 1 };     // 가로 방향 Sobel 마스크
    int kernelY[3][3] = {   -1, -2, -1,
                             0, 0, 0,
                             1, 2, 1 };     // 세로 방향 Sobel 마스크
    // 마스크 합이 0이  되므로 1로 정규화하는 과정은 필요 없음
    Mat dstImg(srcImg.size(), CV_8UC1);
    uchar* srcData = srcImg.data;
    uchar* dstData = dstImg.data;
    int width = srcImg.cols;
    int height = srcImg.rows;

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            dstData[y * width + x] = (abs(myKernelConv3x3(srcData, kernel1X, x, y, width, heigth)) +
                                      abs(myKernelConv3x3(srcData, kernelY, x, y, width, height)))/2;
            // 두 에지 결과의 절대값 합 형태로 최종결과 도출
        }
    }
    return dstImg;
}

Mat mySampling(Mat srcImg){
    int width = srcImg.cols / 2;
    int height = srcImg.rows / 2;
    Mat dstImg(height, width, CV_8UC1);
    // 가로, 세로가 입력 영상의 절반인 영상을 먼저 생성
    uchar* srcData = srcImg.data;
    uchar* dstData = dstImg.data;

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x ++){
            dstData[y * width + x] = srcData[(y * 2) * (width * 2) + (x *2)];
            // 2배 간격으로 인덱싱 해 큰 영상을 작은 영상에 대입할 수 있음
        }
    }

    return dstImg;

}

