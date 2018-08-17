Introduction
============

This is attempt to create Live2D module for RenPy. This is WIP and not production version. Project has no ETA. This source code is invitation to developers that are willing to help with development of module.

Windows version is not ready yet.

This module works only with OpenGL renderer.

Usage example
=============

.. code:: renpy

    define e = Character("Eileen")

    init python:
        from live2d.displayable import Live2DDisplayable
        from renpy.loader import transfn
    
    init:
        image sprite_live2d = Live2DDisplayable(transfn(u'live2d_resources/Hiyori/'), u'Hiyori.model3.json')
    
    label start:

        show sprite_live2d
    
        e "You've created a new Ren'Py game."

        e "Once you add a story, pictures, and music, you can release it to the world!"

        return

Building
========

macOS
-----

Extensions for RenPy should be built with special Python version that is configured for RenPy:

1. Download `renpy-deps <https://github.com/renpy/renpy-deps>`_ repository.

2. Open 'build_python.sh' script and apply patch:

   .. code:: diff
   
       diff --git a/build_python.sh b/build_python.sh
       index ca76c4d..274d03e 100755
       --- a/build_python.sh
       +++ b/build_python.sh
       @@ -7,7 +7,7 @@ INSTALL=$PWD/install

        # The xes are required to prevent msys from interpreting these as
        # paths. (We use the system python to do this normalization.)
       -SOURCE=`python $SOURCE/norm_source.py "x$PWD" "x$SOURCE"`
       +SOURCE=`python2 $SOURCE/norm_source.py "x$PWD" "x$SOURCE"`

        export LD_LIBRARY_PATH="$INSTALL/lib"
        export DYLIB_LIBRARY_PATH="$INSTALL/lib"

3. Run script and build Python.

4. Download Cython source code and install it via launching 'setup.py' script with fresh Python build:

   .. code:: shell
        
        %PATH_TO_PYTHON_BUILD%/python setup.py install
        
Now you have special Python build that is suitable for building Live2D module for RenPy:

1. Download `Cubism Native SDK <https://live2d.github.io/index.html#native>`_ and replace 'CubismSDK/Core' folder of this library with 'Core' folder from downloaded SDK.

2. Open console and go to the folder where you placed content of this repository

3. Launch 

   .. code:: shell
   
      %PATH_TO_PYTHON_BUILD%/python setup.py build_ext --inplace
      
4. Module is ready. Now you could launch RenPy game.

Linux
-----

Should be similar to macOS but i could not confirm.

Windows
-------

Windows has no support yet because Live2D has not prebuilt version of Core for MSVC 9. I'm waiting response from Live2D and thinking about wrapper with dynamic loading of methods from DLL if Live2D will not response.

Installing
==========

Create 'live2d' folder inside 'game' folder of your project. And copy content of repository to this folder. Then follow 'Building' section. Prebuilt modules will be provided in the future.