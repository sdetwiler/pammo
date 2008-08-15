import wx
import osfix

class Backdrop:
    def __init__(self, name):
        self.name = name
        path = osfix.path('../data/backdrops/' + name + '.png')
        self.bitmap = wx.Image(path, wx.BITMAP_TYPE_PNG).ConvertToBitmap()
        
    def getName(self):
        return self.name

    def getBitmap(self):
        return self.bitmap

def pathToName(path):
    base = osfix.path('../data/backdrops/')
    path = osfix.path(path)
    if path[-4:] != '.png': return None
    if path.find(base) == -1: return None
    return path[len(base)+1:-4]
