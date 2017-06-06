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

#include <boost/thread.hpp>
#include<string>
#include<stdio.h>
#include<iostream>

#include"util/DataSetReader.hpp"

std::string calib = "";
std::string vignette = "";
std::string gammaCalib = "";
std::string source = "";

void parseArguments(char* argv)
{
    char buf[256];
    //read source images
    if(1==sscanf(argv,"files=%s",buf))
	{
		source = buf;
		std::cout<<"Loading data from "<<source<<std::endl;
		return;
	}
	else if(0==strcmp(argv,"camera"))
	{
		std::cout<<"Real-Time Camera Mode"<<std::endl;
		return;
	}

    //calibration Parameters
    if(1==sscanf(argv,"calib=%s",buf))
	{
		calib = buf;
		std::cout<<"Loading calibration from "<<calib<<std::endl;
		return;
	}

    //Vignette Parameters
    if(1==sscanf(argv,"vignette=%s",buf))
	{
		vignette = buf;
		std::cout<<"Loading vignette from "<<vignette<<std::endl;
		return;
	}

    //GammaCalib Parameters
    if(1==sscanf(argv,"gamma=%s",buf))
	{
		gammaCalib = buf;
		std::cout<<"Loading gammaCalib from "<<gammaCalib<<std::endl;
		return;
	}

    
}



int main(int argc,char ** argv)
{
    for(int i=1;i<argc;i++)
    {
        parseArguments(argv[i]);
    }

	ImageFolderReader* reader = new ImageFolderReader(source,calib, gammaCalib, vignette);

	boost::thread runThread([&]() {

	}
	);

	runThread.join();

	std::cout<<"\033[32mDone!\033[0m"<<std::endl;
    
}