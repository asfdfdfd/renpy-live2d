from libcpp cimport bool
from libcpp.cast cimport reinterpret_cast
from libc.stdlib cimport malloc, free
from libc.stdio cimport FILE, fopen, fwrite, fscanf, fclose, fprintf, fseek, ftell, SEEK_END, rewind, fread

ctypedef unsigned char csmByte
ctypedef char csmChar
ctypedef int csmSizeInt
ctypedef void (*cfptr)(int)
ctypedef signed int csmInt32
ctypedef unsigned int csmUint32
ctypedef float csmFloat32

cdef extern from "Math/CubismMatrix44.hpp" namespace "Live2D::Cubism::Framework":
    cdef cppclass CubismMatrix44:
        void Scale(csmFloat32 x, csmFloat32 y)
        void MultiplyByMatrix(CubismMatrix44* m)
        void TranslateRelative(csmFloat32 x, csmFloat32 y)
        void Translate(csmFloat32 x, csmFloat32 y)
        void TranslateX(csmFloat32 x)
        void TranslateY(csmFloat32 y)
                
cdef extern from "Math/CubismViewMatrix.hpp" namespace "Live2D::Cubism::Framework":
    cdef cppclass CubismViewMatrix(CubismMatrix44):
        void Scale(csmFloat32 x, csmFloat32 y)
        void SetScreenRect(csmFloat32 left, csmFloat32 right, csmFloat32 bottom, csmFloat32 top)
        void SetMaxScale(csmFloat32 maxScale)
        void SetMinScale(csmFloat32 minScale)
        void SetMaxScreenRect(csmFloat32 left, csmFloat32 right, csmFloat32 bottom, csmFloat32 top)
        void AdjustTranslate(csmFloat32 x, csmFloat32 y)
        void AdjustScale(csmFloat32 cx, csmFloat32 cy, csmFloat32 scale)

cdef extern from "ICubismModelSetting.hpp" namespace "Live2D::Cubism::Framework":
    cdef cppclass ICubismModelSetting:
        pass
    
cdef extern from "CubismModelSettingJson.hpp" namespace "Live2D::Cubism::Framework":
    cdef cppclass CubismModelSettingJson(ICubismModelSetting):
        CubismModelSettingJson(const csmByte*, csmSizeInt)
    
        const csmChar* GetModelFileName()
        
cdef extern from "CubismFramework.hpp" namespace "Live2D::Cubism::Framework::CubismFramework":
    cdef cppclass Option:
        pass
        
cdef extern from "CubismFramework.hpp" namespace "Live2D::Cubism::Framework":
    cdef cppclass ICubismAllocator:
        pass
        
    cdef cppclass CubismFramework:
        @staticmethod
        void StartUp(ICubismAllocator* allocator, Option* option)
        @staticmethod
        void Initialize()
        @staticmethod
        bool IsInitialized()
        @staticmethod
        bool IsStarted()        
        @staticmethod
        void Dispose()


cdef extern from "Rendering/CubismRenderer.hpp" namespace "Live2D::Cubism::Framework::Rendering":
    cdef cppclass CubismRenderer:
        pass
            

cdef extern from "Rendering/OpenGL/CubismRenderer_OpenGLES2.hpp" namespace "Live2D::Cubism::Framework::Rendering":
    cdef cppclass CubismRenderer_OpenGLES2(CubismRenderer):
        pass            
                        
cdef extern from "Model/CubismUserModel.hpp" namespace "Live2D::Cubism::Framework":        
    cdef cppclass CubismUserModel:
        CubismUserModel()

        void CreateRenderer()
        void LoadModel(const csmByte*, csmSizeInt)
        T* GetRenderer[T]()

cdef extern from "LAppAllocator.hpp":
    cdef cppclass LAppAllocator(ICubismAllocator):
        pass
                    
cdef extern from "LAppModel.hpp":
    cdef cppclass LAppModel(CubismUserModel):
        void LoadAssets(const char* dir, const char* filename)
        void Draw(CubismMatrix44& matrix)
        void Update()
        void SetRandomExpression()

cdef extern from "LAppDelegate.hpp":
    cdef cppclass LAppDelegate:
        @staticmethod
        LAppDelegate* GetInstance()
        void Initialize()

cdef extern from "LAppScene.hpp":
    cdef cppclass LAppScene:
        LAppModel* CreateModel(const char* dir, const char* filename)    
        void ReleaseModel(LAppModel* model)
        
        void Initialize(csmUint32 width, csmUint32 height)
        
        void Update()
        void Draw(csmUint32 stride, void* pixels)
                                
