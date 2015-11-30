#include "CGraphBuilder.h"


std::string CGraphBuilder::floader = "1";


double vec2Mul(cv::Point2d p1,cv::Point2d p2)
{
	return p1.x*p2.y-p1.y*p2.x;
};

bool CGraphBuilder::isPointsOnDiffenertSideOfSeq(const cv::Point2d& p1,const cv::Point2d& p2,const cv::Point2d& s1,const cv::Point2d& s2)
{
	return (vec2Mul(s1-p1,s1-s2)*vec2Mul(s1-p2,s1-s2)) >= 0;
};

bool CGraphBuilder::isPointBetweenSeq(const cv::Point2d& p,const cv::Point2d& s1,const cv::Point2d& s2)
{
	return  (max(s1.x,s2.x) > p.x && p.x > min(s1.x,s2.x)) 
			&& 
			(max(s1.y,s2.y) > p.y && p.y > min(s1.y,s2.y));
};

bool CGraphBuilder::isSeqIntersects(const cv::Point2d& a1,const cv::Point2d& a2,const cv::Point2d& b1,const cv::Point2d& b2)
{
	if(!isPointsOnDiffenertSideOfSeq(a1,a2,b1,b2))
		return false;
	if (isPointBetweenSeq(a1,b1,b2))
		return true;
	if (isPointBetweenSeq(a2,b1,b2))
		return true;
	if (isPointBetweenSeq(b1,a1,a2))
		return true;
	if (isPointBetweenSeq(b2,a1,a2))
		return true;
	return false;
};

bool  CGraphBuilder::isImageIntersects(int id1,int id2)
{
	CMatrix cam2 = _V[id2].cameraPosition;
	CMatrix cam1 = _V[id1].cameraPosition;
	cv::Point2d p2[]=  {cam2.transform(cv::Point2d(0,0)),cam2.transform(cv::Point2d(imageSize.width,0)),cam2.transform(cv::Point2d(0,imageSize.height)),cam2.transform(cv::Point2d(imageSize.width,imageSize.height))};
	cv::Point2d p1[]=  {cam1.transform(cv::Point2d(0,0)),cam1.transform(cv::Point2d(imageSize.width,0)),cam1.transform(cv::Point2d(0,imageSize.height)),cam1.transform(cv::Point2d(imageSize.width,imageSize.height))};
	for(int i = 0; i < 4;++i)
		for(int j = 0; j < 4;++j)
			if( isSeqIntersects(p1[i],p1[(i+1)%4],p2[j],p2[(j+1)%4]) )
				return true;
	return false;
};

void CGraphBuilder::findVertexes()
{
	std::vector<int> AllID = imageManeger.getAllImageID();	
	cv::Point2d center(((double)imageSize.width)/2.0,((double)imageSize.height)/2.0);
	for(size_t i = 0;i<AllID.size();++i)
	{
		_V[AllID[i]] = Vertex();
		_V[AllID[i]].info = imageManeger.getImageInfo(AllID[i]);
		_V[AllID[i]].cameraPosition = CMatrix(_V[AllID[i]].info, AreaInOnePixelPetHeihgt,center);
		cout<<_V[AllID[i]].cameraPosition;
	};
};

void CGraphBuilder::findEdges()
{
	for(auto i = _V.begin(); i != _V.end(); ++i)
	{
		auto j = i;
		while((++j) != _V.end())
		{
			if (isImageIntersects(i->first,j->first))
			{
				i->second.neighbors.push_back(j->first);
				j->second.neighbors.push_back(i->first);
				_E[PairID(j->first,i->first)] = CMatches();
				_E[PairID(i->first,j->first)] = CMatches();
			};
		};
	};
};

