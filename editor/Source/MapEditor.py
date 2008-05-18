import wx

import MapDisplay
import MaterialLibrary

class MapEditor(wx.Panel):
    def __init__(self, parent, id, map):
        wx.Panel.__init__(self, parent, id)

        self.map = map
        self.map.addObserver(self.onMapChanged)

        self.tool = None
        self.toolActive = False

        self.display = MapDisplay.MapDisplay(self, -1, map)
        self.display.addMouseObserver(self.onDisplayMouseEvent)

        toolbar = wx.BoxSizer(wx.HORIZONTAL)
        gridButton = wx.CheckBox(self, -1, "Grid")
        gridButton.SetValue(self.display.getDrawGrid())
        self.Bind(wx.EVT_CHECKBOX, self.onToggleGrid, gridButton)
        toolbar.Add(gridButton, 0, wx.CENTER)

        tileButton = wx.CheckBox(self, -1, "Tiles")
        tileButton.SetValue(self.display.getDrawTiles())
        self.Bind(wx.EVT_CHECKBOX, self.onToggleTiles, tileButton)
        toolbar.Add(tileButton, 0, wx.CENTER)

        self.slider = wx.Slider(self, -1, 100, 25, 500, style=wx.SL_HORIZONTAL)
        self.Bind(wx.EVT_SCROLL, self.onChangeScale, self.slider)
        toolbar.Add(self.slider, 0, wx.CENTER)

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.display, 1, wx.EXPAND)
        sizer.Add(toolbar, 0, wx.EXPAND)
        self.SetSizer(sizer)

    def getMap(self): return self.map

    def onToolChanged(self, tool):
        self.tool = tool
    
    def onDisplayMouseEvent(self, display, event):
        if not self.tool: return

        if not event.LeftIsDown(): return
        #print event.Button
        #print event.ButtonDown()
        #print event.Moving()

        tileSize = MaterialLibrary.getMaterialSize()
        sizeX, sizeY = self.map.getProperties().getSize()
        realX, realY = self.display.CalcUnscrolledPosition(event.GetX(), event.GetY())
        tileX, tileY = realX / tileSize, realY / tileSize
        if tileX < 0 or tileX > sizeX-1 or tileY < 0 or tileY > sizeY-1: return
        if self.map.getMaterialTile(tileX, tileY) == self.tool.getName(): return

        self.map.setMaterialTile(tileX, tileY, self.tool.getName())
        self.display.Refresh()

    def onMapChanged(self, map):
        self.display.updateVirtualSize()

    def onToggleGrid(self, event):
        self.display.setDrawGrid(event.IsChecked())

    def onToggleTiles(self, event):
        self.display.setDrawTiles(event.IsChecked())

    def onChangeScale(self, event):
        self.display.setDrawScale(self.slider.GetValue() / 100.)

