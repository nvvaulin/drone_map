#pragma once
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "../CImageHomography/CImageHomography.h"


struct CMatches
{
	std::vector<cv::Point2d> matchedObjectPoints;
	std::vector<cv::Point2d> matchedScenePoints;
	CMatches reverse()
	{
		CMatches res; 
		res.matchedObjectPoints = matchedScenePoints;
		res.matchedScenePoints = matchedObjectPoints;
		return res;
	};
};

class CHomographyFinder
{
	cv::Mat homography;
	std::vector<cv::Point2f> objectPoints;
	std::vector<cv::Point2f> scenePoints;
	std::vector<cv::KeyPoint> objectKeyPoints;
	std::vector<cv::KeyPoint> sceneKeyPoints;
	CMatches matches;
	const double& objInfo;
	int _countOfMatches;
	void ransac();
	void ransacAfine();
	void homographyFind();
public:
	CHomographyFinder(const std::vector<cv::KeyPoint>& objectKeyPoints,const std::vector<cv::KeyPoint>& sceneKeyPoints,const std::vector<cv::DMatch >& matches, const double _objInfo);
	cv::Mat getHomography() const;
	CMatches getMatches() const
	{
		return matches;
	};
	cv::Mat drowHomography(const cv::Mat& img_object,const cv::Mat& img_matches) const;
	int countOfMatches() const{
		return _countOfMatches;
	};
};