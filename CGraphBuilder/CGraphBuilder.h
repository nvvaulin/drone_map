#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <opencv2/core/core.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../CImage/Image.h"
#include "../CImageInfo/ImageInfo.h"
#include "../CImageManager/ImageManager.h"
#include "../CImageHomography/CImageHomography.h"
#include "../CImageStitcher/CFeatureExtractor.h"
#include "../CImageStitcher/CFeatureMatcher.h"
#include "../CImageStitcher/CHomographyFinder.h"



class CGraphBuilder
{
	struct Vertex
	{
		std::vector<int> neighbors;
		CMatrix cameraPosition;//position of in global basis
		CImageInfo info;
	};


	typedef std::pair<int,int> PairID;

	struct EqualUnorderedPairsID{
		bool operator()(const std::pair<int,int>& x,const std::pair<int,int>& y)const{
			return ((x.first == y.first)&&(x.second == y.second)) || ((x.first == y.second)&&(x.second == y.first));
		};
	};

	struct hashPairID{
		size_t operator()(const std::pair<int,int>& x)const{
			std::hash<__int64> h;
			__int64 a  =x.first;
			__int64 b = x.second;
			return  h(((a<<32)&(~(0xFFFFFFFF)))|(b));
		};
	};

	CImageManager& imageManeger;
	std::unordered_map<int,Vertex> _V;
	std::unordered_map<PairID,CMatches,hashPairID> _E;//pair(im1,im2), homography im2 to basis of im1
	std::vector<int> FindNeighbors(int id);
	bool isImageIntersects(int id1,int id2);
	static bool isPointsOnDiffenertSideOfSeq(const cv::Point2d& p1,const cv::Point2d& p2,const cv::Point2d& s1,const cv::Point2d& s2);
	static bool isPointBetweenSeq(const cv::Point2d& p,const cv::Point2d& s1,const cv::Point2d& s2);
	static bool isSeqIntersects(const cv::Point2d& a1,const cv::Point2d& a2,const cv::Point2d& b1,const cv::Point2d&);
	void findVertexes();//get all image Ids from  CImageManager
	void findCameraPositions();//find Vertexes Homographies using Edges Homographies
	void findEdges();//find all neighbors using coordinates and sizes from imageInfo
	void findEdgesHomographies();//find Edges Homographies using Key Points
	cv::Size imageSize;
	double AreaInOnePixelPetHeihgt;//area(in gps metrix) in one pxel at height = 1
public:
	/******For debug*******/
	cv::Mat pano;
	void initPano();//do it befor all debug methods
	void drowEdges();
	void drowCameraHomographyes();
	void DrowAll();
	void show()
	{
		cv::imwrite(floader + "\\pano.jpg", pano );
	}
	static std::string floader;
	/**********************/
		
	CGraphBuilder(CImageManager& _imageManeger,double arperture_x): imageManeger(_imageManeger),AreaInOnePixelPetHeihgt(1){
		findVertexes();
		cv::Mat im = imageManeger.getImage(_V.begin()->first).getMat();
		imageSize = cv::Size(im.cols,im.rows);
		/*for debug***/
		initPano();
		//drowPano();
		//show();
		/**********/
		findEdges();
		/******/
		drowEdges();
		drowCameraHomographyes();
		show();
		/*******/
		findEdgesHomographies();
		show();
		findCameraPositions();
	};

	Vertex& V(int id){
		return _V[id];
	};

};