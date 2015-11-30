#pragma once
#include <opencv2/core/core.hpp>
#include <exception>
#include <math.h>
#include "../CImageInfo/ImageInfo.h"
#include <iostream>

class CMatrix
{	
	size_t cols,rows;

public:

	static CMatrix one();

	static CMatrix  rotate(double angle);

	static CMatrix stretch(double x,double y);

	static CMatrix move(double x,double y);

	cv::Mat matrix;


	CMatrix(cv::Mat m): cols(m.cols), rows(m.rows), matrix(m){};

	CMatrix(const CImageInfo& imInf,double AreaInOnePixelPetHeihgt,cv::Point2d center) 
	{
		(*this) = one();
		double a = cos(imInf.getAngle())*AreaInOnePixelPetHeihgt;
		double b = sin(imInf.getAngle())*AreaInOnePixelPetHeihgt;
		(*this)(0,0) = a;
		(*this)(1,1) = a;
		(*this)(1,0) = b;
		(*this)(0,1) = -b;
		(*this)(2,0) = -(a*center.x+b*center.y)+imInf.getX();
		(*this)(2,1) = -(-b*center.x+a*center.y)+imInf.getY();
		(*this)(2,2) = 1;
		(*this)(0,2) = (*this)(1,2) = 0;
	};

	CMatrix& operator=(cv::Mat m);

	CMatrix(size_t _cols = 3, size_t _rows = 3);

	operator cv::Mat()const;

	operator cv::Mat&();

	double& operator()(size_t i,size_t j);

	double operator()(size_t i,size_t j)const;

	CMatrix operator *(const CMatrix& m)const;

	CMatrix operator -(const CMatrix& m)const;

	CMatrix operator +(const CMatrix& m)const;

	CMatrix operator /(const double& m)const;

	CMatrix operator *(const double& m)const;
	bool operator == ( const CMatrix& m)const;
	//just for 3x3 matrix
	CMatrix get3x2()const;
	CMatrix& set3x2(const CMatrix&);

	cv::Point2d transform(const cv::Point2d& p)const;

	double det()const;
	std::pair<double,double> CMatrix::eigenValues()const;//if one of eigenValues is complex returns (0,0)
	double getAngle()const;
};

std::ostream& operator << (std::ostream& s,const CMatrix& m);