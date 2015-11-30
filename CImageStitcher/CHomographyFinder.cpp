#include "CHomographyFinder.h"
#include <stdio.h>
#include <time.h>
#include <math.h>
#ifdef ADD_NOISE_H
double __drand()
{
	return 0.5 - (double(rand() % 100000))/100000.0;
};

void getNiose(cv::Mat H,cv::Point2f src[4])
{
	double c_pos = 0.01,c_angle = 0.01;
	CMatrix m = CMatrix::move(-H.at<double>(2,0)*(__drand()*c_pos),-H.at<double>(2,1)*(__drand()*c_pos))*CMatrix::rotate(__drand()*c_angle);
	for(int i = 0;i<4;++i)
		src[i] =m.transform(src[i]);
}
#endif


CHomographyFinder::CHomographyFinder(const std::vector<cv::KeyPoint>& _objectKeyPoints,const std::vector<cv::KeyPoint>& _sceneKeyPoints,const std::vector<cv::DMatch >& matches, const double _objInfo): objInfo(_objInfo), sceneKeyPoints(_sceneKeyPoints),objectKeyPoints(_objectKeyPoints),_countOfMatches(0)
{
	for( size_t i = 0; i < matches.size(); i++ )
	{
		objectPoints.push_back( objectKeyPoints[ matches[i].queryIdx ].pt );
		scenePoints.push_back( sceneKeyPoints[ matches[i].trainIdx ].pt );
	}
	homographyFind();
};

void CHomographyFinder::homographyFind()
{
	//ransacAfine();
	ransac();
};

cv::Mat CHomographyFinder::getHomography() const
{
	return homography;
};

cv::Mat CHomographyFinder::drowHomography(const cv::Mat& img_object,const cv::Mat& img_matches) const
{
	cv::Mat res = img_matches;
	std::vector<cv::Point2f> obj_corners(4);
	std::vector<cv::Point2f> scene_corners(4);

	obj_corners[0] = cvPoint(0,0); 
	obj_corners[1] = cvPoint( img_object.cols, 0 );
	obj_corners[2] = cvPoint( img_object.cols, img_object.rows ); 
	obj_corners[3] = cvPoint( 0, img_object.rows );
	cv::perspectiveTransform(obj_corners,scene_corners,homography);
	cv::line( res, scene_corners[0] + cv::Point2f( (float)img_object.cols, 0.0), scene_corners[1] + cv::Point2f( (float)img_object.cols, 0.0), cv::Scalar(0, 255, 0), 4 );
	cv::line( res, scene_corners[1] + cv::Point2f( (float)img_object.cols, 0.0), scene_corners[2] + cv::Point2f( (float)img_object.cols, 0.0), cv::Scalar( 0, 255, 0), 4 );
	cv::line( res, scene_corners[2] + cv::Point2f((float) img_object.cols, 0.0), scene_corners[3] + cv::Point2f( (float)img_object.cols, 0.0), cv::Scalar( 0, 255, 0), 4 );
	cv::line( res, scene_corners[3] + cv::Point2f((float) img_object.cols, 0.0), scene_corners[0] + cv::Point2f( (float)img_object.cols, 0.0), cv::Scalar( 0, 255, 0), 4 );
	
	return res;
};


void CHomographyFinder::ransac()
{
	const int maxIter = 5000;
	const int maxDistance = 5;
	int count = objectPoints.size();
	std::vector<cv::Point2f> _objectPoints(count);
	cv::Mat H;
	srand((unsigned int)time(0));
	for(int i = 0; i< maxIter;++i)
	{
		int goodMatches = 0;
		cv::Point2f src[4];
		cv::Point2f dst[4];
		int p[4];
		for(int j = 0; j<4;++j)
		{
			do
				p[j] = ((int)rand())%count;
			while( ((p[0]==p[j])&&(0<j)) || ((p[1]==p[j])&&(1<j)) || ((p[2]==p[j])&&(2<j)) );
			src[j] = objectPoints[p[j]];	
			dst[j] = scenePoints[p[j]];
		};
		H = cv::getPerspectiveTransform(src,dst);
		if (!CImageHomography::isValidPersective(H,objInfo))
			continue;
		cv::perspectiveTransform(objectPoints,_objectPoints,H);
		for(int j = 0;j<count;++j)
			if ( abs(_objectPoints[j].x-scenePoints[j].x)+abs(_objectPoints[j].y-scenePoints[j].y) < maxDistance)
			{
				++goodMatches;
			};
		if (goodMatches> _countOfMatches)
		{
#ifdef ADD_NOISE_H
			getNiose(H,src);
			H = cv::getPerspectiveTransform(src,dst);
#endif
			homography = H;
			_countOfMatches = goodMatches;
			
		};
		if( ((_countOfMatches > count*0.4) && (count > 50)) || (_countOfMatches > count*0.7)&&(_countOfMatches>6) )
			break;
	}
	if(_countOfMatches >= 4)
	{
		cv::perspectiveTransform(objectPoints,_objectPoints,homography);
		for(int j = 0;j<count;++j)
			if ( abs(_objectPoints[j].x-scenePoints[j].x)+abs(_objectPoints[j].y-scenePoints[j].y) < maxDistance)
			{
				matches.matchedObjectPoints.push_back(Point2d(objectPoints[j].x,objectPoints[j].y));
				matches.matchedScenePoints.push_back(Point2d(scenePoints[j].x,scenePoints[j].y));
			};
	}

};



void CHomographyFinder::ransacAfine()
{
	const int maxIter = 5000;
	const int maxDistance = 5;
	int count = objectPoints.size();
	std::vector<cv::Point2f> _objectPoints(count);
	cv::Mat H;
	srand((unsigned int)time(0));
	for(int i = 0; i< maxIter;++i)
	{
		int goodMatches = 0;
		cv::Point2f src[3];
		cv::Point2f dst[3];
		int p[4];
		for(int j = 0; j<3;++j)
		{
			do
				p[j] = ((int)rand())%count;
			while( ((p[0]==p[j])&&(0<j)) || ((p[1]==p[j])&&(1<j)) );
			src[j] = objectPoints[p[j]];	
			dst[j] = scenePoints[p[j]];
		};
		H = cv::getAffineTransform(src,dst);
		H = CMatrix().set3x2(H).matrix;///!! it is not nessescery
		if (!CImageHomography::isValidPersective(H,objInfo))
			continue;
		cv::perspectiveTransform(objectPoints,_objectPoints,H);
		for(int j = 0;j<count;++j)
			if ( abs(_objectPoints[j].x-scenePoints[j].x)+abs(_objectPoints[j].y-scenePoints[j].y) < maxDistance)
				++goodMatches;
		if (goodMatches> _countOfMatches)
		{
#ifdef ADD_NOISE_H
			getNiose(H,src);
			H = cv::getPerspectiveTransform(src,dst);
#endif
			homography = H;
			_countOfMatches = goodMatches;
			
		};
		if( ((_countOfMatches > count*0.4) && (count > 50)) || (_countOfMatches > count*0.7)&&(_countOfMatches>6) )
			break;
	}

};