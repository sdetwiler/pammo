import wx
import os, os.path

class Material:
    def __init__(self, name):
        self.name = name
        path = os.path.abspath('../data/materials/' + name + '.png')
        self.bitmap = wx.Image(path, wx.BITMAP_TYPE_PNG).ConvertToBitmap()
        
    def getName(self):
        return self.name

    def getBitmap(self):
        return self.bitmap

def pathToName(path):
    base = os.path.abspath('../data/materials/')
    path = os.path.abspath(path)
    if path[-4:] != '.png': return None
    if path.find(base) == -1: return None
    return path[len(base)+1:-4]
