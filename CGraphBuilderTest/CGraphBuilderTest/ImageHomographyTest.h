#pragma once
#include "../../CGraphBuilder/CGraphBuilder.h"
#include <iomanip>   
#include <fstream>
using namespace std;
using namespace cv;
/*
class CImageHomographyTest
{
public:
	CImageManager m1;
	vector<int> ids;
	cv::Mat pano;
	

	void testTransformToGlobalBasis()
	{
		for(int i = 0; i< ids.size();++i)
		{
			auto info = m1.getImageInfo(ids[i]);
			CImageHomography::show(pano,m1.getImage(ids[i]).getMat(),CImageHomography::getTransformToGlobalBasis(info),cv::Scalar(0,255,0));

		};
	};

	void test1()
	{
		for(int i = 0; i< ids.size();++i)
		{
			auto info = m1.getImageInfo(ids[i]);
			auto m = CImageHomography::getTransformToLocalBasis(info)*CImageHomography::getTransformToGlobalBasis(info) ;
			if (!(m == CMatrix::one()))
				cout<<"test1 error1"<<m;
			m = CImageHomography::getTransformToGlobalBasis(info)*CImageHomography::getTransformToLocalBasis(info) ;
			if (!(m == CMatrix::one()))
				cout<<"test1 error2"<<m;

		};
	};

	void setHInLB()
	{
		auto img  = m1.getImage(ids[0]).getMat();
		cv::Size size(img.cols,img.rows);
		for(int i = 0; i< ids.size();++i)
		{
			auto info = m1.getImageInfo(ids[i]);
			
			auto m = CMatrix::one();
			CImageHomography h;
			h.setHomographyInLocalBasis(m,info,info);
			CImageHomography::show(pano,m1.getImage(ids[i]).getMat(),h.getHomographyInGlobalBasis(),cv::Scalar(255,0,0));
		}

	};


	CImageHomographyTest(string floader,double arperture_x) : m1(floader+"/test")
	{
		pano = imread(floader+"/input.jpg");
		ids = m1.getAllImageID();
		auto temp = m1.getImage(ids[0]).getMat();
		CImageHomography::setConstantForStretch(arperture_x,Size(temp.cols,temp.rows));
		testTransformToGlobalBasis();
		test1();
		setHInLB();
		imwrite(floader+"/CImageHomographyTest.jpg",pano);
	}

	~CImageHomographyTest(void);
};

*/