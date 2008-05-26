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

        self.SetBackgroundStyle(wx.BG_STYLE_CUSTOM)

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
        wx, wy = wx / self.drawScale, wy / self.drawScale
        return wx, wy

    def onMapChanged(self, map):
        self.Refresh()

    def OnPaint(self, event):
        drawLeft = self.GetViewStart()[0] * self.GetScrollPixelsPerUnit()[0] / self.drawScale
        drawTop = self.GetViewStart()[1] * self.GetScrollPixelsPerUnit()[1] / self.drawScale
        drawRight = drawLeft + self.GetSize()[0] / self.drawScale
        drawBottom = drawTop + self.GetSize()[1] / self.drawScale
        drawRect = (drawLeft, drawTop, drawRight, drawBottom)

        dc = wx.AutoBufferedPaintDCFactory(self)
        dc.Clear()
        self.PrepareDC(dc)
        gc = wx.GraphicsContext.Create(dc)

        gc.Scale(self.drawScale, self.drawScale)
        
        (sizeX, sizeY) = self.map.getProperties().getSize()
        tileSize = MaterialLibrary.getMaterialSize()
        startX = int(drawLeft // tileSize)
        startY = int(drawTop // tileSize)
        endX = int(drawRight // tileSize + 1)
        endY = int(drawBottom // tileSize + 1)
        if endX > sizeX: endX = sizeX
        if endY > sizeY: endY = sizeY

        for y in range(startY, endY):
            for x in range(startX, endX):
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
            #gc.Rotate(rot)
            gc.Translate(-w/2, -h/2)
            gc.DrawBitmap(bitmap, 0, 0, w, h)
            gc.PopState()

        for observer in self.drawObservers: observer(self, gc, drawRect)
