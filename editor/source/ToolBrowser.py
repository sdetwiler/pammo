import wx

import PropToolPanel
import MaterialToolPanel
import CollisionToolPanel
import POIToolPanel

class ToolBrowser(wx.Panel):
    def __init__(self, parent, id):
        wx.Panel.__init__(self, parent, id)

        self.notebook = wx.Notebook(self, -1)
        self.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGED, self.onPageChanged, self.notebook)
        sizer = wx.BoxSizer(wx.HORIZONTAL)
        sizer.Add(self.notebook, 1, wx.EXPAND)
        self.SetSizer(sizer)

        self.observers = []
        self.selectedTool = None
        self.selectedEditor = None

        self.propToolPanel = PropToolPanel.PropToolPanel(self.notebook, -1)
        self.notebook.AddPage(self.propToolPanel, "Props")

        self.materialToolPanel = MaterialToolPanel.MaterialToolPanel(self.notebook, -1)
        #self.notebook.AddPage(self.materialToolPanel, "Materials")
        self.notebook.AddPage(self.materialToolPanel, "Mats")

        self.collisionToolPanel = CollisionToolPanel.CollisionToolPanel(self.notebook, -1)
        #self.notebook.AddPage(self.collisionToolPanel, "Collision")
        self.notebook.AddPage(self.collisionToolPanel, "Col")

        self.poiToolPanel = POIToolPanel.POIToolPanel(self.notebook, -1)
        self.notebook.AddPage(self.poiToolPanel, "POI")

    def addObserver(self, observer): self.observers.append(observer)
    def removeObserver(self, observer): self.observers.remove(observer)

    def onPageChanged(self, event):
        if self.selectedEditor and self.selectedTool:
            self.selectedTool.detachFromEditor(self.selectedEditor)
        self.selectedTool = self.notebook.GetPage(self.notebook.GetSelection())
        if self.selectedEditor:
            self.selectedTool.attachToEditor(self.selectedEditor)
            self.selectedEditor.Refresh()

    def onEditorChanged(self, editor):
        if self.selectedEditor:
            self.selectedTool.detachFromEditor(self.selectedEditor)
        self.selectedEditor = editor
        if editor:
            self.selectedTool.attachToEditor(editor)

    def _notify(self):
        for observer in self.observers: observer(self)
