/*
* Copyright(c) Live2D Inc. All rights reserved.
*
* Use of this source code is governed by the Live2D Open Software license
* that can be found at http://live2d.com/eula/live2d-open-software-license-agreement_en.html.
*/


#include "LAppLive2DManager.hpp"
#include <string>
#include <Rendering/CubismRenderer.hpp>
#include "LAppPal.hpp"
#include "LAppDefine.hpp"
#include "LAppDelegate.hpp"
#include "LAppModel.hpp"

using namespace Csm;
using namespace LAppDefine;
using namespace std;

namespace {
    LAppLive2DManager* s_instance = NULL;
}

LAppLive2DManager* LAppLive2DManager::GetInstance()
{
    if (s_instance == NULL)
    {
        s_instance = new LAppLive2DManager();
    }

    return s_instance;
}

void LAppLive2DManager::ReleaseInstance()
{
    if (s_instance != NULL)
    {
        delete s_instance;
    }

    s_instance = NULL;
}

LAppLive2DManager::LAppLive2DManager()
    : _viewMatrix(NULL)
{
}

LAppLive2DManager::~LAppLive2DManager()
{
    ReleaseAllModel();
}

void LAppLive2DManager::ReleaseAllModel()
{
    for (csmUint32 i = 0; i < _models.GetSize(); i++)
    {
        delete _models[i];
    }

    _models.Clear();
}

LAppModel* LAppLive2DManager::GetModel(csmUint32 no) const
{
    if (no < _models.GetSize())
    {
        return _models[no];
    }

    return NULL;
}

void LAppLive2DManager::OnUpdate() const
{
    // CubismMatrix44 projection;
    // int width, height;
    // glfwGetWindowSize(LAppDelegate::GetInstance()->GetWindow(), &width, &height);
    // projection.Scale(1.0f, static_cast<float>(width) / static_cast<float>(height));
    //
    // if (_viewMatrix != NULL)
    // {
    //     projection.MultiplyByMatrix(_viewMatrix);
    // }
    //
    // const CubismMatrix44    saveProjection = projection;
    // csmUint32 modelCount = _models.GetSize();
    // for (csmUint32 i = 0; i < modelCount; ++i)
    // {
    //     LAppModel* model = GetModel(i);
    //     projection = saveProjection;
    //
    //     model->Update();
    //     model->Draw(projection);///< 参照渡しなのでprojectionは変質する
    // }
}