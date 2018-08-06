/*
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at http://live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppPal.hpp"
#include <Model/CubismMoc.hpp>
#include <cstdlib>
#include <cstdarg>
#include <cstdio>

#ifndef CSM_TARGET_WIN_GL
#include <sys/time.h>
#endif

// TODO: Remove.
// #include "cocos2d.h"

using namespace Csm;
// TODO: Remove.
// USING_NS_CC;

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

struct timeval* LAppPal::_lastUpdate = new struct timeval;
float LAppPal::_deltaTime = 0;
bool LAppPal::_nextDeltaTimeZero = false;

#ifdef CSM_TARGET_WIN_GL
typedef long ssize_t;
#endif

csmByte* LAppPal::LoadFileAsBytes(const csmChar* filePath, csmSizeInt* outSize)
{
    FILE* file = fopen(filePath, "rb");
    fseek(file, 0, SEEK_END);
    const ssize_t size = ftell(file);
    rewind(file);
    csmByte* buf = new csmByte[size];
    fread(buf, size, 1, file);
    fclose(file);
    *outSize = static_cast<csmSizeInt>(size);
    return buf;
}

void LAppPal::ReleaseBytes(csmByte* byteData)
{
    free(byteData);
}

csmFloat32  LAppPal::GetDeltaTime()
{
    CalculateDeltaTime();
    
    return _deltaTime;
}

void LAppPal::CalculateDeltaTime()
{
    struct timeval now;

    if (gettimeofday(&now, NULL) != 0)
    {
        _deltaTime = 0;
        return;
    }

    // new delta time. Re-fixed issue #1277
    if (_nextDeltaTimeZero)
    {
        _deltaTime = 0;
        _nextDeltaTimeZero = false;
    }
    else
    {
        _deltaTime = (now.tv_sec - _lastUpdate->tv_sec) + (now.tv_usec - _lastUpdate->tv_usec) / 1000000.0f;
        if (0 > _deltaTime) { _deltaTime = 0; }
    }

    *_lastUpdate = now;
}

void LAppPal::PrintLog(const csmChar* format, ...)
{
    va_list args;
    csmChar buf[256];
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    printf("%s\n", buf);
    va_end(args);
}

void LAppPal::PrintMessage(const csmChar* message)
{
    PrintLog("%s", message);
}
