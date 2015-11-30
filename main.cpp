//
//  main.cpp
//  MAPS stitching with OpenCV
//
//  Created by Даниил Назаров on 08.04.15.
//  Copyright (c) 2015 Даниил Назаров. All rights reserved.
//


#include "ImageManager.h"

int main()
{
    CImageManager test("/Users/Daniil/Desktop/Новая папка");
    //  так тоже работает
    //  test.upload("/Users/Daniil/Desktop/Новая папка");
    
    vector<int> IDs = test.getAllImageID();
    for( int i = 0; i < IDs.size(); ++i ) {
        cout << IDs[i] << ' ';
    }
    cout << endl;
    
    CImageInfo a = test.getImageInfo( 1 );
    cout << a.getX() << ' ' << a.getY() <<
    ' ' << a.getHeight() << ' ' << a.getAngle() << endl;
    
    CImage b = test.getImage( 3 );
    b.zoom( 30 );
    b.show();
    Mat c = b.getMat();
    
    return 0;
}
