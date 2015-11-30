#pragma once
#include "../../CGraphBuilder/CGraphBuilder.h"
#include "CTestImageCreator.h"
#include <iomanip>   
#include <fstream>
using namespace std;
using namespace cv;


class CMatrixTest
{
public:
	static void angleTest();
	static void moveTest();

	static double drand(){
		return 0.5 - (double(rand() % 100000))/100000.0;
	};

	static bool eqDub(double v1,double v2){
		return abs(v1-v2)<0.000001;
	};
	
	CMatrixTest(){
		moveTest();
		angleTest();
	}
};


