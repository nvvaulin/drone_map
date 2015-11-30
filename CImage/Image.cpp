//
//  Image.cpp
//  MAPS stitching with OpenCV
//
//  Created by Даниил Назаров on 08.04.15.
//  Copyright (c) 2015 Даниил Назаров. All rights reserved.
//

#include "Image.h"


CImage::CImage()
{
}

CImage::CImage( string dir )
{
    image = imread( dir, CV_LOAD_IMAGE_COLOR );
    if( !image.data )
    {
        cout <<  "Could not open or find the image" << endl;
        exit( 0 );
    }
}

CImage::~CImage()
{
}

void CImage::upload( string dir )
{
    image = imread( dir, CV_LOAD_IMAGE_COLOR );
    if( !image.data )
    {
        cout <<  "Could not open or find the image" << endl ;
        exit( 0 );
    }
    
}

void CImage::show() const
{
    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", image );
    
    waitKey( 0 );
}

void CImage::zoom( double percent )
{
    IplImage img = image;
    IplImage* temp = cvCreateImage( cvSize( img.width * percent / 100, img.height * percent / 100), img.depth, img.nChannels );
    
    /* INTER_NEAREST - a nearest-neighbor interpolation
    INTER_LINEAR - a bilinear interpolation (used by default)
    INTER_AREA - resampling using pixel area relation. It may be a preferred method for image decimation,
    as it gives moire’-free results. But when the image is zoomed, it is similar to the INTER_NEAREST method.
    INTER_CUBIC - a bicubic interpolation over 4x4 pixel neighborhood
    INTER_LANCZOS4 - a Lanczos interpolation over 8x8 pixel neighborhood*/
    
    cvResize( &img, temp, CV_INTER_LINEAR );
    image = temp;
}

Mat& CImage::getMat()
{
    return image;
}
