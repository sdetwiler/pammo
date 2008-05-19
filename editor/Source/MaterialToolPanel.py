import wx

import MaterialBrowser
import MaterialLibrary

class MaterialToolPanel(wx.Panel):
    def __init__(self, parent, id):
        wx.Panel.__init__(self, parent, id)
        self.browser = MaterialBrowser.MaterialBrowser(self, -1)

        button = wx.Button(self, 10, "Reload Materials");
        self.Bind(wx.EVT_BUTTON, self.onReloadMaterials, button)
        button.SetSize(button.GetBestSize())

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.browser, 1, wx.EXPAND)
        sizer.Add(button, 0, wx.CENTER)
        self.SetSizer(sizer)

    def attachToEditor(self, editor):
        editor.getDisplay().addMouseObserver(self.onMapMouseEvent)

    def detachFromEditor(self, editor):
        editor.getDisplay().removeMouseObserver(self.onMapMouseEvent)

    def onReloadMaterials(self, event):
        MaterialLibrary.reloadFromFilesystem()

    def onMapMouseEvent(self, display, event):
        if not event.LeftIsDown(): return
        
        tileSize = MaterialLibrary.getMaterialSize()
        x, y = display.calcMapLocationFromScreen(event.GetX(), event.GetY())
        tx, ty = int(x / tileSize), int(y / tileSize)
        if tx < 0 or ty < 0: return
        if tx > display.getMap().getProperties().getSizeX() - 1: return
        if ty > display.getMap().getProperties().getSizeY() - 1: return

        display.getMap().setMaterialTile(tx, ty, self.browser.getSelectedMaterial().getName())
        
