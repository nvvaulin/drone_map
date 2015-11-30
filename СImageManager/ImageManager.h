//
//  ImageManager.h
//  MAPS stitching with OpenCV
//
//  Created by Даниил Назаров on 08.04.15.
//  Copyright (c) 2015 Даниил Назаров. All rights reserved.
//

#ifndef __MAPS_stitching_with_OpenCV__ImageManager__
#define __MAPS_stitching_with_OpenCV__ImageManager__

#include <stdio.h>
#include "ImageInfo.h"
#include "Image.h"
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <boost/filesystem.hpp>
using namespace std;
using namespace boost::filesystem;

class CImageManager {
public:
    
    CImageManager();
    
    //  обход каталога по заданному пути
    //  запоминает названия фото и присваивает
    //  каждой свой уникальный ID
    CImageManager( string );
    
    ~CImageManager();
    
    //  обход каталога по заданному пути
    //  запоминает названия фото и присваивает
    //  каждой свой уникальный ID
    void upload( string );
    
    //  получить вектор с ID всех фото
    vector<int> getAllImageID() const;
    
    //  получить метаданные фото по ID
    CImageInfo getImageInfo( int );
    
    //  получить фото по ID
    CImage getImage( int );
private:
    
    //  хранит в качестве ключа ID фото, второе поле - название файла
    map<int, string> cash;
    
    //  хранит в качестве ключа ID фото, второе поле - логи фото
    map<int, CImageInfo> metadata;
};


#endif /* defined(__MAPS_stitching_with_OpenCV__ImageManager__) */
