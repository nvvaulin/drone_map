#pragma once
#include"../../CImageInfo/ImageInfo.h"
#include "../../CImageManager/ImageManager.h"
#include "../../CImageHomography/CImageHomography.h"
#include <vector>
#include <iomanip>   
#include <fstream>



class CTestImageCreator
{
	std::pair<std::vector<cv::Mat>,std::vector<CImageInfo> >  CreateImages(cv::Mat& pano);
	
	void write(const std::pair<std::vector<cv::Mat>,std::vector<CImageInfo> >&  output);

	double drand();
	double interval_x,interval_y;
	double intersect;
	std::vector<CImageInfo> trueInfo;
public:
	std::string fname;
	double d_height,d_gps,d_angle;// disperce  [0;1)
	double delte_height;//max chenging of heihgt [0;1)
	double max_angle;
	double arperture_x;//	arperture_x of camera
	cv::Mat imgHomograpyes;
	cv::Mat get_SubImage(const cv::Mat& pano,int x,int y, double angle,int cols,int rows, double stretch );

	CTestImageCreator():d_height(0.01),d_angle(0.1),d_gps(0.05),delte_height(0),max_angle(1000),arperture_x(0.1){};

	void create(std::string fileName, double _interval_x,double _interval_y,double _intersect)
	{
		interval_x= _interval_x;
		interval_y = _interval_y;
		intersect =_intersect;
		fname  = fileName;
		cv::Mat pano = cv::imread(fname+"/input.jpg");
		pano.copyTo(imgHomograpyes);
		auto res = CreateImages(pano);
		write(res);
	};

};


