/*
* Copyright(c) Live2D Inc. All rights reserved.
*
* Use of this source code is governed by the Live2D Open Software license
* that can be found at http://live2d.com/eula/live2d-open-software-license-agreement_en.html.
*/

#pragma once

#include <string>
#include <Rendering/OpenGL/CubismRenderer_OpenGLES2.hpp>
#include "Type/csmVector.hpp"
#include "LAppAllocator.hpp"

class LAppView;
class LAppTextureManager;

/**
* @brief   アプリケーションクラス。
*   Cubism3の管理を行う。
*/
class LAppDelegate
{
public:
    /**
    * @brief   クラスのインスタンス（シングルトン）を返す。<br>
    *           インスタンスが生成されていない場合は内部でインスタンを生成する。
    *
    * @return  クラスのインスタンス
    */
    static LAppDelegate* GetInstance();

    /**
    * @brief   クラスのインスタンス（シングルトン）を解放する。
    *
    */
    static void ReleaseInstance();
    
    /**
    * @brief   APPに必要なものを初期化する。
    */
    bool Initialize();
    
    /**
    * @brief   解放する。
    */
    void Release();
    
    /**
    * @brief   実行処理。
    */
    // void ();

    /**
    * @brief　シェーダーを登録する。
    */
    GLuint CreateShader();

    /**
    * @brief   View情報を取得する。
    */
    LAppView* GetView() { return _view; }

    /**
    * @brief   アプリケーションを終了するかどうか。
    */
    bool GetIsEnd() { return _isEnd; }

    /**
    * @brief   アプリケーションを終了する。
    */
    void AppEnd() { _isEnd = true; }
        
    /**
     * @brief   テクスチャマネージャーを取得する。
     */
    LAppTextureManager* GetTextureManager() { return _textureManager; }

private:
    /**
    * @brief   コンストラクタ
    */
    LAppDelegate();

    /**
    * @brief   デストラクタ
    */
    ~LAppDelegate();

    /**
    * @brief   Cubism3の初期化
    */
    void InitializeCubism();
    
    /**
     * @brief   文字列を指定の文字で切り分ける
     */
    Csm::csmVector<std::string> Split(const std::string& baseString, char delim);

    LAppAllocator _cubismAllocator;              ///< Cubism3 Allocator
    Csm::CubismFramework::Option _cubismOption;  ///< Cubism3 Option
    LAppView* _view;                             ///< View情報
    bool _captured;                              ///< クリックしているか
    bool _isEnd;                                 ///< APP終了しているか
    LAppTextureManager* _textureManager;         ///< テクスチャマネージャー
};
