DEPRECATED
==========

RenPy has native Live2D support now. `Use it. <https://www.renpy.org/doc/html/live2d.html>`_

Introduction
============

This is attempt to create Live2D module for RenPy. This is WIP and not production version. Project has no ETA. This source code is invitation to developers that are willing to help with development of module.

This module works only with OpenGL renderer.

Installing
==========

As for now prebuild version of this module available only for macOS and Windows.

1. Go to the `'release' section <https://github.com/asfdfdfd/renpy-live2d/releases>`_ and download latest release.

2. Extract files and place 'live2d' folder to the 'game' folder of your RenPy project.

3. Download `Cubism Native SDK <https://live2d.github.io/index.html#native>`_ and replace 'CubismSDK/Core' folder of this library with 'Core' folder from downloaded SDK.

4. You are ready to go.

Usage example
=============

.. code:: renpy

    define h = Character("Hiyori")

    init python:
        from live2d.displayable import Live2DDisplayable
        from renpy.loader import transfn
    
        sprite_live2d = Live2DDisplayable()
    
    init:
        image sprite_live2d = sprite_live2d
    
    label start:
    
        show sprite_live2d
            
        $ live2d_model_hiyori = sprite_live2d.scene.create_model(transfn(u'live2d_resources/Hiyori/'), u'Hiyori.model3.json')
    
        $ live2d_model_hiyori.start_random_motion(group = u"Idle", priority = 3)
    
        h "You've created a new Ren'Py game."

        $ live2d_model_hiyori.start_motion(group = u"TapBody", no = 0, priority = 3)

        h "Once you add a story, pictures, and music, you can release it to the world!"

        return

Building
========

This section is for those who wants to participate in extension development. If you do not want to participate in development and just want to use your Live2D models in RenPy — read 'Installing' section.

macOS
-----

Extensions for RenPy should be built with custmoized Python 2.7.10 version that is configured for RenPy. The most convenient way is to use pyenv.

1. Install pyenv.

.. code:: shell
   
    brew install pyenv
   
If you want want to integrate pyenv into your shell read `'installation' section <https://github.com/pyenv/pyenv#installation>`_ in pyenv manual. I assume that you want to use pyenv just to build module and do not want to make deep integration.

2. Install python.

.. code:: shell

    PYTHON_CONFIGURE_OPTS="--enable-unicode=ucs4 --enable-shared" pyenv install 2.7.10    

If it fails with error read `pyenv common build problems <https://github.com/pyenv/pyenv/wiki/common-build-problems>`_.

If you need to have multiple Python 2.7.10 installs use `pyenv-alias plugin <https://github.com/s1341/pyenv-alias>`_.

3. Create folder to store python virtual environment. It will be mentioned as %TARGET_DIR%.
    
4. Create virtual environment and install required packages.

.. code:: shell

    cd %TARGET_DIR%
    pyenv local 2.7.10
    $(pyenv root)/versions/2.7.10/bin/pip install --upgrade pip
    $(pyenv root)/versions/2.7.10/bin/pip install virtualenv
    $(pyenv root)/versions/2.7.10/bin/virtualenv .
    bin/pip install pyasn1==0.1.7 rsa==3.1.4 altgraph==0.12 macholib==1.7 cython==0.29.7
                             
Now you have special Python build that is suitable for building Live2D module for RenPy:

1. Download `Cubism Native SDK <https://live2d.github.io/index.html#native>`_ and replace 'CubismSDK/Core' folder of this library with 'Core' folder from downloaded SDK.

2. Open console and go to the folder where you placed content of this repository

3. Launch 

.. code:: shell

  %TARGET_DIR%/bin/python setup.py build_ext --inplace
      
4. Module is ready. Now you could launch RenPy game.

Linux
-----

Should be similar to macOS but i could not confirm.

Windows
-------

1. Download and install `Microsoft Visual C++ Compiler for Python 2.7 <https://www.microsoft.com/en-us/download/details.aspx?id=44266>`_

2. Download and install x86 version of `Python 2.7.10  <https://www.python.org/ftp/python/2.7.10/python-2.7.10.msi>`_

3. Download `Cubism Native SDK <https://live2d.github.io/index.html#native>`_ and replace 'CubismSDK/Core' folder of this library with 'Core' folder from downloaded SDK.

4. Launch 'Visual C++ 2008 32-bit Command Prompt' from 'Start' menu and go to 'CubismSDK/Core/dll/windows/x86' folder.

5. Launch

.. code:: shell

    dumpbin /EXPORTS Live2DCubismCore.dll > Live2DCubismCore.exports

6. Edit 'Live2DCubismCore.exports' to create 'Live2DCubismCore.def' file. Also you could get ready 'def' file `here <https://gist.github.com/asfdfdfd/e20835ed92bd245e258d8a1c1b2f77ac>`_ but it may be a bit outdated so i recommend to create it by yourself.

7. Launch
    
.. code:: shell

    lib /def:Live2DCubismCore.def /out:Live2DCubismCore.lib

8. Go to root module folder and launch

.. code:: shell

    %PATH_TO_PYTHON%\python.exe setup.py build_ext --inplace --compiler=msvc
        
9. Module is ready. Now you could launch RenPy game.
