#include "LAppScene.hpp"

#include <string>
#include <Rendering/CubismRenderer.hpp>

#include "LAppPal.hpp"
#include "LAppDefine.hpp"
#include "LAppDelegate.hpp"
#include "LAppModel.hpp"

using namespace Csm;
using namespace LAppDefine;

LAppScene::LAppScene() : _viewMatrix(NULL)
{
}

LAppScene::~LAppScene() 
{
    std::set<LAppModel*>::iterator it;
    for (it = _models.begin(); it != _models.end(); ++it)
    {
        delete (*it);
    }
}

void LAppScene::Initialize(const Csm::csmUint32& width, const Csm::csmUint32& height) 
{
    _width = width;
    _height = height;
    
    // Create FBO render texture.
    
    glGenTextures(1, &_renderTextureId);
    glBindTexture(GL_TEXTURE_2D, _renderTextureId);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        
    // Create FBO.

    glGenFramebuffers(1, &_framebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebufferId);
    
    // Attach FBO texture.

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _renderTextureId, 0);
        
    // Cleanup.
    
    glBindTexture(GL_TEXTURE_2D, 0);        
    glBindFramebuffer(GL_FRAMEBUFFER, 0);        
}

LAppModel* LAppScene::CreateModel(const Csm::csmChar* dir, const Csm::csmChar* fileName) 
{
    LAppModel* model = new LAppModel();
    model->LoadAssets(dir, fileName);
    _models.insert(model);
    return model;
}

void LAppScene::ReleaseModel(LAppModel* model) 
{
    _models.erase(model);
    delete model;
}

void LAppScene::Update() 
{
    // TODO: Wrong place to update time. Will not work correctly with multiple scenes.
    LAppPal::UpdateTime();
    
    std::set<LAppModel*>::iterator it;
    for (it = _models.begin(); it != _models.end(); ++it)
    {
        (*it)->Update();
    }
}

void LAppScene::Draw(CubismMatrix44& matrix) 
{
    // Prepare for render.
    
    glBindFramebuffer(GL_FRAMEBUFFER, _framebufferId);            
    
    glUseProgram(0);
    
    glViewport(0, 0, _width, _height);
    
    glClearColor(0, 0, 0, 1);             
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0);
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
        
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
            
    glFrontFace(GL_CCW);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                                
    // LAppLive2DManager matrices operations.
    
    CubismMatrix44 projection;
    projection.Scale(1.0f, static_cast<float>(_width) / static_cast<float>(_height));

    if (_viewMatrix != NULL)
    {
        projection.MultiplyByMatrix(_viewMatrix);
    }

    // Render.
    
    const CubismMatrix44 saveProjection = projection;
    std::set<LAppModel*>::iterator it;
    for (it = _models.begin(); it != _models.end(); ++it)
    {
        LAppModel* model = (*it);
        projection = saveProjection;

        (*it)->Draw(projection);
    }
        
    // Read render result.
    
    // result = renpy.Render(1024, 1024)
    // surface = result.canvas().get_surface()
    // surface.lock()
    // glPixelStorei(GL_PACK_ROW_LENGTH, surface.get_pitch() // surface.get_bytesize())
    // glReadPixels(0, 0, 1024, 1024, GL_RGBA, GL_UNSIGNED_BYTE, surface._pixels_address)
    // glPixelStorei(GL_PACK_ROW_LENGTH, 0)
    // surface.unlock()
    //
    // surface_rotated = pygame.transform.rotate(surface, 180)
    // surface.blit(surface_rotated, (0, 0))
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);       
}