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

        fill = wx.Button(self, -1, "Fill with Material");
        self.Bind(wx.EVT_BUTTON, self.onFillMap, fill)
        fill.SetSize(fill.GetBestSize())

        reload = wx.Button(self, -1, "Reload Materials");
        self.Bind(wx.EVT_BUTTON, self.onReloadMaterials, reload)
        reload.SetSize(reload.GetBestSize())

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.browser, 1, wx.EXPAND)
        sizer.Add(gridButton, 0, wx.ALIGN_CENTER | wx.ALL, 2)
        sizer.Add(fill, 0, wx.ALIGN_CENTER | wx.ALL, 2)
        sizer.Add(reload, 0, wx.ALIGN_CENTER | wx.ALL, 2)
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

    def onFillMap(self, event):
        if not self.editor: return

        mat = self.browser.getSelectedMaterial().getName()
        sizeX, sizeY = self.editor.getMap().getProperties().getSize()
        for y in range(sizeY):
            for x in range(sizeX):
                self.editor.getMap().setMaterialTile(x, y, mat)

    def onMapDraw(self, display, gc, rect):
        if not self.drawGrid: return

        (sizeX, sizeY) = display.getMap().getProperties().getSize()
        tileSize = MaterialLibrary.getMaterialSize()
        startX = int(rect[0] // tileSize)
        startY = int(rect[1] // tileSize)
        endX = int(rect[2] // tileSize + 1)
        endY = int(rect[3] // tileSize + 1)
        if endX > sizeX: endX = sizeX
        if endY > sizeY: endY = sizeY

        gc.SetPen(wx.Pen(wx.Color(0, 0, 0, 168), 3))
        for x in range(startX, endX + 1):
            gc.StrokeLine(x*tileSize, 0, x*tileSize, sizeY*tileSize)
        for y in range(startY, endY + 1):
            gc.StrokeLine(0, y*tileSize, sizeX*tileSize, y*tileSize)
        
        
