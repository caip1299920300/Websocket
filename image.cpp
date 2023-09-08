#include <iostream>
#include <vector>
using namespace std;

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "base64.cpp"
using namespace cv;

#define MAX_SIZE_BASE64 100000

Mat ReImg(Mat img, double rate)
{
    Mat resize_img;

    int src_width = img.size().width; // this.srcImage.width
    int src_hei = img.size().height;  // this.srcImage.height

    Size re = Size(src_width * rate, src_hei * rate);

    resize(img, resize_img, re, 0, 0, INTER_NEAREST);
    return resize_img;
}

int main()
{
    String img = "/home/rust/图片/vscode.jpeg";
    Mat grayimg;
    Mat srcImage=imread(img); // 读取原始图像
    Mat resize_img = ReImg(srcImage, 0.25);

    if (resize_img.empty()){
        cout << "None Pic" << endl;
        return 0;
    }else{
        imshow("IMG",resize_img);

    std::vector<uchar> data(MAX_SIZE_BASE64, 0);

    cv::imencode(".jpg", resize_img, data);
    std::string img_data = base64_encode(data.data(), data.size(), false);
    printf("%s\n", img_data.c_str());
    };
    cvtColor(resize_img, grayimg, COLOR_BGR2GRAY);
    imshow("Gray", grayimg);
    waitKey(0);
}