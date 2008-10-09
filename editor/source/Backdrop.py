import wx
import osfix
import hashlib

class Backdrop:
    def __init__(self, name):
        self.name = name
        path = osfix.path('../data/backdrops/' + name + '.png')
        self.bitmap = wx.Image(path, wx.BITMAP_TYPE_PNG).ConvertToBitmap()
        
        digest = hashlib.sha1()
        f = open(path, "rb")
        while True:
            buffer = f.read(100*1024)
            if len(buffer) == 0: break
            digest.update(buffer)
        f.close()
        self.hash = digest.hexdigest()
        print self.hash
        
    def getName(self):
        return self.name

    def getBitmap(self):
        return self.bitmap
    
    def getHash(self):
        return self.hash

def pathToName(path):
    base = osfix.path('../data/backdrops/')
    path = osfix.path(path)
    if path[-4:] != '.png': return None
    if path.find(base) == -1: return None
    return path[len(base)+1:-4]
