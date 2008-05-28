import wx

import MapDisplay
import MaterialLibrary

import Prop
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

        self.slider = wx.Slider(self, -1, 100, 20, 180, style=wx.SL_HORIZONTAL | wx.SL_AUTOTICKS)
        self.slider.SetTickFreq(20, 0)
        self.Bind(wx.EVT_SCROLL, self.onChangeScale, self.slider)
        toolbar.Add(self.slider, 1, wx.ALIGN_CENTER | wx.RIGHT, 5)

        zoomButton = wx.Button(self, -1, "100%")
        self.Bind(wx.EVT_BUTTON, self.onZoomButton, zoomButton)
        toolbar.Add(zoomButton, 0, wx.ALIGN_CENTER | wx.RIGHT, 5)

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.display, 1, wx.EXPAND)
        sizer.Add(toolbar, 0, wx.EXPAND | wx.ALL, 2)
        self.SetSizer(sizer)

    def getMap(self): return self.map

    def getDisplay(self): return self.display

    def OnDropFiles(self, x, y, filenames):
        for name in filenames:
            name = Prop.pathToName(name)
            if not name: continue
            x, y = self.display.calcMapLocationFromScreen(x, y)
            entity = Entity.Entity(name, (x, y), 1, 0)
            self.map.addEntity(entity)
        self.Refresh()

    def onMapChanged(self, map):
        self.display.updateVirtualSize()

    def onChangeScale(self, event):
        self.display.setDrawScale(self.slider.GetValue() / 100.)

    def onZoomButton(self, event):
        self.display.setDrawScale(1)
        self.slider.SetValue(100)

