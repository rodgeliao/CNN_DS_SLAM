/**
* This file is part of CNN_DS_SLAM.
* 
* Copyright 2017 Shantou University.
* Developed by Ruibo Zou. 
* For more information see <https://github.com/RippleZou/CNN_DS_SLAM>.
* If you use this code, please cite the respective publications as
* listed on the above website.
*
* CNN_DS_SLAM is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* CNN_DS_SLAM is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*/

#pragma once

#include <Eigen/Core>
#include "sophus/sim3.hpp"
#include "sophus/se3.hpp"

namespace dldso
{

typedef Sophus::SE3d SE3;
typedef Sophus::Sim3d Sim3d;
typedef Sophus::SO3d SO3;

//Matrix for Rotation Matrix & Translation Matrix & Transform Matrix
typedef Eigen::Matrix<double,3,3> Rotation_3_3;
typedef Eigen::Matrix<double,3,1> Translation_3_1;
typedef Eigen::Matrix<double,4,4> Transform_4_4;

//Points in 2d & 3d
typedef Eigen::Matrix<float,2,1> Point_2f;
typedef Eigen::Matrix<float,3,1> Point_3f;



}