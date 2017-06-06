#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace cv;



string window_name="Calibration";
enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2};

int main()
{
    int image_count=10;
    Mat frame;
    Size image_size;
    Size board_size = Size(8,4);
    vector<Point2f> corners;
    vector<vector<Point2f> > corners_Seq;
    ofstream fout("calibartion_result.txt");
    int mode = DETECTION;

    VideoCapture capture(1);
    capture.set(CV_CAP_PROP_FRAME_WIDTH,640);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    if(!capture.isOpened())
    {
        cout<<"Failed to open the camera!\nExit!"<<endl;
        exit(-1);
    }
    namedWindow(window_name,CV_WINDOW_AUTOSIZE);
    cout<<"Press 's' to start capturing images"<<endl;

    int count=0,n=0;
    stringstream tempname;
    string filename;
    int key;
    string msg;
    int baseLine;
    Size textSize;

    while(n<image_count)
    {
        capture>>frame;
        
        if(mode == DETECTION)
        {
            key = waitKey(30);
            if(key==27)//Esc
                break;
            
            if(capture.isOpened() && key == 's')
                mode = CAPTURING;
        }

        if(mode == CAPTURING)
        {
            key = waitKey(30);
            if(key == 32)//Spacebar
            {
                image_size = frame.size();
                Mat imageGray;
                cvtColor(frame,imageGray,CV_RGB2GRAY);
                bool patternfound = findChessboardCorners(frame,board_size,corners,
                                                CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK
                                                /*CALIB_CB_ADAPTIVE_THRESH+CALIB_CB_NORMALIZE_IMAGE*/);
                for(int i=0;i<corners.size();i++)
                {
                    cout<<corners[i]<<endl;
                }
                if(patternfound)
                {
                    n++;
                    tempname<<n;
                    tempname>>filename;
                    filename+=".jpg";
                    cornerSubPix(imageGray, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
                    count +=corners.size();
                    corners_Seq.push_back(corners);
                    imwrite(filename,frame);
                    tempname.clear();
                    filename.clear();
                }
                else
                {
                    cout<<"Detected Failed!"<<endl;
                }
            }
        }
        msg = mode ==CAPTURING ? "100/100/s" : mode ==CALIBRATED ? "Calibrated" : "Press 's' to start";
        baseLine = 0;
        textSize = getTextSize(msg,1,1,1,&baseLine);
        Point textOrigin(frame.cols-2*textSize.width-10,frame.rows-2*baseLine -10);
        if(mode == CAPTURING)
        {
            msg = format("%d%d",n,image_count);
        }

        putText( frame, msg, textOrigin, 1, 1,mode != CALIBRATED ? Scalar(0,0,255) : Scalar(0,255,0));
        imshow(window_name,frame);
        key = waitKey(1);
        if(key == 27)
            break;
    }

    Size square_size = Size(25,25);
    vector<vector<Point3f> > Object_Points;
    vector<int> point_counts;
    vector<Mat> rotation_vectors;
    vector<Mat> translation_vectors;
    Mat image_points = Mat(1,count,CV_32FC2,Scalar::all(0));
    Mat distortion_coeffs = Mat(1,5,CV_32FC1,Scalar::all(0));
    Mat intrinsic_matrix = Mat(3,3,CV_32FC1,Scalar::all(0));

    for(int t=0;t<image_count;t++)
    {
        vector<Point3f> tempPointSet;
        for (int i=0;i<board_size.height;i++) 
		{   
            for (int j=0;j<board_size.width;j++) 
			{    
				Point3f tempPoint;
				tempPoint.x = i*square_size.width;
				tempPoint.y = j*square_size.height;
				tempPoint.z = 0;
				tempPointSet.push_back(tempPoint);
            }   
        }
        Object_Points.push_back(tempPointSet);
    }
    for(int i=0;i<image_count;i++)
    {
        point_counts.push_back(board_size.width*board_size.height);
    }

    calibrateCamera(Object_Points, corners_Seq, image_size,  intrinsic_matrix  ,distortion_coeffs, rotation_vectors, translation_vectors);  

    cout<<"Start saving parameters..."<<endl;
    fout<<"camera internal parameters: "<<endl;
    fout<<intrinsic_matrix<<"\n"<<endl;
    fout<<"Camera distortion coefficient: "<<endl;
    fout<<distortion_coeffs<<"\n"<<endl;


    return 0;
}