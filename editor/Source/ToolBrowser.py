import wx

import PropToolPanel
import MaterialToolPanel

class ToolBrowser(wx.Notebook):
    def __init__(self, parent, id):
        wx.Notebook.__init__(self, parent, id)
        self.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGED, self.onPageChanged)

        self.observers = []
        self.selectedTool = None
        self.selectedEditor = None

        self.materialToolPanel = MaterialToolPanel.MaterialToolPanel(self, -1)
        self.AddPage(self.materialToolPanel, "Materials")

        self.propToolPanel = PropToolPanel.PropToolPanel(self, -1)
        self.AddPage(self.propToolPanel, "Props")

    def addObserver(self, observer): self.observers.append(observer)
    def removeObserver(self, observer): self.observers.remove(observer)

    def onPageChanged(self, event):
        if self.selectedEditor and self.selectedTool:
            self.selectedTool.detachFromEditor(self.selectedEditor)
        self.selectedTool = self.GetPage(event.GetSelection())
        if self.selectedEditor:
            self.selectedTool.attachToEditor(self.selectedEditor)
        event.Skip()

    def onEditorChanged(self, editor):
        if self.selectedEditor:
            self.selectedTool.detachFromEditor(self.selectedEditor)
        self.selectedEditor = editor
        self.selectedTool.attachToEditor(editor)

    def _notify(self):
        for observer in self.observers: observer(self)
