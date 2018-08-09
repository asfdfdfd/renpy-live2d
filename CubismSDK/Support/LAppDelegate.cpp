/*
* Copyright(c) Live2D Inc. All rights reserved.
*
* Use of this source code is governed by the Live2D Open Software license
* that can be found at http://live2d.com/eula/live2d-open-software-license-agreement_en.html.
*/

#include "LAppDelegate.hpp"
#include <iostream>
#include <sstream>
#include "LAppPal.hpp"
#include "LAppDefine.hpp"
#include "LAppTextureManager.hpp"

using namespace Csm;
using namespace std;
using namespace LAppDefine;

namespace {
    LAppDelegate* s_instance = NULL;
}

LAppDelegate* LAppDelegate::GetInstance()
{
    if (s_instance == NULL)
    {
        s_instance = new LAppDelegate();
    }

    return s_instance;
}

void LAppDelegate::ReleaseInstance()
{
    if (s_instance != NULL)
    {
        delete s_instance;
    }

    s_instance = NULL;
}

bool LAppDelegate::Initialize()
{
    if (DebugLogEnable)
    {
        LAppPal::PrintLog("START");
    }

    // Cubism3の初期化
    InitializeCubism();
        
    return GL_TRUE;
}

void LAppDelegate::Release()
{
    delete _textureManager;

    // リソースを解放
    // LAppLive2DManager::ReleaseInstance();
    
    //Cubism3の解放
    CubismFramework::Dispose();
}

LAppDelegate::LAppDelegate():
    _cubismOption(),
    _captured(false),
    _isEnd(false)
{
    _textureManager = new LAppTextureManager();
}

LAppDelegate::~LAppDelegate()
{
    
}

void LAppDelegate::InitializeCubism()
{
    //setup cubism
    _cubismOption.LogFunction = LAppPal::PrintMessage;
    _cubismOption.LoggingLevel = LAppDefine::CubismLoggingLevel;
    Csm::CubismFramework::StartUp(&_cubismAllocator, &_cubismOption);

    //Initialize cubism
    CubismFramework::Initialize();

    //default proj
    CubismMatrix44 projection;

    LAppPal::UpdateTime();
}

GLuint LAppDelegate::CreateShader()
{
    //バーテックスシェーダのコンパイル
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShader =
        "attribute vec3 position;"
        "attribute vec2 uv;"
        "varying vec2 vuv;"
        "void main(void){"
        "    gl_Position = vec4(position, 1.0);"
        "    vuv = uv;"
        "}";
    glShaderSource(vertexShaderId, 1, &vertexShader, NULL);
    glCompileShader(vertexShaderId);

    //フラグメントシェーダのコンパイル
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShader =
        "varying vec2 vuv;"
        "uniform sampler2D texture;"
        "void main(void){"
        "    gl_FragColor = texture2D(texture, vuv);"
        "}";
    glShaderSource(fragmentShaderId, 1, &fragmentShader, NULL);
    glCompileShader(fragmentShaderId);

    //プログラムオブジェクトの作成
    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    // リンク
    glLinkProgram(programId);

    glUseProgram(programId);

    return programId;
}

Csm::csmVector<string> LAppDelegate::Split(const std::string& baseString, char delimiter)
{
    Csm::csmVector<string> elems;
    stringstream ss(baseString);
    string item;
    while(getline(ss, item, delimiter))
    {
        if(!item.empty())
        {
            elems.PushBack(item);
        }
    }
    return elems;
}
