#include <iostream>
#include "opencv2/core/core.hpp" // Mat class와 각종 data structure 및 산술 루틴을 포함하는 헤더
#include "opencv2/highgui/highgui.hpp" // GUI와 관련된 요소를 포함하는 헤더 (imshow 등)
#include "opencv2/imgproc/imgproc.hpp" // 가종 이미지 처리 함수를 포함하는 헤더
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;

// Pixel Value : 픽셀 값 접근 방법 - 이미지의 각 픽셀을 개별적으로 접근하는 방식 (Opencv 사용하면 안씀)
void SpreadSalts(Mat img, int num){
    // num : 점을 찍을 개수
    for (int n = 0; n < num; n++){
        int x = rand() % img.cols; // img.cols 는 이미지의 폭 정보를 저장
        int y = rand() % img.rows; // img.rows 는 이미지의 높이 정보를 저장
        /*
        나머지는 나누는 수를 넘을 수 없으므로 무작위 위치가
        이미지의 크기를 벗어나지 않도록 제한하는 역할을 해줌
        */

       if (img.channels()==1){ 
        // img.channels() 는 이미지의 채널 수를 반환
            img.at<uchar>(y,x) = 255; // 단일 채널 접근
       }
       else {
            img.at<Vec3b>(y,x)[0] = 255 ; // Blue 채널 접근
            img.at<Vec3b>(y,x)[1] = 255 ; // Green 채널 접근
            img.at<Vec3b>(y,x)[2] = 255 ; // Red 채널 접근

       }
    }
}

// Histogram : 히스토그램 분석 
Mat GetHistogram(Mat&src) {
    Mat histogram;
    const int* channel_numbers = {0};
    float channel_range[] = {0.0, 255.0};
    const float*channel_ranges = channel_range;
    int number_bins = 255;

    // 히스토그램 계산 
    calcHist(&src, 1, channel_numbers, Mat(), histogram, 1, &number_bins, &channel_ranges);

    // 히스토그램 plot
    int hist_w = 512;
    int hist_h = 400;
    int bin_w = cvRound((double)hist_w / number_bins);

    Mat histImage (hist_h, hist_w, CV_8UC1, Scalar(0,0,0));
    normalize(histogram, histogram, 0, histImage.rows, NORM_MINMAX, -1, Mat()); //정규화

    for (int i = 1; i < number_bins; i++){
                line(histImage, Point(bin_w*(i-1), hist_h - cvRound(histogram.at<float>(i-1))),
                Point(bin_w*(i), hist_h - cvRound(histogram.at<float>(i))),
                Scalar(255, 0, 0), 2, 8, 0); // 값과 값을 잇는 선을 그리는 방식으로 plot
        } 
            return histImage;
}

int main(){
    Mat imgA = imread("/home/casey/workspace/크림쑤_공부/DIP/landing.jpg",1); // 이미지 읽기
    Mat imgA2 = imread("/home/casey/workspace/크림쑤_공부/DIP/landing.jpg",0); // 이미지 읽기

    // int flags = IMREAD_COLOR  or int flags = 1 => 컬러 영상으로 읽음
    // int flags = IMREAD_GRAYSCALE  or int flags = 0 => 흑백 영상으로 읽음
    // int flags = IMREAD_UNCHANGED  or int flags = -1 => 원본 영상의 형식대로 읽음
    Mat imgB = imread("/home/casey/workspace/크림쑤_공부/DIP/img4.jpg",1); 
    resize(imgB, imgB, Size(imgA.cols, imgA.rows));  // 두 이미지 사이즈를 통일
    
    Mat dst1, dst2, dst3, dst4;
    add(imgA, imgB, dst1); // dst1 = imgA + imgB; 와 동일함
    add(imgA, Scalar(100,100,100),dst2); // dst2 = imgA + Scalar(100,100,100); 도 동일함
    subtract(imgA, imgB, dst3);
    subtract(imgA, Scalar(100,100,100),dst4);


    Mat black(imgA.size(), CV_8UC3, Scalar(0));  // 동일한 크기의 검정 영상 생성
    Mat mask(imgA.size(), CV_8U, Scalar(0));     // 동일한 크기의 마스크 생성
    circle(mask,Point(500,250),200,Scalar(255),-1,-1); // 마스크에 원 생성 (마지막 뒤에서 2번째 -1 : 채우기 플래그)

    Mat dst5 ;
    // bitwise_and(imgA, imgA, dst5, mask); // 마스크 영역에서 image와 image의 AND 연함
    bitwise_or(black, imgA, dst5, mask); // 마스크 영역에서 검정 영상과 image의 OR 연산

    // SpreadSalts noise & histogram
    Mat spreadsalts = imgA.clone();
    SpreadSalts(spreadsalts, 500);

    Mat histImage = GetHistogram(imgA2); // 히스토그램 생성 (얘는 흑백 이미지를 input으로 받아와야함)
    //이미지 출력
    imshow("Original Image A", imgA);
    imshow("Image B", imgB);
    imshow("Added Image", dst1);
    imshow("Subtracted Image", dst3);
    imshow("Masked Image", dst5);
    imshow("SpreadSalt", spreadsalts); 
    imshow("Histogram", histImage);

    waitKey(0); // 키 입력 대기 (0: 키가 산력될 때 까지 프로그램 멈춤춤
    destroyWindow("Test window"); // 이미지 출력창 종료

    // imwrite("landing_gray.png", imgA); // 변환된 흑백 이미지 저장

    
    return 0;
}