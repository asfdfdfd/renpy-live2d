# Previously we had distutils here but this does not work on windows.
try:
    from setuptools import setup
    from setuptools import Extension
except ImportError:
    from distutils.core import setup
    from distutils.extension import Extension
    
from Cython.Build import cythonize
from Cython.Distutils import build_ext
import fnmatch
import os
import platform

CUBISM_FRAMEWORK_SRC = os.path.realpath(os.path.dirname(__file__)) + "/CubismSDK/Framework/src"
CUBISM_EXTERNAL_SRC = os.path.realpath(os.path.dirname(__file__)) + "/External"
CUBISM_SUPPORT_SRC = os.path.realpath(os.path.dirname(__file__)) + "/CubismSDK/Support"

def system_macros():
    if platform.system() == "Darwin":
        return "CSM_TARGET_MAC_GL"
    elif platform.system() == "Windows":
        return "CSM_TARGET_WIN_GL"
        
def library_dirs():        
    if platform.system() == "Darwin":
        return ['CubismSDK/Core/lib/macos']
    elif platform.system() == "Windows":
        return ['CubismSDK/Core/dll/windows/x86', 'glew/lib/Release/Win32']
        
def libraries():        
    if platform.system() == "Darwin":
        return ['Live2DCubismCore']
    elif platform.system() == "Windows":
        return ['Live2DCubismCore', "glew32", "opengl32"]

def extra_compile_args():
    if platform.system() == "Darwin":
        return ["-std=c++11"]
    elif platform.system() == "Windows":
        return []
        
def extra_link_args():
    if platform.system() == "Darwin":
        return ["-framework", "OpenGL"]
    elif platform.system() == "Windows":
        return []            
        
def include_dirs():
    if platform.system() == "Darwin":
        return []
    elif platform.system() == "Windows":
        return ["glew/include"]                
        
def make_ext(modname, pyxfilename):
    cubism_framework_cpp_files = recursive_glob(CUBISM_FRAMEWORK_SRC, "*.cpp")
    cubism_external_cpp_files = recursive_glob(CUBISM_EXTERNAL_SRC, "*.cpp")    
    cubism_support_cpp_files = recursive_glob(CUBISM_SUPPORT_SRC, "*.cpp")        
    from distutils.extension import Extension
    return Extension(modname,
            sources=[pyxfilename] + cubism_framework_cpp_files + cubism_external_cpp_files + cubism_support_cpp_files,
            include_dirs = ['CubismSDK/Core/include', 'CubismSDK/Framework/src', 'CubismSDK/Support'] + include_dirs(),
            library_dirs = library_dirs(),
            libraries = libraries(),
            extra_compile_args = extra_compile_args(),
            extra_link_args = extra_link_args(),
            define_macros = [(system_macros(), '1')],            
            language='c++')
            
def recursive_glob(search_path, filename_glob):
    matching_filenames = set()
    for root, dirs, files in os.walk(search_path):
        matching_filenames.update([os.path.join(root, filename) for filename in fnmatch.filter(files, filename_glob)])
    return list(matching_filenames)

setup(name='live2d',
      cmdclass = {'build_ext': build_ext},
      ext_modules=[make_ext("wrapper", "wrapper.pyx")])
