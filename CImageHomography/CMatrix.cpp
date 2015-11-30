#include "CMatrix.h"
#include <iomanip>   


CMatrix& CMatrix::operator=(cv::Mat m)
{
	(*this) = CMatrix(m);
	return (*this);
};

CMatrix::CMatrix(size_t _cols, size_t _rows): cols(_cols), rows(_rows)
{
	matrix =  cv::Mat_<double>(cv::Size(_cols,_rows));
};

CMatrix::operator cv::Mat()const
{
	return matrix;
};

CMatrix::operator cv::Mat&()
{
	return matrix;
};

double& CMatrix::operator()(size_t i,size_t j)
{
	return matrix.at<double>(j,i);
};

double CMatrix::operator()(size_t i,size_t j)const
{
	return matrix.at<double>(j,i);
};

CMatrix CMatrix::operator *(const CMatrix& m)const 
{
#ifdef _DEBUG
	if (cols != m.rows)
		throw std::exception("incorrect type of matrix in multiply");
#endif
	CMatrix res(m.cols,rows);
	for(size_t i = 0; i< rows;++i)
		for(size_t j = 0; j<m.cols;++j)
		{
			double s = 0;
			for(size_t k = 0; k<cols; ++k)
				s+=(*this)(k,i)*m(j,k);
			res(j,i) = s;
		};
	return res;
};

CMatrix CMatrix::operator -(const CMatrix& m)const 
{
	CMatrix res(cols,rows);
#ifdef _DEBUG
	if ((cols != m.cols) || (rows != m.rows))
		throw std::exception("incorrect type of matrix in subtraction");
#endif
	for(size_t i = 0; i< cols;++i)
		for(size_t j = 0; j<rows;++j)
		{
			res(i,j) = (*this)(i,j) - m(i,j);
		};
	return res;
};


CMatrix CMatrix::operator +(const CMatrix& m)const 
{
	CMatrix res(cols,rows);
#ifdef _DEBUG
	if ((cols != m.cols) || (rows != m.rows))
		throw std::exception("incorrect type of matrix in sum");
#endif
	for(size_t i = 0; i< cols;++i)
		for(size_t j = 0; j<rows;++j)
		{
			res(i,j) = (*this)(i,j) + m(i,j);
		};
	return res;
};


CMatrix CMatrix::operator /(const double& m)const 
{
	CMatrix res(cols,rows);
#ifdef _DEBUG
	if (m==0)
		throw std::exception("division matrix on 0");
#endif
	for(size_t i = 0; i< rows;++i)
		for(size_t j = 0; j<cols;++j)
		{
			res(i,j) = (*this)(i,j)/m;
		};
	return res;
};

CMatrix CMatrix::operator *(const double& m)const 
{
	CMatrix res(cols,rows);
	for(size_t i = 0; i< rows;++i)
		for(size_t j = 0; j<cols;++j)
		{
			res(i,j) = (*this)(i,j)*m;
		};
	return res;
};


CMatrix CMatrix::rotate(double angle)// b = matrix::rotate(x)*a; (angle from a to b) = x
{
	CMatrix res(3,3);
	res(0,0) = cos(angle);
	res(0,1) = sin(angle);
	res(1,1) = cos(angle);
	res(1,0) =-sin(angle);
	res(2,0) =res(2,1) = res(1,2) =res(0,2)  = 0;
	res(2,2) = 1;
	return res;
};

CMatrix CMatrix::stretch(double x,double y)
{
	CMatrix res(3,3);
	res(0,0) = x;
	res(1,1) = y;
	res(0,1) = res(1,0) = res(2,0) =res(2,1) = res(1,2) =res(0,2)  = 0;
	res(2,2) = 1;
	return res;
};

CMatrix CMatrix::move(double x,double y)
{
	CMatrix res(3,3);
	res(2,0) = x;
	res(2,1) = y;
	res(1,1) = res(0,0)= 1;
	res(2,2) = res(1,0) = res(1,2) =res(0,2)  = res(0,1) = 0;
	return res;
};

CMatrix CMatrix::one()
{
	CMatrix res(3,3);
	res(0,0) = 1;
	res(1,1) = 1;
	res(0,1) = res(1,0) = res(2,0) =res(2,1) = res(1,2) =res(0,2)  = 0;
	res(2,2) = 1;
	return res;

};


cv::Point2d  CMatrix::transform(const cv::Point2d& p)const
{
#ifdef _DEBUG
	if(cols != 3 || rows != 3)
		throw std::exception("incorrect type of matrix in transform");
#endif
	cv::Point2d res;
	res.x = (*this)(0,0)*p.x+(*this)(1,0)*p.y+(*this)(2,0);
	res.y = (*this)(0,1)*p.x+(*this)(1,1)*p.y+(*this)(2,1);

	return res;
};


std::pair<double,double> CMatrix::eigenValues()const
{
#ifdef _DEBUG
	if(cols != 3 || rows != 3)
		throw std::exception("incorrect type of matrix in eigenvectors");
#endif
	double eigenvalue1,eigenvalue2;
	double a = (*this)(0,0);
	double b = (*this)(0,1);
	double c = (*this)(1,0);
	double d = (*this)(1,1);
	double desc = (a+d)*(a+d)-4*(a*d-b*c);

	if (desc < 0 )
		return std::pair<double,double>(0,0);

	eigenvalue1 = ((a+d)+sqrt(desc))/2;
	eigenvalue2 = ((a+d)-sqrt(desc))/2;

	return std::pair<double,double>(eigenvalue1,eigenvalue2);
};

double CMatrix::det()const
{
#ifdef _DEBUG
	if(cols != 3 || rows != 3)
		throw std::exception("incorrect type of matrix in eigenvectors");
#endif
	return (*this)(0,0)*(*this)(1,1)-(*this)(1,0)*(*this)(0,1);
};


double CMatrix::getAngle()const
{
	double x = (*this)(0,0);
	double y = (*this)(0,1);
	double angle = acos(x/sqrt(x*x+y*y));
	if  ( y < 0 )
		return -angle;
	else
		return angle;
};





std::ostream& operator << (std::ostream& s,const CMatrix& m)
{

	for(int j=0;j<m.matrix.rows;++j){
		for(int i=0;i<m.matrix.cols;++i)
			s<<std::fixed<<std::setprecision(3)<<m(i,j)<< ' ';
		s<<'\n';
	};
	s<<'\n';
	return s;

};

CMatrix CMatrix::get3x2()const
{
	CMatrix res(3,2);
	for(int j=0;j<2;++j)
		for(int i=0;i<3;++i)
			res(i,j) = (*this)(i,j);
	return res;
};



bool CMatrix::operator == ( const CMatrix& m)const
{
	if (cols != m.cols || rows != m.rows)
		return false;
	for(int j=0;j<2;++j)
		for(int i=0;i<3;++i)
			if(abs(m(i,j) - (*this)(i,j)) > 0.000000001)
				return false;
	return true;
};


CMatrix& CMatrix::set3x2(const CMatrix& m)
{

	for(int j=0;j<2;++j)
		for(int i=0;i<3;++i)
			(*this)(i,j) = m(i,j);
	(*this)(0,2) = (*this)(1,2) = 0;
	(*this)(2,2) = 1;
	return (*this);
};