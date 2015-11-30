#pragma once
#include "../CImageInfo/ImageInfo.h"
#include "../CImage/Image.h"
#include "CMatrix.h"
#include <opencv2/core/core.hpp>





class CImageHomography
{
	bool is_valid;
	void validate();
#ifdef _DEBUG
	static void validateClass();    
#endif
public:	
	vector<cv::Point> objectPoints;	
	vector<cv::Point> scenePoints;	
	CMatrix Matrix;

	static cv::Mat invertTransform(const cv::Mat&);

	bool isValid();
	bool static isValidPersective(const cv::Mat& homography,  const double angle);

	CImage perspectiveTransform(const CImage&);
	static void show(cv::Mat& pano,const cv::Mat& img_object,cv::Mat h, cv::Scalar color );
	CImageHomography(void);
};

