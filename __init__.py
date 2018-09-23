# coding=utf-8

import os
import sys
import imp
from renpy.loader import transfn
import renpy
import platform

if platform.system() == "Windows":
    os.environ['PATH'] = transfn("live2d/") + os.pathsep + os.environ['PATH']
            
class Live2DImporter(object):
    
    def find_module(self, fullname, path=None):
        return self if fullname == "live2d.wrapper" else None

    def load_module(self, fullname):       
        platform_system = platform.system() 
        if platform_system == "Darwin" or platform_system == "Linux":
            return imp.load_dynamic("wrapper", transfn("live2d/wrapper.so"))
        elif platform_system == "Windows":
            os.environ['PATH'] = transfn("live2d/CubismSDK/Core/dll/windows/x86") + os.pathsep + os.environ['PATH']
            return imp.load_dynamic("wrapper", transfn("live2d/wrapper.pyd"))
            
sys.meta_path.insert(0, Live2DImporter())        

from wrapper import *
