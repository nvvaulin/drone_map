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
#include "../CImageInfo/ImageInfo.h"
#include "../CImage/Image.h"
#include <map>
#include <string>
#include <vector>
#include <fstream>

class CImageManager {
public:
    
    
    //  обход каталога по заданному пути
    //  запоминает названия фото и присваивает
    //  каждой свой уникальный ID
    CImageManager( std::string );
    
    ~CImageManager();
    
    
    //  получить вектор с ID всех фото
    std::vector<int> getAllImageID() const;
    
    //  получить метаданные фото по ID
    CImageInfo getImageInfo( int );
    
    //  получить фото по ID
    CImage getImage( int );
private:
    
    //  хранит в качестве ключа ID фото, второе поле - название файла
    std::map<int, std::string> cash;
    
    //  хранит в качестве ключа ID фото, второе поле - логи фото
    std::map<int, CImageInfo> metadata;
};


#endif /* defined(__MAPS_stitching_with_OpenCV__ImageManager__) */