cdef class PyLAppScene:
    cdef LAppScene* thisptr

    def __cinit__(self):
        self.thisptr = new LAppScene()

    def initialize(self, width, height):
        self.thisptr.Initialize(width, height)

    def create_model(self, unicode dir, unicode filename):
        cdef LAppModel* model_ptr = self.thisptr.CreateModel(dir.encode("UTF-8"), filename.encode("UTF-8"))
        return PyLAppModel.create(model_ptr)
                    
    def release_model(self, PyLAppModel model):
        self.thisptr.ReleaseModel(model.thisptr)
                     
    def update(self):
        self.thisptr.Update()

    def draw(self, int stride, long pixels):
        self.thisptr.Draw(stride, <void*>pixels)
                               
cdef class PyCubismFramework:

    @staticmethod
    def startup():        
        CubismFramework.StartUp(new LAppAllocator(), NULL)
        
    @staticmethod
    def initialize():
        CubismFramework.Initialize()

    @staticmethod        
    def is_initialized():
        return CubismFramework.IsInitialized()

    @staticmethod        
    def is_started():
        return CubismFramework.IsStarted()
            
    @staticmethod           
    def dispose():
        CubismFramework.Dispose()
        
cdef class PyCubismRendererOpenGLES2:
    cdef CubismRenderer_OpenGLES2* thisptr;

    def __init__(self, *args):
        raise TypeError('Cannot create instance from Python')
        
    @staticmethod
    cdef create(CubismRenderer_OpenGLES2* ptr):
        # create instance without calling __init__
        obj = <PyCubismRendererOpenGLES2>PyCubismRendererOpenGLES2.__new__(PyCubismRendererOpenGLES2)
        obj.thisptr = ptr
        return obj
                        
cdef class PyCubismUserModel:
    cdef CubismUserModel* thisptr

    def __cinit__(self):
        self.thisptr = new CubismUserModel()

    def __dealloc__(self):
        #del self.thisptr
        pass
        
    def create_renderer(self):
        self.thisptr.CreateRenderer()
        
    def load_model(self, unicode path):
        cdef FILE* file = fopen(path.encode("UTF-8"), "rb")
        fseek(file, 0, SEEK_END)
        cdef size_t file_size = ftell(file)
        rewind(file)
        cdef void* buffer = malloc(file_size)
        fread(buffer, file_size, 1, file)
        fclose(file)
        self.thisptr.LoadModel(<const csmByte*>buffer, file_size)
        free(buffer)  
        
    def get_renderer(self):
        return PyCubismRendererOpenGLES2.create(self.thisptr.GetRenderer[CubismRenderer_OpenGLES2]())
        
cdef class PyLAppDelegate:    
    @staticmethod
    def initialize():
        LAppDelegate.GetInstance().Initialize()
        
cdef class PyLAppModel:
    cdef LAppModel* thisptr

    cdef CubismViewMatrix viewMatrix
        
    @staticmethod
    cdef create(LAppModel* model_ptr):
        model = PyLAppModel()
        model.thisptr = model_ptr
        return model;
        
    def __cinit__(self, *args, **kwargs):
        pass
        #if len(args) == 0:
        #    self.thisptr = new LAppModel()
        #else:            
        #    self.thisptr = args[0]
        #
        #ratio = 512.0 / 512.0
        #self.viewMatrix.SetScreenRect(-1, 1, -ratio, ratio)
        #self.viewMatrix.SetMaxScale(2.0)
        #self.viewMatrix.SetMinScale(0.8)
        #self.viewMatrix.SetMaxScreenRect(-2, 2, -2, 2)
    
    def __init__(self, *args, **kwargs):
        pass
    
    def load_assets(self, unicode dir, unicode filename):
        self.thisptr.LoadAssets(dir.encode("UTF-8"), filename.encode("UTF-8"))
                             
    def update(self):
        self.thisptr.Update()
        
    def draw(self):
        cdef CubismMatrix44 projection
        projection.MultiplyByMatrix(&self.viewMatrix)
        projection.Scale(1, 512.0/512.0)
        #projection.TranslateRelative(-1, 0.5)
        #projection.Scale(0.1, 0.1)
        self.thisptr.Draw(projection)
               
    def set_random_expression(self):
        self.thisptr.SetRandomExpression()
        
cdef class PyCubismModelSettingJson:
    cdef CubismModelSettingJson* thisptr;

    def __cinit__(self, unicode path):
        cdef FILE* file_settings = fopen(path.encode("UTF-8"), "rb")
        fseek(file_settings, 0, SEEK_END)
        cdef size_t file_size = ftell(file_settings)
        rewind(file_settings)
        cdef void* buffer_settings = malloc(file_size)
        fread(buffer_settings, file_size, 1, file_settings)
        fclose(file_settings)
        self.thisptr = new CubismModelSettingJson(<const csmByte*>buffer_settings, file_size)
        free(buffer_settings)

    def __dealloc__(self):
        del self.thisptr
        
    @property
    def model_file_name(self):
        return self.thisptr.GetModelFileName().decode("UTF-8")