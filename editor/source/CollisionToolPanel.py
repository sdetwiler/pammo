import wx
import NumValidator
import MaterialLibrary

class CollisionToolPanel(wx.Panel):
    def __init__(self, parent, id):
        wx.Panel.__init__(self, parent, id)

        self.editor = None
        self.deselect()

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

        self.newButton = wx.Button(self, -1, "New Shape")
        self.Bind(wx.EVT_BUTTON, self.onNewButton, self.newButton)
        sizer.Add(self.newButton, 0, wx.ALIGN_RIGHT | wx.ALL, 5)

        line = wx.StaticLine(self, -1, style = wx.LI_HORIZONTAL)
        sizer.Add(line, 0, wx.EXPAND | wx.ALL, 5)

        self.addBeforeButton = wx.Button(self, -1, "Add Before")
        self.Bind(wx.EVT_BUTTON, self.onAddBeforeButton, self.addBeforeButton)
        sizer.Add(self.addBeforeButton, 0, wx.ALIGN_RIGHT | wx.ALL, 5)

        self.addAfterButton = wx.Button(self, -1, "Add After")
        self.Bind(wx.EVT_BUTTON, self.onAddAfterButton, self.addAfterButton)
        sizer.Add(self.addAfterButton, 0, wx.ALIGN_RIGHT | wx.ALL, 5)

        self.deleteButton = wx.Button(self, -1, "Delete")
        self.Bind(wx.EVT_BUTTON, self.onDeleteButton, self.deleteButton)
        sizer.Add(self.deleteButton, 0, wx.ALIGN_RIGHT | wx.ALL, 5)

        self.SetSizerAndFit(sizer)
        self.setState()

    def onSnapChanged(self, event):
        if self.editor: self.editor.Refresh()

    def onDeleteButton(self, event):
        groups = self.editor.getMap().getCollisionGroups()

        if len(groups[self.selectedGroup]) <= 3:
            del groups[self.selectedGroup]
            self.deselect()
        else:
            del groups[self.selectedGroup][self.selectedPoint]
            self.selectedPoint = (self.selectedPoint - 1) % len(groups[self.selectedGroup])

        self.editor.getMap().setCollisionGroups(groups)
        self.setState()

    def onAddBeforeButton(self, event):
        groups = self.editor.getMap().getCollisionGroups()
        group = groups[self.selectedGroup]

        p1 = group[self.selectedPoint]
        p2 = group[(self.selectedPoint + 1) % len(group)]
        
        newP = ((p1[0] + p2[0])/2, (p1[1] + p2[1])/2)
        group.insert(self.selectedPoint+1, newP)
        self.selectedPoint += 1
        self.editor.getMap().setCollisionGroups(groups)

    def onAddAfterButton(self, event):
        groups = self.editor.getMap().getCollisionGroups()
        group = groups[self.selectedGroup]

        p1 = group[self.selectedPoint]
        p2 = group[(self.selectedPoint - 1) % len(group)]
        
        newP = ((p1[0] + p2[0])/2, (p1[1] + p2[1])/2)
        group.insert(self.selectedPoint, newP)
        self.editor.getMap().setCollisionGroups(groups)

    def onNewButton(self, event):
        # Find the middle of the screen in world space.
        size = self.editor.Size
        center = self.editor.getDisplay().calcMapLocationFromScreen(size[0]/2, size[1]/2)

        grow = 100        
        groups = self.editor.getMap().getCollisionGroups()
        groups.append([[center[0], center[1] - grow],
                       [center[0] - grow, center[1] + grow/2],
                       [center[0] + grow, center[1] + grow/2]])
        self.selectedGroup = len(groups)-1
        self.selectedPoint = 0
        self.editor.getMap().setCollisionGroups(groups)
        self.setState()

    def setState(self):
        if self.editor:
            self.newButton.Enable(True)
        else:
            self.newButton.Enable(False)

        if self.editor and self.hasSelection():
            self.addAfterButton.Enable(True)
            self.addBeforeButton.Enable(True)
            self.deleteButton.Enable(True)
        else:
            self.addAfterButton.Enable(False)
            self.addBeforeButton.Enable(False)
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
        self.deselect()
        self.setState()

    def deselect(self):
        self.selectedGroup = -1
        self.selectedPoint = -1
    
    def hasSelection(self):
        if self.selectedGroup != -1 and self.selectedPoint != -1:
            return True
        else:
            return False

    def findClosestGroupAndPoint(self, groups, pos):
        dist = 1e300
        foundGroupIndex = -1
        foundPointIndex = -1

        for (groupIndex, group) in enumerate(groups):
            for (pointIndex, point) in enumerate(group):
                d = (point[0] - pos[0])**2 + (point[1] - pos[1])**2
                if d < dist:
                    dist = d
                    foundGroupIndex = groupIndex
                    foundPointIndex = pointIndex
        if dist < 300:
            return foundGroupIndex, foundPointIndex
        else:
            return -1, -1

    def onMapMouseEvent(self, display, event):
        pos = display.calcMapLocationFromScreen(event.GetX(), event.GetY())

        if event.LeftDown():
            collisionGroup = display.getMap().getCollisionGroups()
            g, p = self.findClosestGroupAndPoint(collisionGroup, pos)
            self.selectedGroup, self.selectedPoint = g, p
            self.setState()
            display.Refresh()
        
        if event.LeftIsDown() and event.Dragging() and self.hasSelection():
            if self.snapButton.GetValue():
                snap = float(self.snapAmount.GetValue())
                pos = (round(pos[0]/snap) * snap, round(pos[1]/snap) * snap)
            collisionGroups = display.getMap().getCollisionGroups()
            collisionGroups[self.selectedGroup][self.selectedPoint] = pos
            display.getMap().setCollisionGroups(collisionGroups)

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

        groups = display.getMap().getCollisionGroups()

        # Draw all collision shapes.
        gc.SetBrush(wx.Brush(wx.Color(0, 100, 0, 92)))
        gc.SetPen(wx.Pen(wx.Color(0, 128, 0, 168), 3))
        for group in groups:
            path = gc.CreatePath()
            path.MoveToPoint(group[0][0], group[0][1])
            for point in group[1:]:
                path.AddLineToPoint(point[0], point[1])
            path.CloseSubpath()
            gc.FillPath(path)
            gc.StrokePath(path)

        # If one is selcted, draw it perty and special.
        if self.hasSelection():
            gc.SetBrush(wx.Brush(wx.Color(100, 0, 0, 92)))
            gc.SetPen(wx.Pen(wx.Color(128, 0, 0, 168), 3))
            group = groups[self.selectedGroup]

            # Draw the path and points more different.
            path = gc.CreatePath()
            path.MoveToPoint(group[0][0], group[0][1])
            gc.DrawEllipse(group[0][0]-3, group[0][1]-3, 6, 6)
            for point in group[1:]:
                path.AddLineToPoint(point[0], point[1])
                gc.DrawEllipse(point[0]-3, point[1]-3, 6, 6)
            path.CloseSubpath()
            gc.FillPath(path)
            gc.StrokePath(path)

            # Draw the selected point most different.
            gc.SetPen(wx.Pen(wx.Color(0, 0, 128, 228), 6))
            point = groups[self.selectedGroup][self.selectedPoint]
            gc.DrawEllipse(point[0]-7, point[1]-7, 14, 14)
        
        
