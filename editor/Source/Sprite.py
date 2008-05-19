import wx

import os, os.path

class Sprite:
    def __init__(self, name):
        self.name = name
        path = os.getcwd() + '/' + name + '.png'
        self.bitmap = wx.Image(path, wx.BITMAP_TYPE_PNG).ConvertToBitmap()
        
    def getName(self):
        return self.name

    def getBitmap(self):
        return self.bitmap

def pathToName(path):
    path = os.path.abspath(path)
    if path[-4:] != '.png': return None
    if path.find(os.getcwd()) == -1: return None
    return path[len(os.getcwd()) + 1:-4]
