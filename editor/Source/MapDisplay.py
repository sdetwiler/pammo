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

        self.drawGrid = True
        self.drawTiles = False
        self.drawScale = 1
        self.map = map

        self.updateVirtualSize()

    def addMouseObserver(self, observer): self.mouseObservers.append(observer)

    def removeMouseObserver(self, observer): self.mouseObservers.remove(observer)

    def getDrawGrid(self): return self.drawGrid

    def setDrawGrid(self, drawGrid):
        self.drawGrid = drawGrid
        self.Refresh()

    def getDrawTiles(self): return self.drawTiles

    def setDrawTiles(self, drawTiles):
        self.drawTiles = drawTiles
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

    def OnPaint(self, event):
        if self.drawTiles: self.OnPaintTiles()
        else: self.OnPaintMaterials()

    def OnPaintMaterials(self):
        dc = wx.PaintDC(self)
        self.PrepareDC(dc)
        dc.BeginDrawing()

        dc.SetUserScale(self.drawScale, self.drawScale)
        
        (sizeX, sizeY) = self.map.getProperties().getSize()
        tileSize = MaterialLibrary.getMaterialSize()
        for y in range(sizeY):
            for x in range(sizeX):
                materialTile = MaterialLibrary.getMaterial(self.map.getMaterialTile(x, y))
                if not materialTile: continue
                dc.DrawBitmap(materialTile.getBitmap(), x*tileSize - .0001, y*tileSize - .0001, True)

        if self.drawGrid:
            dc.SetPen(wx.Pen('BLACK', 3))
            for x in range(sizeX+1):
                dc.DrawLine(x*tileSize, 0, x*tileSize, sizeY*tileSize)
            for y in range(sizeY+1):
               dc.DrawLine(0, y*tileSize, sizeX*tileSize, y*tileSize)

        dc.SetUserScale(1, 1)

        dc.EndDrawing()

    def OnPaintTiles(self):
        dc = wx.PaintDC(self)
        self.PrepareDC(dc)
        dc.BeginDrawing()

        dc.SetUserScale(self.drawScale, self.drawScale)
        
        (sizeX, sizeY) = self.map.getProperties().getSize()
        tileSize = MaterialLibrary.getMaterialSize()
        for y in range(sizeY - 1):
            for x in range(sizeX - 1):
                m0 = self.map.getMaterialTile(x, y)
                m1 = self.map.getMaterialTile(x+1, y)
                m2 = self.map.getMaterialTile(x, y+1)
                m3 = self.map.getMaterialTile(x+1, y+1)
                bitmap = MaterialLibrary.getMaterialTransition(m0, m1, m2, m3)
                if not bitmap:
                    tile = MaterialLibrary.getMaterial(m0)
                    if not tile: continue
                    bitmap = tile.getBitmap()
                dc.DrawBitmap(bitmap, x*tileSize + tileSize/2, y*tileSize + tileSize/2, True)

        if self.drawGrid:
            dc.SetPen(wx.Pen('BLACK', 3))
            for x in range(sizeX):
                dc.DrawLine(x*tileSize + tileSize/2, tileSize/2, x*tileSize + tileSize/2, sizeY*tileSize - tileSize/2)
            for y in range(sizeY):
               dc.DrawLine(tileSize/2, y*tileSize + tileSize/2, sizeX*tileSize - tileSize/2, y*tileSize + tileSize/2)

        dc.SetUserScale(1, 1)

        dc.EndDrawing()

