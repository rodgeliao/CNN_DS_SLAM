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

#include<fstream>
#include<iostream>
#include<string>
#include<algorithm>
#include<dirent.h>
#include<vector>

inline int getdir (std::string dir, std::vector<std::string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL)
    {
        return -1;
    }

    while ((dirp = readdir(dp)) != NULL) {
    	std::string name = std::string(dirp->d_name);

    	if(name != "." && name != "..")
    		files.push_back(name);
    }
    closedir(dp);


    std::sort(files.begin(), files.end());

    if(dir.at( dir.length() - 1 ) != '/') dir = dir+"/";
	for(unsigned int i=0;i<files.size();i++)
	{
		if(files[i].at(0) != '/')
			files[i] = dir + files[i];
	}

    return files.size();
}


class ImageFolderReader
{
public:
    ImageFolderReader(std::string image_path,std::string calibfile,std::string gammafile,std::string vignettefile)
    {
        this->image_path=image_path;
        this->calibfile=calibfile;
        this->gammafile=gammafile;
        this->vignettefile=vignettefile;
        num_images=getdir(image_path,files);
        std::cout<<"The number of images : "<<num_images<<std::endl;
    }

private:
    int num_images;
    std::string image_path;
    std::string calibfile;
    std::string gammafile;
    std::string vignettefile;

    std::vector<std::string> files;
};