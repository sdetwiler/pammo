import sys
import wx
import MapEditorPanel

class IrradiatedEditorFrame(wx.Frame):
    def __init__(self, parent, title):
        wx.Frame.__init__(self, parent, -1, title,
                          pos=(150, 150), size=(800, 600))

        self.Bind(wx.EVT_MENU, self.onCloseMenu, id=wx.ID_EXIT)
        self.Bind(wx.EVT_CLOSE, self.onTimeToQuit, self)

        self.Bind(wx.EVT_MENU_RANGE, self.onMenuItem, id=100, id2=200)

        #self.notebook = wx.Notebook(self, -1)
        #self.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGED, self.onPageChanged, self.notebook)

        self.mapEditor = MapEditorPanel.MapEditorPanel(self, -1)
        self.mapEditor.onPanelSelected(self)
        #self.notebook.AddPage(self.mapEditor, "Map Editor")

        #self.materialEditor = MaterialEditorPanel.MaterialEditorPanel(self.notebook, -1)
        #self.notebook.AddPage(self.materialEditor, "Material Editor")

        #self.templateEditor = TemplateEditorPanel.TemplateEditorPanel(self.notebook, -1)
        #self.notebook.AddPage(self.templateEditor, "Template Editor")

        #self.spriteEditor = SpriteEditorPanel.SpriteEditorPanel(self.notebook, -1)
        #self.notebook.AddPage(self.spriteEditor, "Sprite Editor")

    def askClose(self):
        if not self.mapEditor.onTimeToQuit(): return False
        return True

    def onCloseMenu(self, evt):
        if self.askClose(): self.Close()

    def onTimeToQuit(self, evt):
        if self.askClose(): evt.Skip()

    #def onPageChanged(self, event):
    #    self.notebook.GetPage(event.GetSelection()).onPanelSelected(self)

    def onMenuItem(self, event):
        #self.notebook.GetPage(self.notebook.GetSelection()).AddPendingEvent(event)
        self.mapEditor.AddPendingEvent(event)

class IrradiatedEditorApp(wx.App):
    def OnInit(self):
        frame = IrradiatedEditorFrame(None, "Irradiated Map Editor")
        self.SetTopWindow(frame)
        frame.Show(True)
        return True    

app = IrradiatedEditorApp(redirect=False)
app.MainLoop()
