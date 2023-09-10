#include <thread>
#include <mutex>
#include <vector>

#include <string>
#include <iostream>
#include <unistd.h>
#include <fstream>
using namespace std;

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
// #include <facedetectcnn.h>
#include "base64.cpp"

//#include <json.h>

// #define ASIO_STANDALONE

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#define MAX_SIZE_BASE64 100000

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::message_ptr message_ptr;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

mutex M;
Mat cvFrame,dst_frame;


void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
    Mat fork;
    fork = dst_frame.clone();
    std::vector<uchar> data(MAX_SIZE_BASE64, 0);

    cv::imencode(".jpg", fork, data);
    std::string img_data = base64_encode(data.data(), data.size(), false);
    //std::cout<<img_data<<std::endl;
    data.clear();
    // ofstream ofresult("1", iostream::out);
    // ofresult << img_data.c_str() << endl;

    s->send(hdl, img_data.c_str(), msg->get_opcode());
    
}

// Record
void th1()
{
    server wsserver;
    cout << "Now Start websocket Server" << endl;

    wsserver.set_access_channels(websocketpp::log::alevel::all);
    wsserver.clear_access_channels(websocketpp::log::alevel::frame_payload);

    wsserver.set_error_channels(websocketpp::log::elevel::all);
    wsserver.init_asio();

    wsserver.set_message_handler(bind(&on_message,&wsserver,::_1,::_2));
   
    wsserver.listen(9000);
    cout << "websocket port 9000" << endl;
    wsserver.start_accept();

    wsserver.run();
}

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
    // String img = "/home/rust/下载/vscode.jpeg";
    // Mat grayimg;
    // Mat srcImage=imread(img); // 读取原始图像
    // Mat resize_img = ReImg(srcImage, 0.5);
    // if (resize_img.empty()){
    //     cout << "None Pic" << endl;
    //     return 0;
    // }else{
    //     imshow("IMG",resize_img);
    // };
    // cvtColor(resize_img, grayimg, COLOR_BGR2GRAY);
    // imshow("Gray", grayimg);

    //  / \               
    //   |                
    //   | This is a image example 

    VideoCapture cap;
    //cap.open(0);
    cap.open("https://vd3.bdstatic.com/mda-kidf9t0h94ga32tu/v1-cae/sc/mda-kidf9t0h94ga32tu.mp4?v_from_s=hkapp-haokan-hnb&auth_key=1692682929-0-0-fb7bc38357c4376537d42d3f399d63ad&bcevod_channel=searchbox_feed&pd=1&cr=2&cd=0&pt=3&logid=2529742350&vid=9815726611466644147&klogid=2529742350&abtest=111803_4-112162_4");
    cout << "Start Main" << endl;
    if (!cap.isOpened())
    {
        cout << "could not open camera" << endl;
        return -1;
    }

    thread child_record(th1);
    while (1)
    {

        cap >> cvFrame;

        Mat resize_frame = ReImg(cvFrame, 0.5);
        flip(resize_frame, dst_frame, 1); //  水平翻转
        if (cvFrame.empty())
        {
            cout << "empty" << endl;
            break;
        }

        M.lock();
        // cout << "this is father" << endl;
        M.unlock();

        //cv::imshow("camera0", dst_frame);

        //waitKey(1);
    }
    
    child_record.join();
    
    cap.release();
    destroyAllWindows();
    return 0;
}
