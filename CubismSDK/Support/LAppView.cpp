/*
* Copyright(c) Live2D Inc. All rights reserved.
*
* Use of this source code is governed by the Live2D Open Software license
* that can be found at http://live2d.com/eula/live2d-open-software-license-agreement_en.html.
*/

#include "LAppView.hpp"
#include <math.h>
#include <string>
#include "LAppPal.hpp"
#include "LAppDelegate.hpp"
#include "LAppTextureManager.hpp"
#include "LAppDefine.hpp"

using namespace std;
using namespace LAppDefine;

LAppView::LAppView():
    _programId(0)
{
    // デバイス座標からスクリーン座標に変換するための
    _deviceToScreen = new CubismMatrix44();

    // 画面の表示の拡大縮小や移動の変換を行う行列
    _viewMatrix = new CubismViewMatrix();
}

LAppView::~LAppView()
{ 
    delete _viewMatrix;
    delete _deviceToScreen;
}

void LAppView::Initialize()
{
    // int width, height;
    // glfwGetWindowSize(LAppDelegate::GetInstance()->GetWindow(), &width, &height);
    //
    // float ratio = static_cast<float>(height) / static_cast<float>(width);
    // float left = ViewLogicalLeft;
    // float right = ViewLogicalRight;
    // float bottom = -ratio;
    // float top = ratio;
    //
    // _viewMatrix->SetScreenRect(left, right, bottom, top); // デバイスに対応する画面の範囲。 Xの左端, Xの右端, Yの下端, Yの上端
    //
    // float screenW = fabsf(left - right);
    // _deviceToScreen->ScaleRelative(screenW / width, -screenW / width);
    // _deviceToScreen->TranslateRelative(-width * 0.5f, -height * 0.5f);
    //
    // // 表示範囲の設定
    // _viewMatrix->SetMaxScale(ViewMaxScale); // 限界拡大率
    // _viewMatrix->SetMinScale(ViewMinScale); // 限界縮小率
    //
    // // 表示できる最大範囲
    // _viewMatrix->SetMaxScreenRect(
    //     ViewLogicalMaxLeft,
    //     ViewLogicalMaxRight,
    //     ViewLogicalMaxBottom,
    //     ViewLogicalMaxTop
    // );
}

void LAppView::Render() 
{

}

void LAppView::InitializeSprite()
{ 
    _programId = LAppDelegate::GetInstance()->CreateShader();
}