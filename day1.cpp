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

// Histogram : 히스토그램 분석 - 


int main(){
    Mat src_img = imread("/home/casey/workspace/크림쑤_공부/DIP/landing.jpg",1); // 이미지 읽기
    // int flags = IMREAD_COLOR  or int flags = 1 => 컬러 영상으로 읽음
    // int flags = IMREAD_GRAYSCALE  or int flags = 0 => 흑백 영상으로 읽음
    // int flags = IMREAD_UNCHANGED  or int flags = -1 => 원본 영상의 형식대로 읽음

    imshow("Test window", src_img); //이미지 출력
    waitKey(0); // 키 입력 대기 (0: 키가 입력될 때 까지 프로그램 멈춤춤
    destroyWindow("Test window"); // 이미지 출력창 종료

    imwrite("landing_gray.png",src_img); //이미지 쓰기
    
    return 0; 
 
}