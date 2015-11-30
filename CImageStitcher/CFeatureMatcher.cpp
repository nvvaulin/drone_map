#include "CFeatureMatcher.h"


void CFeatureMatcher::match()
{
	matches.clear();
	matcher.match( descriptors_object, desctiptors_scene, matches );
	cv::KeyPointsFilter f;
	double max_dist = 0; 
	double min_dist = 100;
	for( int i = 0; i < descriptors_object.rows; i++ )
	{ 
		double dist = matches[i].distance;
		if( dist < min_dist ) min_dist = dist;
		if( dist > max_dist ) max_dist = dist;
	}

	std::vector< cv::DMatch > good_matches;

	for( int i = 0; i < descriptors_object.rows; i++ )
	{ 
		if( (matches[i].distance < 2*min_dist) || (good_matches.size() < 4) )
		{ 
			good_matches.push_back( matches[i]); 
		}
	}
	if(good_matches.size() > maxMatches)
		good_matches.resize(maxMatches);

	matches = good_matches;
};

std::vector<cv::DMatch > CFeatureMatcher::getMatches() const 
{
	return matches;
};


cv::Mat CFeatureMatcher::drowMatches(const cv::Mat& img_object,std::vector<cv::KeyPoint>& keypoints_object,const cv::Mat& img_scene,std::vector<cv::KeyPoint>& keypoints_scene)const 
{
	cv::Mat img_matches;
	drawMatches( img_object, keypoints_object, img_scene, keypoints_scene,
                 matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
               std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );	
	return img_matches;
};