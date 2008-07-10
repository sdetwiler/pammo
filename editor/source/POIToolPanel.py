import wx
import math
import NumValidator
import MaterialLibrary
import POI

class POIToolPanel(wx.Panel):
    def __init__(self, parent, id):
        wx.Panel.__init__(self, parent, id)

        self.editor = None
        self.selected = None
        self.displaySize = 32
        self.strokeSize = 3

        sizer = wx.BoxSizer(wx.VERTICAL)

        self.snapButton = wx.CheckBox(self, -1, "Snap")
        self.Bind(wx.EVT_CHECKBOX, self.onSnapChanged, self.snapButton)
        self.snapAmount = wx.TextCtrl(self, -1, "32", validator = NumValidator.NumValidator())
        self.snapAmount.Bind(wx.EVT_TEXT, self.onSnapChanged, self.snapAmount)
        self.snapAmount.SetInitialSize((self.snapAmount.GetBestSize()[0]/2, self.snapAmount.GetBestSize()[1]))
        row = wx.BoxSizer(wx.HORIZONTAL)
        row.Add(self.snapButton, 1, wx.ALIGN_LEFT | wx.ALIGN_BOTTOM | wx.RIGHT, 5)
        row.Add(self.snapAmount, 0, wx.ALIGN_RIGHT)
        sizer.Add(row, 0, wx.EXPAND | wx.ALL, 5)

        self.newButton = wx.Button(self, -1, "New POI")
        self.Bind(wx.EVT_BUTTON, self.onNewButton, self.newButton)
        sizer.Add(self.newButton, 0, wx.ALIGN_RIGHT | wx.ALL, 5)

        line = wx.StaticLine(self, -1, style = wx.LI_HORIZONTAL)
        sizer.Add(line, 0, wx.EXPAND | wx.ALL, 5)

        self.typeRadioBox = wx.RadioBox(self, -1, "Type", wx.DefaultPosition, wx.DefaultSize,
                                        POI.TypeNames, 1, wx.RA_SPECIFY_COLS)
        self.Bind(wx.EVT_RADIOBOX, self.onTypeChanged, self.typeRadioBox)
        sizer.Add(self.typeRadioBox, 0, wx.ALIGN_RIGHT | wx.ALL, 5)

        self.deleteButton = wx.Button(self, -1, "Delete")
        self.Bind(wx.EVT_BUTTON, self.onDeleteButton, self.deleteButton)
        sizer.Add(self.deleteButton, 0, wx.ALIGN_RIGHT | wx.ALL, 5)

        self.SetSizerAndFit(sizer)
        self.setState()

    def onSnapChanged(self, event):
        if self.editor: self.editor.Refresh()

    def onDeleteButton(self, event):
        poi = self.selected
        self.selected = None
        self.editor.getMap().removePOI(poi)
        self.setState()

    def onTypeChanged(self, event):
        self.selected.setType(POI.TypeNames[event.GetSelection()])

    def onNewButton(self, event):
        # Find the middle of the screen in world space.
        size = self.editor.Size
        center = self.editor.getDisplay().calcMapLocationFromScreen(size[0]/2, size[1]/2)

        poi = POI.POI(center)
        self.selected = poi
        self.editor.getMap().addPOI(poi)
        self.setState()

    def setState(self):
        if self.editor:
            self.newButton.Enable(True)
        else:
            self.newButton.Enable(False)

        if self.editor and self.selected:
            self.typeRadioBox.Enable(True)
            self.typeRadioBox.SetSelection(POI.TypeNames.index(self.selected.getType()))
            self.deleteButton.Enable(True)
        else:
            self.typeRadioBox.Enable(False)
            self.typeRadioBox.SetSelection(0)
            self.deleteButton.Enable(False)

    def attachToEditor(self, editor):
        self.editor = editor
        editor.getDisplay().addMouseObserver(self.onMapMouseEvent)
        editor.getDisplay().addDrawObserver(self.onMapDraw)
        self.setState()

    def detachFromEditor(self, editor):
        self.editor = None
        editor.getDisplay().removeMouseObserver(self.onMapMouseEvent)
        editor.getDisplay().removeDrawObserver(self.onMapDraw)
        self.selected = None
        self.setState()

    def findClosestPOI(self, pos):
        foundPOI = None
        foundDist = 1e300

        for poi in self.editor.getMap().getPOIs():
            p = poi.getPos()
            d = math.sqrt((p[0] - pos[0])**2 + (p[1] - pos[1])**2)
            if d < foundDist:
                foundPOI = poi
                foundDist = d
        if foundDist < self.displaySize/2 * 1.1:
            return foundPOI
        else:
            return None

    def onMapMouseEvent(self, display, event):
        (x, y) = display.calcMapLocationFromScreen(event.GetX(), event.GetY())
        
        if event.LeftDown():
            self.initialClick = (x, y)
            self.selected = self.findClosestPOI((x, y))
            if self.selected:
                self.initialPos = self.selected.getPos()
            self.setState()
            display.Refresh()
        
        if event.LeftIsDown() and event.Dragging() and self.selected:
            pos = [self.initialPos[0] + x - self.initialClick[0], self.initialPos[1] + y - self.initialClick[1]]
            curPos = self.selected.getPos()
            if self.snapButton.GetValue():
                snap = float(self.snapAmount.GetValue())
                rect = (curPos[0] - self.displaySize/2, curPos[1] - self.displaySize/2,
                        curPos[0] + self.displaySize/2, curPos[1] + self.displaySize/2)
                offset = (pos[0] - curPos[0], pos[1] - curPos[1])
                rect = (rect[0]+offset[0], rect[1]+offset[1], rect[2]+offset[0], rect[3]+offset[1])
                snapRect = ((rect[0] + snap/2.) // snap * snap, (rect[1] + snap/2.) // snap * snap, (rect[2] + snap/2.) // snap * snap, (rect[3] + snap/2.) // snap * snap)
                diffRect = (snapRect[0] - rect[0], snapRect[1] - rect[1], snapRect[2] - rect[2], snapRect[3] - rect[3])
                if diffRect[0] < diffRect[2]: pos[0] += diffRect[0]
                else: pos[0] += diffRect[2]
                if diffRect[1] < diffRect[3]: pos[1] += diffRect[1]
                else: pos[1] += diffRect[3]
            if curPos[0] == pos[0] and curPos[1] == pos[1]: return
            self.selected.setPos(pos)
            self.editor.Refresh()

    def onMapDraw(self, display, gc, rect):
        # Draw snap if I'm supposeda.
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

        pois = display.getMap().getPOIs()
        
        # Draw all collision shapes.
        gc.SetBrush(wx.Brush(wx.Color(0, 100, 0, 92)))
        gc.SetPen(wx.Pen(wx.Color(0, 128, 0, 168), self.strokeSize))
        for poi in pois:
            pos = poi.getPos()
            gc.DrawEllipse(pos[0] - self.displaySize/2 + self.strokeSize - 1, pos[1] - self.displaySize/2 + self.strokeSize - 1,
                           self.displaySize - self.strokeSize*2 + 1, self.displaySize - self.strokeSize*2 + 1)
        
        # If one is selcted, draw it perty and special.
        if self.selected:
            gc.SetBrush(wx.Brush(wx.Color(100, 0, 0, 92)))
            gc.SetPen(wx.Pen(wx.Color(128, 0, 0, 168), self.strokeSize))
            pos = self.selected.getPos()
            gc.DrawEllipse(pos[0] - self.displaySize/2 + self.strokeSize - 1, pos[1] - self.displaySize/2 + self.strokeSize - 1,
                           self.displaySize - self.strokeSize*2 + 1, self.displaySize - self.strokeSize*2 + 1)
        
