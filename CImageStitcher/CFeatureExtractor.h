#pragma once
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"

class CFeatureExtractor
{
	const static int minHessian = 200;
	cv::SurfFeatureDetector detector;
	cv::SurfDescriptorExtractor extractor;
	std::vector<cv::KeyPoint> keyPoints;
	cv::Mat descriptors;
	static const int maxKeyPoints = 700;
	void findKeyPoints();
	void extractDescriptors();
	const cv::Mat* image;
	void extractFeature();
public:
	CFeatureExtractor::CFeatureExtractor(const cv::Mat* _image) : detector( minHessian ), image(_image) 
	{
		extractFeature();
	};
	
	std::vector<cv::KeyPoint> getKeyPoints() const;
	cv::Mat getDescriptors()const;
	cv::Mat drowFeature()const;
};