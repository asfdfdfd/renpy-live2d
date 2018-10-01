import renpy.exports as renpy
from live2d.wrapper import PyCubismUserModel, PyCubismModelSettingJson, PyLAppModel, PyLAppDelegate, PyLAppScene
import json
import os
import pygame
from renpy.display.core import Interface

# TODO: Do not forget to call 'PyCubismFramework.dispose()'.
PyLAppDelegate.initialize()
    
class Live2DDisplayable(renpy.Displayable):
        
    def __init__(self, model_dir, model_file, **kwargs):
        super(Live2DDisplayable, self).__init__(**kwargs)
    
        self.scene = PyLAppScene()
        self.scene_initialized = False
        
        self.render_width = 1024
        self.render_height = 512
        
        self.model_dir = model_dir
        self.model_file = model_file
            
        interface_set_mode_old = Interface.set_mode

        self_displayable = self
        
        def interface_set_mode_new(self, physical_size=None):
            interface_set_mode_old(self, physical_size)
            self_displayable.initialize()

        Interface.set_mode = interface_set_mode_new

    def initialize(self):
        self.scene.initialize(self.render_width, self.render_height)
        
    def render(self, width, height, st, at):     
        if self.scene_initialized == False:
            self.model = self.scene.create_model(self.model_dir, self.model_file)                    
            self.scene_initialized = True
            
        self.scene.update()
                                  
        result = renpy.Render(self.render_width, self.render_height)
        surface = result.canvas().get_surface()
        surface.lock()
        self.scene.draw(surface.get_pitch() // surface.get_bytesize(), surface._pixels_address)        
        surface.unlock()
                                    
        renpy.display.render.redraw(self, 0.06)
                
        return result
        