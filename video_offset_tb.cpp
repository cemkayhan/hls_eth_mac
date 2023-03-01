#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

#include "video_offset.h"

int main()
{
    std::string image_path = "/home/ckayhan/video_offset/Lenna_In.png";
    static cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);

#if 1
    if(img.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }

    static cv::Mat imgScaled;
    cv::resize(img,imgScaled,cv::Size(1920,1080),cv::INTER_LINEAR);

    static hls::stream<ap_axiu<24,1,1,1>> imgStr("imgStr");
    for (auto i=0; i<1080; ++i) {
    	for (auto j=0; j<1920; ++j) {
    		ap_axiu<24,1,1,1> tmp;
    		if (j==1919) {
    			tmp.last=1;
    		} else {
    			tmp.last=0;
    		}
    		if (i==0 && j==0) {
    			tmp.user=1;
    		} else {
    			tmp.user=0;
    		}
    		cv::Vec3b pix = imgScaled.at<cv::Vec3b>(i,j);
    		tmp.data(7,0) = pix[0];
    		tmp.data(15,8) = pix[1];
    		tmp.data(23,16) = pix[2];
    		imgStr.write(tmp);
    	}
    }

    static hls::stream<ap_axiu<24,1,1,1>> imgStr2("imgStr2");
    for (auto i=0; i<1080/2; ++i) {
    	video_offset(imgStr,imgStr2,400);
    }

    static cv::Mat imgScaledOut(1080,1920,CV_8UC3);
    for (auto i=0; i<1080; ++i) {
    	for (auto j=0; j<1920; ++j) {
    		ap_axiu<24,1,1,1> tmp = imgStr2.read();
    		cv::Vec3b pix;
    		pix[0] = tmp.data(7,0);
    		pix[1] = tmp.data(15,8);
    		pix[2] = tmp.data(23,16);
    		imgScaledOut.at<cv::Vec3b>(i,j) = pix;
    	}
    }

    cv::imshow("Display window", imgScaledOut);
    cv::waitKey(0); // Wait for a keystroke in the window
#endif

    return 0;
}
