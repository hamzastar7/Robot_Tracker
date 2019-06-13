#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;
 
int main()
{
    VideoCapture capture(0);
    if(!capture.isOpened()){
            cout << "Failed to connect to the camera." << endl;
    }
    Mat frame;
    capture >> frame;
    if(frame.empty()){
                cout << "Failed to capture an image" << endl;
                return -1;
    }
    imwrite("test.jpg", frame);
    //cout << frame[0][0][0] << endl;
    return 0;
}