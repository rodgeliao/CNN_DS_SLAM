#include<opencv2/opencv.hpp>
#include<iostream>
#include<cmath>

bool Iscollinear(cv::Point2f point1,cv::Point2f point2,cv::Point2f point3);


int main()
{
    cv::Point2f point1,point2,point3;
    std::cout<<"Enter 3 points : "<<std::endl;
    std::cout<<"Point1 : ";
    std::cin>>point1.x>>point1.y;
    std::cout<<std::endl;
    std::cout<<"Point2 : ";
    std::cin>>point2.x>>point2.y;
    std::cout<<std::endl;
    std::cout<<"Point3 : ";
    std::cin>>point3.x>>point3.y;
    bool iscollinear=Iscollinear(point1,point2,point3);
    if(iscollinear)
    {
        std::cout<<"The three points "<<point1<<"  "<<point2<<"  "<<point3<<" are collinear!"<<std::endl;
    }
    else
    {
        std::cout<<"The three points "<<point1<<"  "<<point2<<"  "<<point3<<" are not collinear!"<<std::endl;
    }
}

bool Iscollinear(cv::Point2f point1,cv::Point2f point2,cv::Point2f point3)
{
    std::cout<<point1.x*point2.y<<std::endl;
    std::cout<<point2.x*point3.y<<std::endl;
    std::cout<<point3.x*point1.y<<std::endl;
    std::cout<<-point1.x*point3.y<<std::endl;
    std::cout<<-point2.x*point1.y<<std::endl;
    std::cout<<-point3.x*point2.y<<std::endl;
    std::cout<<fabs(point1.x*point2.y
                  +point2.x*point3.y
                  +point3.x*point1.y
                  -point1.x*point3.y
                  -point2.x*point1.y
                  -point3.x*point2.y)<<std::endl;
    if(fabs(point1.x*point2.y
          +point2.x*point3.y
          +point3.x*point1.y
          -point1.x*point3.y
          -point2.x*point1.y
          -point3.x*point2.y)<1e-2)
        return true;
    else
    {
        return false;    
    }
}