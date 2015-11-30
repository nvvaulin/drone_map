#include "CFeatureExtractor.h"

void CFeatureExtractor::extractFeature() 
{
	findKeyPoints();
	extractDescriptors();
};

std::vector<cv::KeyPoint> CFeatureExtractor::getKeyPoints() const
{
	return keyPoints;
};


cv::Mat CFeatureExtractor::getDescriptors() const 
{
	return descriptors;
};


cv::Mat CFeatureExtractor::drowFeature() const 
{
	cv::Mat res;
	cv::drawKeypoints(*image, keyPoints, res);
	return res;
};


void CFeatureExtractor::findKeyPoints()
{
	cv::KeyPointsFilter filter;
	detector.detect(*image,keyPoints);
	filter.removeDuplicated(keyPoints);
	if(keyPoints.size() > maxKeyPoints)
		keyPoints.resize(maxKeyPoints);
};

void 	CFeatureExtractor::extractDescriptors()
{
	extractor.compute(*image,keyPoints,descriptors);
};