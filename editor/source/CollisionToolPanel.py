import wx
import NumValidator
import math
import CommonDrawing
import CollisionShape

class CollisionToolPanel(wx.Panel):
    def __init__(self, parent, id):
        wx.Panel.__init__(self, parent, id)

        self.editor = None
        self.showPOIs = True
        self.showSafeZone = False
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

        self.showPOIsButton = wx.CheckBox(self, -1, "Show POIs")
        self.showPOIsButton.SetValue(self.showPOIs)
        self.Bind(wx.EVT_CHECKBOX, self.onShowPOIsChanged, self.showPOIsButton)
        sizer.Add(self.showPOIsButton, 0, wx.ALIGN_LEFT | wx.ALL, 5)

        self.showSafeZoneButton = wx.CheckBox(self, -1, "Show Safe Zone")
        self.showSafeZoneButton.SetValue(self.showSafeZone)
        self.Bind(wx.EVT_CHECKBOX, self.onSafeZoneChanged, self.showSafeZoneButton)
        sizer.Add(self.showSafeZoneButton, 0, wx.ALIGN_LEFT | wx.ALL, 5)

        self.newButton = wx.Button(self, -1, "New Shape")
        self.Bind(wx.EVT_BUTTON, self.onNewButton, self.newButton)
        sizer.Add(self.newButton, 0, wx.ALIGN_LEFT | wx.ALL, 5)
        line = wx.StaticLine(self, -1, style = wx.LI_HORIZONTAL)
        sizer.Add(line, 0, wx.EXPAND | wx.ALL, 5)

        self.playerCollideButton = wx.CheckBox(self, -1, "Player Collide")
        self.Bind(wx.EVT_CHECKBOX, self.onPlayerCollideChanged, self.playerCollideButton)
        sizer.Add(self.playerCollideButton, 0, wx.ALIGN_LEFT | wx.ALL, 5)

        self.enemiesCollideButton = wx.CheckBox(self, -1, "Enemies Collide")
        self.Bind(wx.EVT_CHECKBOX, self.onEnemiesCollideChanged, self.enemiesCollideButton)
        sizer.Add(self.enemiesCollideButton, 0, wx.ALIGN_LEFT | wx.ALL, 5)

        self.duplicateButton = wx.Button(self, -1, "Duplicate")
        self.Bind(wx.EVT_BUTTON, self.onDuplicateButton, self.duplicateButton)
        sizer.Add(self.duplicateButton, 0, wx.ALIGN_LEFT | wx.ALL, 5)

        self.deleteButton = wx.Button(self, -1, "Delete")
        self.Bind(wx.EVT_BUTTON, self.onDeleteButton, self.deleteButton)
        sizer.Add(self.deleteButton, 0, wx.ALIGN_LEFT | wx.ALL, 5)

        self.SetSizerAndFit(sizer)
        self.setState()

    def onSnapChanged(self, event):
        if self.editor: self.editor.Refresh()

    def onShowPOIsChanged(self, event):
        self.showPOIs = self.showPOIsButton.GetValue()
        if self.editor: self.editor.Refresh()

    def onSafeZoneChanged(self, event):
        self.showSafeZone = self.showSafeZoneButton.GetValue()
        if self.editor: self.editor.Refresh()

    def onPlayerCollideChanged(self, event):
        self.selectedShape.setPlayerCollide(self.playerCollideButton.GetValue())

    def onEnemiesCollideChanged(self, event):
        self.selectedShape.setEnemiesCollide(self.enemiesCollideButton.GetValue())

    def onDeleteButton(self, event):
        # Delete a point or a shape?
        points = self.selectedShape.getPoints()
        if self.selectedPoint != None and len(points) > 3:
            del points[self.selectedPoint]
            self.selectedPoint = (self.selectedPoint - 1) % len(points)
            self.selectedShape.setPoints(points)
        else:
            selectedShape = self.selectedShape
            self.deselect()
            self.setState()
            self.editor.getMap().removeCollisionShape(selectedShape)

    def onDuplicateButton(self, event):
        newShape = CollisionShape.CollisionShape()
        newShape.setPlayerCollide(self.selectedShape.getPlayerCollide())
        newShape.setEnemiesCollide(self.selectedShape.getEnemiesCollide())

        points = []
        for i in self.selectedShape.getPoints():
            points.append([i[0]+50, i[1]+50])
        newShape.setPoints(points)

        self.selectedShape = newShape
        self.selectedPoint = None
        self.setState()
        self.editor.getMap().addCollisionShape(newShape)

    def onNewButton(self, event):
        # Find the middle of the screen in world space.
        size = self.editor.Size
        center = self.editor.getDisplay().calcMapLocationFromScreen(size[0]/2, size[1]/2)

        grow = 50
        shape = CollisionShape.CollisionShape()
        shape.setPoints([[center[0] - grow, center[1] - grow],
                         [center[0] - grow, center[1] + grow],
                         [center[0] + grow, center[1] + grow],
                         [center[0] + grow, center[1] - grow]])
        self.selectedShape = shape
        self.selectedPoint = None
        self.setState()
        self.editor.getMap().addCollisionShape(shape)

    def setState(self):
        if self.editor:
            self.newButton.Enable(True)
        else:
            self.newButton.Enable(False)

        if self.editor and self.selectedShape:
            self.playerCollideButton.Enable(True)
            self.playerCollideButton.SetValue(self.selectedShape.getPlayerCollide())
            self.enemiesCollideButton.Enable(True)
            self.enemiesCollideButton.SetValue(self.selectedShape.getEnemiesCollide())

            self.duplicateButton.Enable(True)
            self.deleteButton.Enable(True)
        else:
            self.playerCollideButton.Enable(False)
            self.playerCollideButton.SetValue(False)
            self.enemiesCollideButton.Enable(False)
            self.enemiesCollideButton.SetValue(False)

            self.duplicateButton.Enable(False)
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
        self.selectedShape = None
        self.selectedPoint = None

    def findClosestShapeAndPoint(self, shapes, pos):
        dist = 1e300
        foundShape = None
        foundPointIndex = None

        for shape in shapes:
            for (pointIndex, point) in enumerate(shape.getPoints()):
                d = (point[0] - pos[0])**2 + (point[1] - pos[1])**2
                if d < dist:
                    dist = d
                    foundShape = shape
                    foundPointIndex = pointIndex
        if dist < 300:
            return foundShape, foundPointIndex
        else:
            return None, None

    def findClosestNewShapeAndPoint(self, shapes, pos):
        dist = 1e300
        foundShape = None
        foundPointIndex = None
        insertCoordinate = (0, 0)

        for shape in shapes:
            points = shape.getPoints()
            for (pointIndex, point) in enumerate(points):
                point2 = points[(pointIndex+1)%len(points)]
                vec = ((point[0] + point2[0])/2., (point[1] + point2[1])/2.)
                d = (vec[0] - pos[0])**2 + (vec[1] - pos[1])**2
                if d < dist:
                    dist = d
                    foundShape = shape
                    foundPointIndex = pointIndex
                    insertCoordinate = vec
        if dist < 300:
            points = foundShape.getPoints()
            points.insert(foundPointIndex+1, insertCoordinate)
            foundShape.setPoints(points)
            return foundShape, foundPointIndex+1
        else:
            return None, None

    def findInteriorShape(self, shapes, p):
        for shape in shapes:
            
            # Point in polygon
            # http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
            c = 0
            testx, testy = p
            points = shape.getPoints()
            for i in range(len(points)):
                vertxi, vertyi = points[i]
                vertxj, vertyj = points[(i-1) % len(points)]

                if (vertyi>testy) != (vertyj>testy):
                    if testx < (vertxj-vertxi) * (testy-vertyi) / (vertyj-vertyi) + vertxi:
                        c = not c;
            if c: return shape
        return None

    def onMapMouseEvent(self, display, event):
        pos = display.calcMapLocationFromScreen(event.GetX(), event.GetY())

        # Determin if anything was hit by this click.
        if event.LeftDown():
            shapes = display.getMap().getCollisionShapes()
            g, p = self.findClosestShapeAndPoint(shapes, pos)
            if g == None:
                g, p = self.findClosestNewShapeAndPoint(shapes, pos)
                if g == None:
                    self.initialClick = pos
                    g = self.findInteriorShape(shapes, pos)
                    p = None
                
            self.selectedShape, self.selectedPoint = g, p
            self.setState()
            display.Refresh()
        
        # Drag a vertex.
        if event.LeftIsDown() and event.Dragging() and self.selectedPoint != None:
            if self.snapButton.GetValue():
                snap = float(self.snapAmount.GetValue())
                pos = (round(pos[0]/snap) * snap, round(pos[1]/snap) * snap)
            points = self.selectedShape.getPoints()
            points[self.selectedPoint] = pos
            self.selectedShape.setPoints(points)
        
        # Drag a poly.
        if event.LeftIsDown() and event.Dragging() and self.selectedShape != None and self.selectedPoint == None:
            offset = (pos[0] - self.initialClick[0], pos[1] - self.initialClick[1])
            #if self.snapButton.GetValue():
            #    snap = float(self.snapAmount.GetValue())
            #    pos = (round(pos[0]/snap) * snap, round(pos[1]/snap) * snap)
            points = self.selectedShape.getPoints()
            for i in range(len(points)):
                points[i] = (points[i][0] + offset[0], points[i][1] + offset[1])
            self.selectedShape.setPoints(points)
            self.initialClick = pos

    def onMapDraw(self, display, gc, rect):
        # Draw POIs.
        if self.showPOIs:
            CommonDrawing.drawPOIs(display, gc, rect)

        # Draw safe zone.
        if self.showSafeZone:
            CommonDrawing.drawSafeZone(display, gc, rect)

        # Draw snap if I'm supposeda.
        if self.snapButton.GetValue() and self.snapAmount.GetValue():
            CommonDrawing.drawGrid(display, gc, rect, self.snapAmount.GetValue())

        # Draw all collision shapes.
        CommonDrawing.drawCollisionShapes(display, gc, rect)

        # If one is selcted, draw it perty and special.
        if self.selectedShape != None:
            points = self.selectedShape.getPoints()

            # Draw the path more different.
            gc.SetBrush(wx.Brush(wx.Color(100, 0, 0, 92)))
            gc.SetPen(wx.Pen(wx.Color(128, 0, 0, 168), 3))
            path = gc.CreatePath()
            path.MoveToPoint(points[0][0], points[0][1])
            for point in points[1:]:
                path.AddLineToPoint(point[0], point[1])
            path.CloseSubpath()
            gc.FillPath(path)
            gc.StrokePath(path)

            # Draw the selected point most different.
            if self.selectedPoint != None:
                gc.SetBrush(wx.Brush(wx.Color(0, 0, 128, 228)))
                gc.SetPen(wx.Pen(wx.Color(0, 0, 128, 228), 6))
                point = points[self.selectedPoint]
                gc.DrawEllipse(point[0]-7, point[1]-7, 14, 14)

            # Draw new point handles more different.
            gc.SetBrush(wx.Brush(wx.Color(148, 32, 64, 128)))
            gc.SetPen(wx.Pen(wx.Color(148, 32, 64, 128), 3))
            for i in range(len(points)):
                v1 = points[i]
                v2 = points[(i+1)%len(points)]
                a = ((v1[0] + v2[0]) / 2., (v1[1] + v2[1])/2.)
                gc.DrawEllipse(a[0]-3, a[1]-3, 6, 6)

            # Draw vertexes more different.
            gc.SetBrush(wx.Brush(wx.Color(64, 32, 192, 200)))
            gc.SetPen(wx.Pen(wx.Color(64, 32, 192, 200), 3))
            for point in points:
                gc.DrawEllipse(point[0]-3, point[1]-3, 6, 6)
        
        