void CGraphBuilder::findEdgesHomographies()
{
	std::unordered_set<PairID,hashPairID,EqualUnorderedPairsID> is_calculated;//flag -- homography of edge was found or doesn't exist

	std::unordered_map<int,std::vector<cv::KeyPoint> > keyPoints;
	std::unordered_map<int,cv::Mat> descriptors;

	/**********find homographies in edges**************************/
	for(auto i = _E.begin(); i != _E.end(); ++i)
	{
		if(is_calculated.find(i->first) == is_calculated.end())
		{
			is_calculated.insert(i->first);

			/********************find Homography in edge i***********************/		
			
			if(keyPoints.find(i->first.first) == keyPoints.end())//initialize keyPoints and descriptors if necessary 
			{
				CImage object = imageManeger.getImage(i->first.first);
				cv::Mat temp;
				temp  = object.getMat();//cv::resize(object.getMat(),temp,imageSize);
				CFeatureExtractor object_extractor(&temp);
				keyPoints[i->first.first] = object_extractor.getKeyPoints();
				descriptors[i->first.first] = object_extractor.getDescriptors();
			};

			if(keyPoints.find(i->first.second) == keyPoints.end())//initialize keyPoints and descriptors if necessary 
			{
				CImage scene = imageManeger.getImage(i->first.second);
				cv::Mat temp;
				temp  = scene.getMat();//cv::resize(scene.getMat(),temp,imageSize);
				CFeatureExtractor scene_extractor(&scene.getMat());
				keyPoints[i->first.second] = scene_extractor.getKeyPoints();
				descriptors[i->first.second] = scene_extractor.getDescriptors();
			};

			CFeatureMatcher matcher(descriptors[i->first.first],descriptors[i->first.second]);

			if(matcher.getMatches().size() > 3)//CHomographyFinder needs at least 4 matches
			{
				CHomographyFinder homographyFinder(keyPoints[i->first.first],
												   keyPoints[i->first.second],
												   matcher.getMatches(),_V[i->first.second].info.getAngle()-_V[i->first.first].info.getAngle() );

			/*****if homography is correct set edge******/
				if(homographyFinder.countOfMatches() > 3)
				{
					i->second = homographyFinder.getMatches();
					/***************************/
					auto img_obj =imageManeger.getImage(i->first.first).getMat();
					auto img = matcher.drowMatches(img_obj,
												keyPoints[i->first.first],
												imageManeger.getImage(i->first.second).getMat(),
												keyPoints[i->first.second]);
					cv::imwrite(floader+"/"+std::to_string(i->first.first)+" "+std::to_string(i->first.second)+".jpg",homographyFinder.drowHomography(img_obj,img));
					/**************************/
					i->second = homographyFinder.getMatches().reverse();
				};
			};
		};
	};
	keyPoints.clear();
	descriptors.clear();

	/*******delete all edges wuthout homography*******/
	for(auto i = _V.begin(); i != _V.end(); ++i)
	{
		std::vector<int> goodNeighbors;
		for(auto j = i->second.neighbors.begin(); j !=  i->second.neighbors.end(); ++j)
			if(_E.find(PairID(i->first,*j)) != _E.end())
			{
				if (_E[PairID(i->first,*j)].matchedObjectPoints.size() < 5)
				{
					_E.erase(PairID(i->first,*j));
				}
				else
					goodNeighbors.push_back(*j);
			};
		i->second.neighbors = goodNeighbors;
	};


};

void CGraphBuilder::findCameraPositions()
{
	/*********Gradient minimization***********/
	for(int k = 0; k < 1000;++k)//good enougth for Gradient minimization
	{
		for( int v = 0; v < _V.size(); ++v)
		{
			Point2d f(0,0);
			auto vert = _V[v];
			for( auto n = vert.neighbors.begin(); n != vert.neighbors.end(); ++n)
			{
				auto neighbor = _V[*n];
				auto matches = _E[PairID(v,*n)];
				for(int i = 0; i < matches.matchedObjectPoints.size(); ++i)
				{
					Point2d a = neighbor.cameraPosition.transform(matches.matchedObjectPoints[i]);
					Point2d b = vert.cameraPosition.transform(matches.matchedScenePoints[i]);
					f += (b-a);
				};
				vert.cameraPosition.move(f.x/(double)matches.matchedObjectPoints.size(),f.y/(double)matches.matchedObjectPoints.size());
			}


		};
	};

};

void CGraphBuilder::initPano()
{
	pano = cv::imread(floader+"/"+"output.jpg");
};

void CGraphBuilder::drowEdges()
{
	for(auto i = _E.begin(); i != _E.end(); ++i)
	{
		CImageInfo& i1 = _V[i->first.first].info;
		CImageInfo& i2 = _V[i->first.second].info;
		cv::line(pano,cv::Point2d(i1.getX(),i1.getY()),cv::Point2d(i2.getX(),i2.getY()),cv::Scalar( 0,0, 255), 10);
	};
};

void CGraphBuilder::drowCameraHomographyes()
{
	for(auto i = _V.begin(); i != _V.end(); ++i)
		CImageHomography::show(pano,imageManeger.getImage(i->first).getMat(),i->second.cameraPosition,cv::Scalar(0,255,0));
};

void CGraphBuilder::DrowAll()
{
	initPano();
	drowEdges();
	drowCameraHomographyes();
};