//
//  ImageInfo.h
//  MAPS stitching with OpenCV
//
//  Created by Даниил Назаров on 08.04.15.
//  Copyright (c) 2015 Даниил Назаров. All rights reserved.
//

#ifndef __MAPS_stitching_with_OpenCV__ImageInfo__
#define __MAPS_stitching_with_OpenCV__ImageInfo__

class CImageInfo {
public:
    CImageInfo();
    CImageInfo( double, double, double, double );
    ~CImageInfo();
    
    void push( double, double, double, double );
    
    double getX() const;
    double getY() const;
    double getHeight() const;
    double getAngle() const;
    
private:
    double x;
    double y;
    
    double height;
    double angle;
};

#endif /* defined(__MAPS_stitching_with_OpenCV__ImageInfo__) */