//
//  ImageManager.cpp
//  MAPS stitching with OpenCV
//
//  Created by Даниил Назаров on 08.04.15.
//  Copyright (c) 2015 Даниил Назаров. All rights reserved.
//

#include "ImageManager.h"


CImageManager::CImageManager()
{
}

CImageManager::CImageManager( string dir )
{
    if( is_directory( dir ) ) {
        
        string ext, fileName;
        ifstream log;
        CImageInfo next;
        int IDc = 0;
        int IDm = 0;
        recursive_directory_iterator end;
        
        for( recursive_directory_iterator it( dir ); it != end; ++it )
        {
            ext = it->path().extension().string();
            fileName = it->path().string();
            if( ext == ".jpg" || ext == ".jpeg" || ext == ".bmp" || ext == ".png" ) {
                IDc++;
                cash.insert( pair<int, string> ( IDc, fileName ) );
            }
            if( ext == ".txt" ) {
                log.open( fileName );
                if( log.fail() ) {
                    cout << "Error with opening log file!" << endl;
                    exit( 99 );
                } else {
                    double x, y, h, a;
                    while( !log.eof() ) {
                        IDm++;
                        log >> x >> y >> h >> a;
                        next.push( x, y, h, a );
                        metadata.insert( pair<int, CImageInfo>( IDm, next ) );
                    }
                }
            }
        }
        log.close();
        
    } else {
        cout << "Error! Wrong path!" << endl;
        exit( 100 );
    }

}

CImageManager::~CImageManager()
{

}

void CImageManager::upload( string dir )
{
    if( is_directory( dir ) ) {
        
        string ext, fileName;
        ifstream log;
        CImageInfo next;
        int IDc = 0;
        int IDm = 0;
        recursive_directory_iterator end;
        
        for( recursive_directory_iterator it( dir ); it != end; ++it )
        {
            ext = it->path().extension().string();
            fileName = it->path().string();
            if( ext == ".jpg" || ext == ".jpeg" || ext == ".bmp" || ext == ".png" ) {
                IDc++;
                cash.insert( pair<int, string> ( IDc, fileName ) );
            }
            if( ext == ".txt" ) {
                log.open( fileName );
                if( log.fail() ) {
                    cout << "Error with opening log file!" << endl;
                    exit( 99 );
                } else {
                    double x, y, h, a;
                    while( !log.eof() ) {
                        IDm++;
                        log >> x >> y >> h >> a;
                        next.push( x, y, h, a );
                        metadata.insert( pair<int, CImageInfo>( IDm, next ) );
                     }
                }
            }
        }
        log.close();
        
    } else {
        cout << "Error! Wrong path!" << endl;
        exit( 100 );
    }
}

vector<int> CImageManager::getAllImageID() const
{
    if( cash.empty() ) {
        cout << "Error! Cash is Empty!" << endl;
        exit( 101 );
    } else {
        vector<int> imgID;
    
        for( auto it = cash.begin(); it != cash.end(); ++it ) {
            imgID.push_back( it->first );
        }

        return imgID;
    }
}

CImageInfo CImageManager::getImageInfo( int ID )
{
    if( metadata.empty() || metadata.find( ID ) == metadata.end() ) {
        cout << "Error! Not valid ID" << endl;
        exit( 102 );
    } else {
        return metadata[ID];
    }
}

CImage CImageManager::getImage( int ID )
{
    if( cash.empty() || cash.find( ID ) == cash.end() ) {
        cout << "Error! Not valid ID" << endl;
        exit( 102 );
    } else {
        CImage temp;
        temp.upload( cash[ID] );
        return temp;
    }
    
}

