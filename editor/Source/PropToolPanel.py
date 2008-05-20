import wx

class PropToolPanel(wx.Panel):
    def __init__(self, parent, id):
        wx.Panel.__init__(self, parent, id)

        border = 5
        sizer = wx.BoxSizer(wx.VERTICAL)

        self.deleteButton = wx.Button(self, -1, "Delete")
        self.Bind(wx.EVT_BUTTON, self.onDeleteButton, self.deleteButton)
        sizer.Add(self.deleteButton, 0, wx.ALIGN_CENTER)

        #self.propNameField = wx.TextCtrl(self, -1, "")
        #self.fileNameField = wx.TextCtrl(self, -1, "")
        #self.fileResolutionField = wx.StaticText(self, -1, "128x128")
        #self.fileSizeField = wx.StaticText(self, -1, "4mb")
        #self.posFieldX = wx.TextCtrl(self, -1, "10")
        #self.posFieldY = wx.TextCtrl(self, -1, "10")
        #self.scaleField = wx.TextCtrl(self, -1, "0")
        #self.rotationField = wx.TextCtrl(self, -1, "0")

        #box = wx.BoxSizer(wx.HORIZONTAL)
        #box.Add(wx.StaticText(self, -1, "Prop Name:"), 0, wx.ALIGN_CENTER | wx.ALL, border)
        #box.Add(self.propNameField, 1, wx.ALIGN_CENTER | wx.ALL, border)
        #sizer.Add(box, 0, wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL, border)

        #box = wx.BoxSizer(wx.HORIZONTAL)
        #box.Add(wx.StaticText(self, -1, "Position:"), 0, wx.ALIGN_CENTER | wx.ALL, border)
        #box.Add(self.posFieldX, 1, wx.ALIGN_CENTER | wx.ALL, border)
        #box.Add(wx.StaticText(self, -1, "x"), 0, wx.ALIGN_CENTER | wx.ALL, border)
        #box.Add(self.posFieldY, 1, wx.ALIGN_CENTER | wx.ALL, border)
        #sizer.Add(box, 0, wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL, border)

        #box = wx.BoxSizer(wx.HORIZONTAL)
        #box.Add(wx.StaticText(self, -1, "Scale:"), 0, wx.ALIGN_CENTER | wx.ALL, border)
        #box.Add(self.scaleField, 1, wx.ALIGN_CENTER | wx.ALL, border)
        #sizer.Add(box, 0, wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL, border)

        #box = wx.BoxSizer(wx.HORIZONTAL)
        #box.Add(wx.StaticText(self, -1, "Rotation:"), 0, wx.ALIGN_CENTER | wx.ALL, border)
        #box.Add(self.rotationField, 1, wx.ALIGN_CENTER | wx.ALL, border)
        #sizer.Add(box, 0, wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL, border)

        self.SetSizerAndFit(sizer)
        self.editor = None
        self.tracking = None
        self.updateState()

    def onDeleteButton(self, event):
        entity = self.tracking
        self.detachFromEntity(entity)
        self.editor.getMap().removeEntity(entity)

    def attachToEditor(self, editor):
        assert(self.editor == None)
        self.editor = editor
        self.editor.getDisplay().addMouseObserver(self.onMapMouseEvent)
        self.editor.getDisplay().addDrawObserver(self.onMapDraw)

    def detachFromEditor(self, editor):
        assert(editor == self.editor)
        editor.getDisplay().removeMouseObserver(self.onMapMouseEvent)
        editor.getDisplay().removeDrawObserver(self.onMapDraw)
        if self.tracking: self.detachFromEntity(self.tracking)
        self.editor = None

    def updateState(self):
        if self.tracking:
            self.deleteButton.Enable(True)
        else:
            self.deleteButton.Enable(False)

    def attachToEntity(self, entity):
        assert(self.tracking == None)
        self.tracking = entity
        self.updateState()

    def detachFromEntity(self, entity):
        assert(self.tracking == entity)
        self.tracking = None
        self.updateState()
        self.editor.display.Refresh()

    def onMapMouseEvent(self, display, event):
        x, y = display.calcMapLocationFromScreen(event.GetX(), event.GetY())

        if event.LeftDown():
            hit = None
            self.last = (x, y)
            for e in display.getMap().getEntities():
                pos = e.getPos()
                scale = e.getScale()
                size = (e.getSprite().getBitmap().GetWidth(), e.getSprite().getBitmap().GetHeight())
                if x < pos[0] - size[0]/2*scale: continue
                if x > pos[0] + size[0]/2*scale: continue
                if y < pos[1] - size[1]/2*scale: continue
                if y > pos[1] + size[1]/2*scale: continue

                hit = e

            if self.tracking:
                self.detachFromEntity(self.tracking)

            if hit:
                self.attachToEntity(hit)
        
        if event.LeftIsDown() and self.tracking:
            pos = self.tracking.getPos()
            self.tracking.setPos((pos[0] + x - self.last[0], pos[1] + y - self.last[1]))
            self.last = (x, y)
            self.editor.Refresh()

    def onMapDraw(self, display, gc):
        if not self.tracking: return

        pos = self.tracking.getPos()
        scale = self.tracking.getScale()
        size = (self.tracking.getSprite().getBitmap().GetWidth(), self.tracking.getSprite().getBitmap().GetHeight())
        left = pos[0] - size[0]/2*scale
        right = pos[0] + size[0]/2*scale
        top = pos[1] - size[1]/2*scale
        bottom = pos[1] + size[1]/2*scale
        
        gc.SetPen(wx.Pen("Red", 3))
        gc.StrokeLine(left, top, right, top)
        gc.StrokeLine(left, bottom, right, bottom)
        gc.StrokeLine(left, top, left, bottom)
        gc.StrokeLine(right, top, right, bottom)
        


