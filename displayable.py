import renpy.exports as renpy
import ctypes
from ctypes import util, c_float
from OpenGL.GL import *
from OpenGL.GL import shaders
from OpenGL.GLU import *
from live2d.wrapper import PyCubismUserModel, PyCubismModelSettingJson, PyLAppModel, PyLAppDelegate, PyLAppScene
import json
import os
import pygame
from renpy.loader import transfn

class Live2DDisplayable(renpy.Displayable):
        
    def __init__(self, **kwargs):
        super(Live2DDisplayable, self).__init__(**kwargs)
    
        # TODO. Move to the upper level to support multiple displayables.
        # TODO: Do not forget to call 'PyCubismFramework.dispose()'.        
        PyLAppDelegate.initialize()
    
        self.scene = PyLAppScene()
        self.scene_initialized = False
        
        self.render_width = 1024
        self.render_height = 512
        
    def render(self, width, height, st, at):     
        if self.scene_initialized == False:
            self.scene.initialize(self.render_width, self.render_height)
            # TODO: Get rid of transfn.
            self.model = self.scene.create_model(transfn('live2d_resources/Hiyori/'), u'Hiyori.model3.json')            
            self.scene_initialized = True
            
        self.scene.update()
                                  
        result = renpy.Render(self.render_width, self.render_height)
        surface = result.canvas().get_surface()
        surface.lock()
        self.scene.draw(surface.get_pitch() // surface.get_bytesize(), surface._pixels_address)        
        surface.unlock()
                                                                
        surface_rotated = pygame.transform.rotate(surface, 180)
        surface.blit(surface_rotated, (0, 0))
                            
        renpy.display.render.redraw(self, 0.06)
                
        return result
        