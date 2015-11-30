//
//  ImageInfo.cpp
//  MAPS stitching with OpenCV
//
//  Created by Даниил Назаров on 08.04.15.
//  Copyright (c) 2015 Даниил Назаров. All rights reserved.
//

#include "ImageInfo.h"


CImageInfo::CImageInfo() :
    x( 0 ),
    y( 0 ),
    height( 0 ),
    angle( 0 )
{
    
}

CImageInfo::CImageInfo( double _x, double _y,
                       double _height, double _angle ) :
    x( _x ),
    y( _y ),
    height( _height ),
    angle( _angle )
{
    
}

CImageInfo::~CImageInfo()
{
    
}

void CImageInfo::push( double _x, double _y,
                      double _height, double _angle )
{
    x = _x;
    y = _y;
    height = _height;
    angle = _angle;
}

double CImageInfo::getX() const
{
    return x;
}

double CImageInfo::getY() const
{
    return y;
}

double CImageInfo::getHeight() const
{
    return height;
}

double CImageInfo::getAngle() const
{
    return angle;
}