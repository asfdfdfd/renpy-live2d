/*
* Copyright(c) Live2D Inc. All rights reserved.
*
* Use of this source code is governed by the Live2D Open Software license
* that can be found at http://live2d.com/eula/live2d-open-software-license-agreement_en.html.
*/

#include "LAppPal.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>  
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <Model/CubismMoc.hpp>
#include "LAppDefine.hpp"

using std::endl;
using namespace Csm;
using namespace std;
using namespace LAppDefine;

struct timeval LAppPal::s_lastFrame = timeval();
double LAppPal::s_deltaTime = 0.0;

#ifndef CSM_TARGET_WIN_GL
#include <sys/time.h>
#endif

#ifdef CSM_TARGET_WIN_GL

#include <winsock2.h>

typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970 
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
    return 0;
}
#endif 

#ifdef CSM_TARGET_WIN_GL
typedef long ssize_t;
#endif

csmByte* LAppPal::LoadFileAsBytes(const string filePath, csmSizeInt* outSize)
{
    //filePath;// 
    const char* path = filePath.c_str();

    int size = 0;
    struct stat statBuf;
    if (stat(path, &statBuf) == 0)
    {
        size = statBuf.st_size;
    }

    std::fstream file;
    char* buf = new char[size];

    file.open(path, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        if (DebugLogEnable)
        {
            PrintLog("file open error");
        }
        return NULL;
    }
    file.read(buf, size);
    file.close();
    
    *outSize = size;
    return reinterpret_cast<csmByte*>(buf);
}

void LAppPal::ReleaseBytes(csmByte* byteData)
{
    delete[] byteData;
}

csmFloat32 LAppPal::GetDeltaTime()
{
    return static_cast<csmFloat32>(s_deltaTime);
}

void LAppPal::UpdateTime()
{
    struct timeval now;    
    
    if (gettimeofday(&now, NULL) != 0)
    {
        s_deltaTime = 0;
        return;
    }
        
    s_deltaTime = (now.tv_sec - s_lastFrame.tv_sec) + (now.tv_usec - s_lastFrame.tv_usec) / 1000000.0f;
    
    if (0 > s_deltaTime) { 
        s_deltaTime = 0; 
    }

    s_lastFrame = now;
}

void LAppPal::PrintLog(const csmChar* format, ...)
{
    va_list args;
    csmChar buf[256];
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args); // 標準出力でレンダリング
    std::cerr << buf << std::endl;
    va_end(args);
}

void LAppPal::PrintMessage(const csmChar* message)
{
    PrintLog("%s", message);
}
