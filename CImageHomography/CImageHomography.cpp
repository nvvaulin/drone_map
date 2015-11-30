#include "CImageHomography.h"


bool CImageHomography::isValidPersective(const cv::Mat& homography, const double angle)
{
	
	/*if(abs(homography.at<double>(2,0)) > 0.005)//so soft
		return false;
	if(abs(homography.at<double>(2,1)) > 0.005)//so soft
		return false;

	CMatrix m(homography);
	double det = m.det();
	double pi = CV_PI;
	double d_andle = m.getAngle() - angle;
	if ( abs(d_andle  -  floor(d_andle/(2*pi))*2*pi)  > 0.1 )
		return false;

	auto eigenValues  = m.eigenValues();		
	if( abs(eigenValues.first - 1) > 0.3 )
		return false;
	if( abs(eigenValues.second - 1) > 0.3 )
		return false;
	if( (eigenValues.first*eigenValues.second) <= 0.0 )
		return false;*/

	return true;
};

bool CImageHomography::isValid()
{
	return is_valid;
};

cv::Mat  CImageHomography::invertTransform(const cv::Mat& H)
{
	std::vector<cv::Point2f> src;
	src.push_back(cv::Point2f(1,1));
	src.push_back(cv::Point2f(0,1));
	src.push_back(cv::Point2f(1,0));
	src.push_back(cv::Point2f(0,0));
	std::vector<cv::Point2f> dst;
	cv::perspectiveTransform(src,dst,H);
	return cv::getPerspectiveTransform(dst,src);
};


CImageHomography::CImageHomography(void): Matrix(3,3),is_valid(false)
{
};

void CImageHomography::show(cv::Mat& res,const cv::Mat& img_object,cv::Mat h, cv::Scalar color )
{
	std::vector<cv::Point2f> obj_corners(4);
	std::vector<cv::Point2f> scene_corners(4);

	obj_corners[0] = cvPoint(0,0); 
	obj_corners[1] = cvPoint( img_object.cols, 0 );
	obj_corners[2] = cvPoint( img_object.cols, img_object.rows ); 
	obj_corners[3] = cvPoint( 0, img_object.rows );

	cv::perspectiveTransform( obj_corners, scene_corners, h);

	cv::line( res, scene_corners[0] , scene_corners[1], color, 4 );
	cv::line( res, scene_corners[1], scene_corners[2] , color, 4 );
	cv::line( res, scene_corners[2], scene_corners[3], color, 4 );
	cv::line( res, scene_corners[3] , scene_corners[0] , color, 4 );

};