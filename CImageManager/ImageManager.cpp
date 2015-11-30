//
//  ImageManager.cpp
//  MAPS stitching with OpenCV
//
//  Created by Даниил Назаров on 08.04.15.
//  Copyright (c) 2015 Даниил Назаров. All rights reserved.
//

#include "ImageManager.h"
#include <windows.h>
#include <tchar.h>
#include <fstream>
using namespace std;



vector<string> getFilesInDir(string _path, string s)
{

	vector<string> files;
    char path[MAX_PATH];
    WIN32_FIND_DATA ffd;

	if (_tcslen(_path.c_str()) > MAX_PATH - 4)
    {
        cerr << _T("Your path is too long.") << endl;
		return files;
    }

    _tcsncpy(path,_path.c_str(), MAX_PATH);
    if (path[_tcslen(path) - 1] != '\\')
        _tcscat(path, _T("\\"));
    _tcscat(path, _T("*."));
	 _tcscat(path, s.c_str());

    HANDLE hFind = FindFirstFile(path, &ffd);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        cerr << _T("Invalid handle value.") << GetLastError() << endl;
		return files;
    }
    while (1)
    {
		files.push_back(ffd.cFileName);
        if (!FindNextFile(hFind, &ffd))
            break;
    }
	return files;
}


CImageManager::CImageManager( string dir )
{
	string s[] = {"png", "jpg", "jpeg"};
	int id = 0;
	ifstream info(dir+"\\info.txt");
	double x,y,r,h;
	for(int i = 0; i < 3;++i)
	{
		auto files = getFilesInDir(dir,s[i]);
		for(int j = 0; j < files.size();++j)
		{
			info>>x>>y>>h>>r;
			cash.insert(pair<int, string>(id,dir+'\\'+files[j]));
			metadata.insert(pair<int, CImageInfo> (id,CImageInfo(x,y,h,r)));
			++id;
		}
	};

}

CImageManager::~CImageManager()
{

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
