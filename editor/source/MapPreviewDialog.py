import wx

import MapDisplay

class MapPreviewDialog(wx.Dialog):
    def __init__(self, parent, id, map):
        wx.Dialog.__init__(self, parent, id, "iPhone Preview")

        self.map = map
        self.display = MapDisplay.MapDisplay(self, -1, map)

        self.display.SetMinSize((480, 320))

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.display, 0, wx.ALIGN_CENTER)
        self.SetSizerAndFit(sizer)
        
