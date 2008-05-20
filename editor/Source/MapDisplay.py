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

        self.mouseObservers = []
        self.drawObservers = []

        self.drawGrid = True
        self.drawScale = 1
        self.map = map
        self.map.addObserver(self.onMapChanged)

        self.updateVirtualSize()

    def getMap(self): return self.map

    def addMouseObserver(self, observer): self.mouseObservers.append(observer)
    def removeMouseObserver(self, observer):
        self.mouseObservers.remove(observer)

    def addDrawObserver(self, observer): self.drawObservers.append(observer)
    def removeDrawObserver(self, observer): self.drawObservers.remove(observer)

    def getDrawGrid(self): return self.drawGrid

    def setDrawGrid(self, drawGrid):
        self.drawGrid = drawGrid
        self.Refresh()

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

        gc.PushState()
        gc.Scale(self.drawScale, self.drawScale)
        
        (sizeX, sizeY) = self.map.getProperties().getSize()
        tileSize = MaterialLibrary.getMaterialSize()
        for y in range(sizeY):
            for x in range(sizeX):
                materialTile = MaterialLibrary.getMaterial(self.map.getMaterialTile(x, y))
                if not materialTile: continue
                gc.DrawBitmap(materialTile.getBitmap(), x*tileSize, y*tileSize, tileSize, tileSize)

        if self.drawGrid:
            gc.SetPen(wx.Pen("Black", 3))
            for x in range(sizeX+1):
                gc.StrokeLine(x*tileSize, 0, x*tileSize, sizeY*tileSize)
            for y in range(sizeY+1):
                gc.StrokeLine(0, y*tileSize, sizeX*tileSize, y*tileSize)

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

        gc.PopState()

        dc.EndDrawing()
