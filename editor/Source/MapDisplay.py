import wx

import Map
import MaterialLibrary

class MapDisplay(wx.ScrolledWindow):
    def __init__(self, parent, id, map):
        wx.ScrolledWindow.__init__(self, parent, id, style=wx.BORDER_SUNKEN)
        self.SetScrollRate(5, 5)
        self.Bind(wx.EVT_LEFT_DOWN, self.onMouseEvent)
        self.Bind(wx.EVT_LEFT_UP, self.onMouseEvent)
        self.Bind(wx.EVT_MOTION, self.onMouseEvent)
        self.Bind(wx.EVT_PAINT, self.OnPaint)
        self.Bind(wx.EVT_WINDOW_DESTROY, self.onDestroy)

        self.mouseObservers = []
        self.drawObservers = []

        self.drawScale = 1
        self.map = map
        self.map.addObserver(self.onMapChanged)

        self.updateVirtualSize()

    def onDestroy(self, event):
        self.map.removeObserver(self.onMapChanged)

    def getMap(self): return self.map

    def addMouseObserver(self, observer): self.mouseObservers.append(observer)
    def removeMouseObserver(self, observer):
        self.mouseObservers.remove(observer)

    def addDrawObserver(self, observer): self.drawObservers.append(observer)
    def removeDrawObserver(self, observer): self.drawObservers.remove(observer)

    def getDrawScale(self): return self.drawScale
    
    def setDrawScale(self, drawScale):
        self.drawScale = drawScale
        self.updateVirtualSize()
        self.Refresh()

    def updateVirtualSize(self):
        (sizeX, sizeY) = self.map.getProperties().getSize()
        tileSize = MaterialLibrary.getMaterialSize()
        self.SetVirtualSize((sizeX*tileSize*self.drawScale, sizeY*tileSize*self.drawScale))

    def onMouseEvent(self, event):
        for observer in self.mouseObservers: observer(self, event)

    def calcMapLocationFromScreen(self, x, y):
        wx, wy = self.CalcUnscrolledPosition(x, y)
        wx, wy = wx / self.getDrawScale(), wy / self.getDrawScale()
        return wx, wy

    def onMapChanged(self, map):
        self.Refresh()

    def OnPaint(self, event):
        dc = wx.PaintDC(self)
        self.PrepareDC(dc)
        dc.BeginDrawing()
        gc = wx.GraphicsContext.Create(dc)

        #print dir(self)
        #print self.GetSize()
        #print self.GetViewStart()
        #buffer = wx.EmptyBitmap(self.GetSize()[0], self.GetSize()[1])
        #bufferDC = wx.BufferedDC(None, buffer)
        #self.PrepareDC(bufferDC)
        #bufferDC.BeginDrawing()
        #gc = wx.GraphicsContext.Create(bufferDC)

        gc.Scale(self.drawScale, self.drawScale)
        
        (sizeX, sizeY) = self.map.getProperties().getSize()
        tileSize = MaterialLibrary.getMaterialSize()
        for y in range(sizeY):
            for x in range(sizeX):
                materialTile = MaterialLibrary.getMaterial(self.map.getMaterialTile(x, y))
                if not materialTile: continue
                gc.DrawBitmap(materialTile.getBitmap(), x*tileSize, y*tileSize, tileSize, tileSize)

        for e in self.map.getEntities():
            s = e.getSprite()
            if not s: continue

            pos = e.getPos()
            scale = e.getScale()
            rot = e.getRot()
            bitmap = s.getBitmap()
            w = bitmap.GetWidth()
            h = bitmap.GetHeight()

            gc.PushState()
            gc.Translate(pos[0], pos[1])
            gc.Scale(scale, scale)
            gc.Rotate(rot)
            gc.Translate(-w/2, -h/2)
            gc.DrawBitmap(bitmap, 0, 0, w, h)
            gc.PopState()

        for observer in self.drawObservers: observer(self, gc)

        #bufferDC.EndDrawing()

        #dc = wx.PaintDC(self)
        #self.PrepareDC(dc)
        #dc.BeginDrawing()
        #dc.DrawBitmap(buffer, -self.GetViewStart()[0], -self.GetViewStart()[1], False)
        #dc.DrawBitmap(buffer, 0, 0, False)
        
        dc.EndDrawing()
