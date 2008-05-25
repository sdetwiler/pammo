import wx

import MaterialBrowser
import MaterialLibrary

class MaterialToolPanel(wx.Panel):
    def __init__(self, parent, id):
        wx.Panel.__init__(self, parent, id)
        self.browser = MaterialBrowser.MaterialBrowser(self, -1)

        self.drawGrid = False
        self.editor = None

        gridButton = wx.CheckBox(self, -1, "Draw Grid")
        gridButton.SetValue(self.drawGrid)
        self.Bind(wx.EVT_CHECKBOX, self.onToggleGrid, gridButton)

        button = wx.Button(self, 10, "Reload Materials");
        self.Bind(wx.EVT_BUTTON, self.onReloadMaterials, button)
        button.SetSize(button.GetBestSize())

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.browser, 1, wx.EXPAND)
        sizer.Add(gridButton, 0, wx.ALIGN_CENTER | wx.ALL, 2)
        sizer.Add(button, 0, wx.ALIGN_CENTER | wx.ALL, 2)
        self.SetSizer(sizer)

    def attachToEditor(self, editor):
        self.editor = editor
        editor.getDisplay().addMouseObserver(self.onMapMouseEvent)
        editor.getDisplay().addDrawObserver(self.onMapDraw)

    def detachFromEditor(self, editor):
        self.editor = None
        editor.getDisplay().removeMouseObserver(self.onMapMouseEvent)
        editor.getDisplay().removeDrawObserver(self.onMapDraw)

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

    def onToggleGrid(self, event):
        self.drawGrid = event.IsChecked()
        if self.editor: self.editor.Refresh()

    def onMapDraw(self, display, gc):
        if not self.drawGrid: return

        (sizeX, sizeY) = display.getMap().getProperties().getSize()
        tileSize = MaterialLibrary.getMaterialSize()

        gc.SetPen(wx.Pen(wx.Color(0, 0, 0, 168), 3))
        for x in range(sizeX+1):
            gc.StrokeLine(x*tileSize, 0, x*tileSize, sizeY*tileSize)
        for y in range(sizeY+1):
            gc.StrokeLine(0, y*tileSize, sizeX*tileSize, y*tileSize)
        
        
