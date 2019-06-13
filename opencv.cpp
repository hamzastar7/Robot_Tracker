#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int i=0;
//Mat image;
int main(){
    
    VideoCapture capture(0);
    capture.set(CV_CAP_PROP_FRAME_WIDTH,360);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,240);
    if(!capture.isOpened()){
        cout << "Failed to connect to the camera." << endl;
    }
    Mat frame, edges,image,hue_img;
    
    while(1){

        
        capture >> frame;
        hue_img << cv.CreateImage(GetSize(frame), 8, 3);
        if(frame.empty()){
            cout << "Failed to capture an image" << endl;
            return -1;
            exit(-1);
        }
        if ( !frame.data )
        {
        printf("No image data \n");
        return -1;
        }


        cvtColor(frame, edges, CV_BGR2GRAY);
        Canny(edges, edges, 0, 30, 3);
        
        namedWindow("Display Image", WINDOW_AUTOSIZE );
        imshow("Display Image", frame);
        
        
        i++;
        
        cout << "frame..." << i << endl;
        
        
        if(waitKey(22) >= 97) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
        {
                cout << "esc key is pressed by user" << endl; 
                break;
        }
        
    }
    //destroyAllWindows();    

    return 0;
}