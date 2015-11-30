#pragma once
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"

class CFeatureMatcher
{
	cv::FlannBasedMatcher matcher;
	std::vector<cv::DMatch > matches;
	cv::Mat descriptors_object;
	cv::Mat desctiptors_scene;
	static const int maxMatches = 500;
	void match();
public:
	CFeatureMatcher(const cv::Mat& _descriptors_object,const cv::Mat& _desctiptors_scene):desctiptors_scene(_desctiptors_scene),descriptors_object(_descriptors_object)
	{
		match();
	};
	std::vector<cv::DMatch > getMatches() const ;
	cv::Mat drowMatches(const cv::Mat& img_object,std::vector<cv::KeyPoint>& keypoints_object,const cv::Mat& img_scene,std::vector<cv::KeyPoint>& keypoints_scene) const;
};