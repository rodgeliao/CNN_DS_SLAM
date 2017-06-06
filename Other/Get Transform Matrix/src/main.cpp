/**
 *Test g2o BA
 *Author: Ruibo Zou
 *Date: 2017.5
 *Email: zouruibo@outlook.com
 *
 */

#include<iostream>
#include<opencv2/opencv.hpp>
#include<boost/concept_check.hpp>

#include<g2o/core/sparse_optimizer.h>
#include<g2o/core/block_solver.h>
#include<g2o/core/robust_kernel.h>
#include<g2o/core/robust_kernel_impl.h>
#include<g2o/core/optimization_algorithm_levenberg.h>
#include<g2o/solvers/cholmod/linear_solver_cholmod.h>
#include<g2o/types/slam3d/se3quat.h>
#include<g2o/types/sba/types_six_dof_expmap.h>

int FindCorrespondingPoints(const cv::Mat& img1,const cv::Mat& img2,std::vector<cv::Point2f>& points1,std::vector<cv::Point2f>& points2);

//camera internal parameters:
float fx=655.97;
float fy=653.73;
float cx=328.32;
float cy=220.50;



int main(int argc, char **argv)
{
    if(argc<3)
    {
        std::cout<<"Please enter two or more images!"<<std::endl;
        exit(1);
    }

    cv::Mat img1 = cv::imread(argv[1]);
    cv::Mat img2 = cv::imread(argv[2]);

    std::vector<cv::Point2f> points1,points2;
    if(FindCorrespondingPoints(img1,img2,points1,points2)==false)
    {
        std::cout<<"Too less match points"<<std::endl;
        return 0;
    }

    std::cout<<"Found "<<points1.size()<<" match points"<<std::endl;

//------------Show match points in two images-----------------
/*
    for(int i=0;i<points1.size();i++)
    {
        cv::circle(img1,points1[i],3,cv::Scalar(0,0,255));
    }

    for(int i=0;i<points2.size();i++)
    {
        cv::circle(img2,points2[i],3,cv::Scalar(0,0,255));
    }
    std::cout<<"Show two image!"<<std::endl;

    cv::imshow("img1",img1);
    cv::imshow("img2",img2);
    cv::waitKey(0);
*/

    //Construct a SparseOptimizer
    g2o::SparseOptimizer optimizer;
    //Use linear solver in Cholmod
    g2o::BlockSolver_6_3::LinearSolverType* linearSolver = new g2o::LinearSolverCholmod<g2o::BlockSolver_6_3::PoseMatrixType>();
    g2o::BlockSolver_6_3* block_solver = new g2o::BlockSolver_6_3(linearSolver);
    //L-M descent
    g2o::OptimizationAlgorithmLevenberg* algorithm = new g2o::OptimizationAlgorithmLevenberg(block_solver);

    optimizer.setAlgorithm(algorithm);
    optimizer.setVerbose(false);

    //add two pose node
    for(int i=0;i<2;i++)
    {
        g2o::VertexSE3Expmap* v = new g2o::VertexSE3Expmap();
        v->setId(i);
        if(i==0)
            v->setFixed(true);//set the first node
        v->setEstimate(g2o::SE3Quat());
        optimizer.addVertex(v);
    }

    //There are too many feature points.So we set the first frame as the base frame
    for(size_t i=0;i<points1.size();i++)
    {
        g2o::VertexSBAPointXYZ* v = new g2o::VertexSBAPointXYZ();
        v->setId(2+i);
        double z=1;
        double x=(points1[i].x-cx)*z/fx;
        double y=(points1[i].y-cy)*z/fy;
        v->setMarginalized(true);
        v->setEstimate(Eigen::Vector3d(x,y,z));
        optimizer.addVertex(v);
    }

    //Prepare the camera parameters
    g2o::CameraParameters* camera = new g2o::CameraParameters(fx,Eigen::Vector2d(cx,cy),0);
    camera->setId(0);
    optimizer.addParameter(camera);

    //Edge
    //First frame
    std::vector<g2o::EdgeProjectXYZ2UV*> edges;
    for(size_t i=0;i<points1.size();i++)
    {
        g2o::EdgeProjectXYZ2UV* edge = new g2o::EdgeProjectXYZ2UV();
        edge->setVertex(0,dynamic_cast<g2o::VertexSBAPointXYZ*>(optimizer.vertex(i+2)));
        edge->setVertex(1,dynamic_cast<g2o::VertexSE3Expmap*>(optimizer.vertex(0)));
        edge->setMeasurement(Eigen::Vector2d(points1[i].x,points1[i].y));
        edge->setInformation(Eigen::Matrix2d::Identity());
        edge->setParameterId(0,0);
        //core function
        edge->setRobustKernel(new g2o::RobustKernelHuber());
        optimizer.addEdge(edge);
        edges.push_back(edge);
    }
    //second frame
    for(size_t i=0;i<points2.size();i++)
    {
        g2o::EdgeProjectXYZ2UV* edge = new g2o::EdgeProjectXYZ2UV();
        edge->setVertex(0,dynamic_cast<g2o::VertexSBAPointXYZ*>(optimizer.vertex(i+2)));
        edge->setVertex(1,dynamic_cast<g2o::VertexSE3Expmap*>(optimizer.vertex(1)));
        edge->setMeasurement(Eigen::Vector2d(points2[i].x,points2[i].y));
        edge->setInformation(Eigen::Matrix2d::Identity());
        edge->setParameterId(0,0);
        //core function
        edge->setRobustKernel(new g2o::RobustKernelHuber());
        optimizer.addEdge(edge);
        edges.push_back(edge);
    }

    std::cout<<"Start optimizing..."<<std::endl;
    optimizer.setVerbose(true);
    optimizer.initializeOptimization();
    optimizer.optimize(10);
    std::cout<<"Finish optimizition"<<std::endl;

    //The transform matrix between two frames
    g2o::VertexSE3Expmap* v = dynamic_cast<g2o::VertexSE3Expmap*>(optimizer.vertex(1));
    Eigen::Isometry3d pose = v->estimate();
    std::cout<<"Pose= \n"<<pose.matrix()<<std::endl;

    return 0;
}


int FindCorrespondingPoints(const cv::Mat& img1,const cv::Mat& img2,std::vector<cv::Point2f>& points1,std::vector<cv::Point2f>& points2)
{
    cv::ORB orb;
    std::vector<cv::KeyPoint> kp1,kp2;
    cv::Mat desp1,desp2;//descriptors
    orb(img1,cv::Mat(),kp1,desp1);
    orb(img2,cv::Mat(),kp2,desp2);
    std::cout<<"Find "<<kp1.size()<<" and "<<kp2.size()<<" feature points"<<std::endl;
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");
    double knn_match_ratio=0.8;
    std::vector<std::vector<cv::DMatch> > matches_knn;
    matcher->knnMatch(desp1,desp2,matches_knn,2);
    std::vector<cv::DMatch> matches;

    for(size_t i=0;i<matches_knn.size();i++)
    {
        if(matches_knn[i][0].distance<knn_match_ratio*matches_knn[i][1].distance)
            matches.push_back(matches_knn[i][0]);
    }

    //the match points are too less
    if(matches.size()<=20)
        return false;

    for(auto m : matches)
    {
        points1.push_back( kp1[m.queryIdx].pt);
        points2.push_back( kp2[m.trainIdx].pt); 
    }
    return true;
}