//
//  Image.h
//  MAPS stitching with OpenCV
//
//  Created by Даниил Назаров on 08.04.15.
//  Copyright (c) 2015 Даниил Назаров. All rights reserved.
//

#ifndef __MAPS_stitching_with_OpenCV__Image__
#define __MAPS_stitching_with_OpenCV__Image__

#include "../CImageInfo/ImageInfo.h"
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <string>
using namespace cv;
using namespace std;

class CImage {
public:
    CImage();
    //  конструктор фото из директории
    CImage( string );
    ~CImage();
    
    //  загрузить фото из директории
    void upload( string );
    //  вывести фото на экран
    void show() const;
    //  изменить размер фото
    void zoom( double );
    //  получить доступ к Mat
    Mat& getMat();

private:
    Mat image;
};


#endif /* defined(__MAPS_stitching_with_OpenCV__Image__) */
