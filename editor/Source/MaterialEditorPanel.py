import wx

import MaterialBrowser
import MaterialLibrary

class MaterialEditorPanel(wx.Panel):
    def __init__(self, parent, id):
        wx.Panel.__init__(self, parent, id)
        self.browser = MaterialBrowser.MaterialBrowser(self, -1)

        button = wx.Button(self, 10, "Reload Materials");
        self.Bind(wx.EVT_BUTTON, self.onReloadMaterials, button)
        button.SetSize(button.GetBestSize())
        toolbar = wx.BoxSizer(wx.VERTICAL)
        toolbar.Add(button, 0, wx.ALIGN_CENTER)

        sizer = wx.BoxSizer(wx.HORIZONTAL)
        sizer.Add(self.browser, 0, wx.EXPAND)
        sizer.Add(toolbar, 1, wx.ALIGN_CENTER)
        self.SetSizer(sizer)

        menu1 = wx.Menu()
        self.menuBar = wx.MenuBar()
        self.menuBar.Append(menu1, "&MaterialEditor")

    def onPanelSelected(self, frame):
        frame.SetMenuBar(self.menuBar)

    def onReloadMaterials(self, event):
        MaterialLibrary.reloadFromFilesystem()
