import wx

import MapDisplay
import MaterialLibrary

import SpriteLibrary
import Sprite
import Entity

class MapEditor(wx.Panel, wx.FileDropTarget):
    def __init__(self, window, log):
        wx.FileDropTarget.__init__(self)
        self.window = window
        self.log = log

    def __init__(self, parent, id, map):
        wx.Panel.__init__(self, parent, id)
        wx.FileDropTarget.__init__(self)

        self.map = map
        self.map.addObserver(self.onMapChanged)

        self.display = MapDisplay.MapDisplay(self, -1, map)
        self.display.SetDropTarget(self)

        toolbar = wx.BoxSizer(wx.HORIZONTAL)
        gridButton = wx.CheckBox(self, -1, "Grid")
        gridButton.SetValue(self.display.getDrawGrid())
        self.Bind(wx.EVT_CHECKBOX, self.onToggleGrid, gridButton)
        toolbar.Add(gridButton, 0, wx.CENTER)

        self.slider = wx.Slider(self, -1, 100, 25, 500, size=(200, 20), style=wx.SL_HORIZONTAL)
        self.Bind(wx.EVT_SCROLL, self.onChangeScale, self.slider)
        toolbar.Add(self.slider, 0, wx.CENTER)

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.display, 1, wx.EXPAND)
        sizer.Add(toolbar, 0, wx.EXPAND)
        self.SetSizer(sizer)

    def getMap(self): return self.map

    def getDisplay(self): return self.display

    def OnDropFiles(self, x, y, filenames):
        for name in filenames:
            name = Sprite.pathToName(name)
            if not name: continue
            x, y = self.display.calcMapLocationFromScreen(x, y)
            #sprite = SpriteLibrary.getSprite(name)
            entity = Entity.Entity(name, (x, y), 1, 0)
            self.map.addEntity(entity)
        self.Refresh()

    def onMapChanged(self, map):
        self.display.updateVirtualSize()

    def onToggleGrid(self, event):
        self.display.setDrawGrid(event.IsChecked())

    def onChangeScale(self, event):
        self.display.setDrawScale(self.slider.GetValue() / 100.)

