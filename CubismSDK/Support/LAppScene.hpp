#pragma once

#include <set>

#include <CubismFramework.hpp>
#include <Model/CubismUserModel.hpp>
#include <ICubismModelSetting.hpp>
#include <Type/csmRectF.hpp>

class LAppModel;

class LAppScene
{
public:
    
    LAppScene();
    ~LAppScene();
    
    void Initialize(const Csm::csmUint32& width, const Csm::csmUint32& height);
    
    LAppModel* CreateModel(const Csm::csmChar* dir, const Csm::csmChar* fileName);
    void ReleaseModel(LAppModel* model);
    
    void Update();    
    void Draw(Csm::CubismMatrix44& matrix);
    
private:
        
    Csm::csmUint32 _width;
    Csm::csmUint32 _height;

    Csm::csmUint32 _renderTextureId;
    Csm::csmUint32 _framebufferId;

    Csm::CubismMatrix44* _viewMatrix;
        
    std::set<LAppModel*> _models;
};