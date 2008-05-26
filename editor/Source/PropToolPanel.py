import wx
import NumValidator
import MaterialLibrary

class PropToolPanel(wx.Panel):
    def __init__(self, parent, id):
        wx.Panel.__init__(self, parent, id)

        self.editor = None
        self.tracking = None

        sizer = wx.BoxSizer(wx.VERTICAL)

        self.snapButton = wx.CheckBox(self, -1, "Snap")
        self.Bind(wx.EVT_CHECKBOX, self.onSnapChanged, self.snapButton)
        self.snapAmount = wx.TextCtrl(self, -1, "64", validator = NumValidator.NumValidator())
        self.snapAmount.Bind(wx.EVT_TEXT, self.onSnapChanged, self.snapAmount)
        self.snapAmount.SetInitialSize((self.snapAmount.GetBestSize()[0]/2, self.snapAmount.GetBestSize()[1]))
        row = wx.BoxSizer(wx.HORIZONTAL)
        row.Add(self.snapButton, 1, wx.ALIGN_LEFT | wx.ALIGN_BOTTOM | wx.RIGHT, 5)
        row.Add(self.snapAmount, 0, wx.ALIGN_RIGHT)
        sizer.Add(row, 0, wx.EXPAND | wx.ALL, 5)

        self.outlineButton = wx.CheckBox(self, -1, "Draw Outlines")
        self.Bind(wx.EVT_CHECKBOX, self.onOutlineChanged, self.outlineButton)
        sizer.Add(self.outlineButton, 0, wx.ALIGN_LEFT | wx.ALL, 5)

        self.selectModeButton = wx.CheckBox(self, -1, "Select by Transparency")
        sizer.Add(self.selectModeButton, 0, wx.ALIGN_LEFT | wx.ALL, 5)
        self.selectModeButton.SetValue(True)

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

        #self.rotField = wx.TextCtrl(self, -1, "", validator = NumValidator.NumValidator())
        #self.rotField.SetInitialSize((self.rotField.GetBestSize()[0]/2, self.rotField.GetBestSize()[1]))
        #self.Bind(wx.EVT_TEXT, self.onRotChanged, self.rotField)
        #row = wx.BoxSizer(wx.HORIZONTAL)
        #row.Add(wx.StaticText(self, -1, "Rot:"), 1, wx.ALIGN_LEFT | wx.RIGHT, 5)
        #row.Add(self.rotField, 0, wx.ALIGN_RIGHT)
        #sizer.Add(row, 0, wx.EXPAND | wx.ALL, 5)

        self.moveForwardButton = wx.Button(self, -1, "Move Forward")
        self.moveBackwardButton = wx.Button(self, -1, "Move Backward")
        self.Bind(wx.EVT_BUTTON, self.onMoveForwardButton, self.moveForwardButton)
        self.Bind(wx.EVT_BUTTON, self.onMoveBackwardButton, self.moveBackwardButton)
        sizer.Add(self.moveForwardButton, 0, wx.ALIGN_RIGHT | wx.ALL, 5)
        sizer.Add(self.moveBackwardButton, 0, wx.ALIGN_RIGHT | wx.ALL, 5)

        self.deleteButton = wx.Button(self, -1, "Delete")
        self.Bind(wx.EVT_BUTTON, self.onDeleteButton, self.deleteButton)
        sizer.Add(self.deleteButton, 0, wx.ALIGN_RIGHT | wx.ALL, 5)

        self.SetSizerAndFit(sizer)
        self.updateState()

    def onSnapChanged(self, event):
        if self.editor: self.editor.Refresh()

    def onOutlineChanged(self, event):
        if self.editor: self.editor.Refresh()

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

    def doPropsOverlap(self, p1, p2):
        pos1 = p1.getPos()
        scale1 = p1.getScale()
        size1 = (p1.getSprite().getBitmap().GetWidth()*scale1/2., p1.getSprite().getBitmap().GetHeight()*scale1/2.)

        pos2 = p2.getPos()
        scale2 = p2.getScale()
        size2 = (p2.getSprite().getBitmap().GetWidth()*scale2/2., p2.getSprite().getBitmap().GetHeight()*scale2/2.)

        left = pos1[0] - size1[0] - size2[0]
        right = pos1[0] + size1[0] + size2[0]
        top = pos1[1] - size1[1] - size2[1]
        bottom = pos1[1] + size1[1] + size2[1]

        if pos2[0] > left and pos2[0] < right and pos2[1] > top and pos2[1] < bottom:
            return True
        else:
            return False

    def onMoveForwardButton(self, event):
        entities = self.editor.getMap().getEntities()
        curIndex = entities.index(self.tracking)
        insertIndex = curIndex
        entities.remove(self.tracking)
        for i in range(len(entities) - curIndex):
            e = entities[i + curIndex]
            if self.doPropsOverlap(self.tracking, e):
                insertIndex = i + curIndex + 1
                break
        entities.insert(insertIndex, self.tracking)
        self.editor.Refresh()

    def onMoveBackwardButton(self, event):
        entities = self.editor.getMap().getEntities()
        curIndex = entities.index(self.tracking)
        insertIndex = curIndex
        entities.remove(self.tracking)
        for i in range(curIndex):
            e = entities[curIndex - i - 1]
            if self.doPropsOverlap(self.tracking, e):
                insertIndex = curIndex - i - 1
                break
        entities.insert(insertIndex, self.tracking)
        self.editor.Refresh()

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
            #self.rotField.SetValue(str(t.getRot()))
            self.tracking = t
            self.posXField.Enable(True)
            self.posYField.Enable(True)
            self.scaleField.Enable(True)
            #self.rotField.Enable(True)
            self.moveForwardButton.Enable(True)
            self.moveBackwardButton.Enable(True)
            self.deleteButton.Enable(True)
        else:
            self.posXField.SetValue("")
            self.posYField.SetValue("")
            self.scaleField.SetValue("")
            #self.rotField.SetValue("")
            self.posXField.Enable(False)
            self.posYField.Enable(False)
            self.scaleField.Enable(False)
            #self.rotField.Enable(False)
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
        self.editor.getDisplay().Refresh()

    def onMapMouseEvent(self, display, event):
        x, y = display.calcMapLocationFromScreen(event.GetX(), event.GetY())

        if event.LeftDown():
            hit = None
            self.initialClick = (x, y)
            for e in display.getMap().getEntities():
                pos = e.getPos()
                scale = e.getScale()
                bmp = e.getSprite().getBitmap()
                size = (bmp.GetWidth(), bmp.GetHeight())
                if x < pos[0] - size[0]/2*scale: continue
                if x > pos[0] + size[0]/2*scale: continue
                if y < pos[1] - size[1]/2*scale: continue
                if y > pos[1] + size[1]/2*scale: continue

                if self.selectModeButton.GetValue():
                    img = bmp.ConvertToImage()
                    subx = (x - (pos[0] - size[0]/2.*scale))/scale
                    suby = (y - (pos[1] - size[1]/2.*scale))/scale
                    if subx == img.GetWidth() or suby == img.GetHeight(): continue
                    if img.GetAlpha(subx, suby) == 0: continue

                hit = e

            if self.tracking:
                self.detachFromEntity(self.tracking)

            if hit:
                self.initialPos = hit.getPos()
                self.attachToEntity(hit)
                self.editor.Refresh()
        
        if event.LeftIsDown() and self.tracking:
            pos = [self.initialPos[0] + x - self.initialClick[0], self.initialPos[1] + y - self.initialClick[1]]
            curPos = self.tracking.getPos()
            if self.snapButton.GetValue():
                snap = float(self.snapAmount.GetValue())
                rect = self.tracking.getRect()
                offset = (pos[0] - curPos[0], pos[1] - curPos[1])
                rect = (rect[0]+offset[0], rect[1]+offset[1], rect[2]+offset[0], rect[3]+offset[1])
                snapRect = ((rect[0] + snap/2.) // snap * snap, (rect[1] + snap/2.) // snap * snap, (rect[2] + snap/2.) // snap * snap, (rect[3] + snap/2.) // snap * snap)
                diffRect = (snapRect[0] - rect[0], snapRect[1] - rect[1], snapRect[2] - rect[2], snapRect[3] - rect[3])
                if diffRect[0] < diffRect[2]: pos[0] += diffRect[0]
                else: pos[0] += diffRect[2]
                if diffRect[1] < diffRect[3]: pos[1] += diffRect[1]
                else: pos[1] += diffRect[3]
            if curPos[0] == pos[0] and curPos[1] == pos[1]: return
            self.tracking.setPos(pos)
            self.editor.Refresh()

    def onMapDraw(self, display, gc, rect):
        if self.snapButton.GetValue() and self.snapAmount.GetValue():
            (worldX, worldY) = display.getMap().getProperties().getSize()
            (worldX, worldY) = (worldX*MaterialLibrary.getMaterialSize(), worldY*MaterialLibrary.getMaterialSize())
            
            tileSize = float(self.snapAmount.GetValue())
            sizeX, sizeY = (int(worldX // tileSize), int(worldY // tileSize))
            startX = int(rect[0] // tileSize)
            startY = int(rect[1] // tileSize)
            endX = int(rect[2] // tileSize + 1)
            endY = int(rect[3] // tileSize + 1)
            if endX > sizeX: endX = sizeX
            if endY > sizeY: endY = sizeY

            gc.SetPen(wx.Pen(wx.Color(0, 0, 0, 32), 2))
            for x in range(startX, endX+1):
                gc.StrokeLine(x*tileSize, 0, x*tileSize, sizeY*tileSize)
            for y in range(startY, endY+1):
                gc.StrokeLine(0, y*tileSize, sizeX*tileSize, y*tileSize)

        if self.outlineButton.GetValue():
            gc.SetPen(wx.Pen(wx.Color(80, 80, 200, 128), 2))
            for e in self.editor.getMap().getEntities():
                pos = e.getPos()
                scale = e.getScale()
                size = (e.getSprite().getBitmap().GetWidth(), e.getSprite().getBitmap().GetHeight())
                left = pos[0] - size[0]/2*scale
                right = pos[0] + size[0]/2*scale
                top = pos[1] - size[1]/2*scale
                bottom = pos[1] + size[1]/2*scale
                
                gc.StrokeLine(left, top, right, top)
                gc.StrokeLine(left, bottom, right, bottom)
                gc.StrokeLine(left, top, left, bottom)
                gc.StrokeLine(right, top, right, bottom)

        if self.tracking:
            pos = self.tracking.getPos()
            scale = self.tracking.getScale()
            size = (self.tracking.getSprite().getBitmap().GetWidth(), self.tracking.getSprite().getBitmap().GetHeight())
            left = pos[0] - size[0]/2*scale
            right = pos[0] + size[0]/2*scale
            top = pos[1] - size[1]/2*scale
            bottom = pos[1] + size[1]/2*scale
        
            gc.SetPen(wx.Pen(wx.Color(255, 0, 0, 200), 3))
            gc.StrokeLine(left, top, right, top)
            gc.StrokeLine(left, bottom, right, bottom)
            gc.StrokeLine(left, top, left, bottom)
            gc.StrokeLine(right, top, right, bottom)


