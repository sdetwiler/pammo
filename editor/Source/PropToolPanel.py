import wx
import NumValidator

class PropToolPanel(wx.Panel):
    def __init__(self, parent, id):
        wx.Panel.__init__(self, parent, id)

        sizer = wx.BoxSizer(wx.VERTICAL)

        self.snapButton = wx.CheckBox(self, -1, "Snap")
        self.snapAmount = wx.TextCtrl(self, -1, "16", validator = NumValidator.NumValidator())
        self.snapAmount.SetInitialSize((self.snapAmount.GetBestSize()[0]/2, self.snapAmount.GetBestSize()[1]))
        self.snapAmount.Enable(False)
        row = wx.BoxSizer(wx.HORIZONTAL)
        row.Add(self.snapButton, 1, wx.ALIGN_LEFT | wx.RIGHT, 5)
        row.Add(self.snapAmount, 0, wx.ALIGN_RIGHT)
        sizer.Add(row, 0, wx.EXPAND | wx.ALL, 5)

        line = wx.StaticLine(self, -1, style = wx.LI_HORIZONTAL)
        sizer.Add(line, 0, wx.EXPAND | wx.ALL, 5)

        self.posXField = wx.TextCtrl(self, -1, "", validator = NumValidator.NumValidator())
        self.posXField.SetInitialSize((self.posXField.GetBestSize()[0]/2, self.posXField.GetBestSize()[1]))
        self.Bind(wx.EVT_TEXT, self.onPosChanged, self.posXField)
        self.posYField = wx.TextCtrl(self, -1, "", validator = NumValidator.NumValidator())
        self.posYField.SetInitialSize((self.posYField.GetBestSize()[0]/2, self.posYField.GetBestSize()[1]))
        self.Bind(wx.EVT_TEXT, self.onPosChanged, self.posYField)
        row = wx.BoxSizer(wx.HORIZONTAL)
        row.Add(wx.StaticText(self, -1, "Pos:"), 1, wx.ALIGN_LEFT | wx.RIGHT, 5)
        row.Add(self.posXField, 0, wx.ALIGN_RIGHT)
        row.Add(wx.StaticText(self, -1, "x"), 0, wx.ALIGN_CENTER | wx.RIGHT | wx.LEFT, 2)
        row.Add(self.posYField, 0, wx.ALIGN_RIGHT)
        sizer.Add(row, 0, wx.EXPAND | wx.ALL, 5)

        self.scaleField = wx.TextCtrl(self, -1, "", validator = NumValidator.NumValidator())
        self.scaleField.SetInitialSize((self.scaleField.GetBestSize()[0]/2, self.scaleField.GetBestSize()[1]))
        self.Bind(wx.EVT_TEXT, self.onScaleChanged, self.scaleField)
        row = wx.BoxSizer(wx.HORIZONTAL)
        row.Add(wx.StaticText(self, -1, "Scale:"), 1, wx.ALIGN_LEFT | wx.RIGHT, 5)
        row.Add(self.scaleField, 0, wx.ALIGN_RIGHT)
        sizer.Add(row, 0, wx.EXPAND | wx.ALL, 5)

        self.rotField = wx.TextCtrl(self, -1, "", validator = NumValidator.NumValidator())
        self.rotField.SetInitialSize((self.rotField.GetBestSize()[0]/2, self.rotField.GetBestSize()[1]))
        self.Bind(wx.EVT_TEXT, self.onRotChanged, self.rotField)
        row = wx.BoxSizer(wx.HORIZONTAL)
        row.Add(wx.StaticText(self, -1, "Rot:"), 1, wx.ALIGN_LEFT | wx.RIGHT, 5)
        row.Add(self.rotField, 0, wx.ALIGN_RIGHT)
        sizer.Add(row, 0, wx.EXPAND | wx.ALL, 5)

        self.moveForwardButton = wx.Button(self, -1, "Move Forward")
        self.moveBackwardButton = wx.Button(self, -1, "Move Backward")
        self.Bind(wx.EVT_BUTTON, self.onMoveForwardButton, self.moveForwardButton)
        self.Bind(wx.EVT_BUTTON, self.onMoveBackwardButton, self.moveBackwardButton)
        sizer.Add(self.moveForwardButton, 0, wx.ALIGN_CENTER | wx.ALL, 5)
        sizer.Add(self.moveBackwardButton, 0, wx.ALIGN_CENTER | wx.ALL, 5)

        self.deleteButton = wx.Button(self, -1, "Delete")
        self.Bind(wx.EVT_BUTTON, self.onDeleteButton, self.deleteButton)
        sizer.Add(self.deleteButton, 0, wx.ALIGN_CENTER | wx.ALL, 5)

        self.SetSizerAndFit(sizer)
        self.editor = None
        self.tracking = None
        self.updateState()

    def onPosChanged(self, event):
        if not self.tracking: return
        try:
            posX = float(self.posXField.GetValue())
            posY = float(self.posYField.GetValue())
            self.tracking.removeObserver(self.onEntityChanged)
            self.tracking.setPos((posX, posY))
            self.tracking.addObserver(self.onEntityChanged)
        except: pass

    def onScaleChanged(self, event):
        if not self.tracking: return
        try:
            val = float(self.scaleField.GetValue())
            self.tracking.removeObserver(self.onEntityChanged)
            self.tracking.setScale(val)
            self.tracking.addObserver(self.onEntityChanged)
        except: pass

    def onRotChanged(self, event):
        if not self.tracking: return
        try:
            val = float(self.rotField.GetValue())
            self.tracking.removeObserver(self.onEntityChanged)
            self.tracking.setRot(val)
            self.tracking.addObserver(self.onEntityChanged)
        except: pass

    def onMoveForwardButton(self, event):
        pass

    def onMoveBackwardButton(self, event):
        pass

    def onDeleteButton(self, event):
        entity = self.tracking
        self.detachFromEntity(entity)
        self.editor.getMap().removeEntity(entity)

    def onEntityChanged(self, entity):
        self.updateState()

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
            # Push tracking to None, so that setting field values doesn't trigger setting entity values.
            t = self.tracking
            self.tracking = None
            self.posXField.SetValue(str(t.getPos()[0]))
            self.posYField.SetValue(str(t.getPos()[1]))
            self.scaleField.SetValue(str(t.getScale()))
            self.rotField.SetValue(str(t.getRot()))
            self.tracking = t
            self.posXField.Enable(True)
            self.posYField.Enable(True)
            self.scaleField.Enable(True)
            self.rotField.Enable(True)
            #self.moveForwardButton.Enable(True)
            #self.moveBackwardButton.Enable(True)
            self.deleteButton.Enable(True)
        else:
            self.posXField.SetValue("")
            self.posYField.SetValue("")
            self.scaleField.SetValue("")
            self.posXField.Enable(False)
            self.posYField.Enable(False)
            self.scaleField.Enable(False)
            self.rotField.Enable(False)
            self.moveForwardButton.Enable(False)
            self.moveBackwardButton.Enable(False)
            self.deleteButton.Enable(False)

    def attachToEntity(self, entity):
        assert(self.tracking == None)
        self.tracking = entity
        self.tracking.addObserver(self.onEntityChanged)
        self.updateState()

    def detachFromEntity(self, entity):
        assert(self.tracking == entity)
        self.tracking.removeObserver(self.onEntityChanged)
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
        


