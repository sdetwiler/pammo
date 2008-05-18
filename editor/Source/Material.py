import wx

class Material:
    def __init__(self, path):
        self.name = path.split('/')[-1].split('.png')[0]
        self.bitmap = wx.Image(path, wx.BITMAP_TYPE_PNG).ConvertToBitmap()
        
    def getName(self):
        return self.name

    def getBitmap(self):
        return self.bitmap

def tryLoad(path):
    if path[-4:] != '.png':
        return None
    else:
        return Material(path)
