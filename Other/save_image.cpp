#include<iostream>
#include<opencv2/opencv.hpp>
#include<ctime>

using namespace std;
using namespace cv;

int main()
{
    Mat frame;
    VideoCapture capture(1);
    capture.set(CV_CAP_PROP_FRAME_WIDTH,640);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    while(1)
    {
        capture>>frame;
        imshow("save_image",frame);
        int key = waitKey(30);
        if(key==27)
            break;
        if(key==32)
        {
            string stime;
            time_t t;
            time(&t);
            cout<<ctime(&t)<<endl;
            stime = ctime(&t);
            cout<<stime<<endl;
            stime = stime +".jpg";
            imwrite(stime,frame);
        }

    }
    return 0;
}