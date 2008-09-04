import wx
import POI

def drawCollisionShapes(display, gc, rect):
    collisionShapes = display.getMap().getCollisionShapes()

    # Draw all collision shapes.
    for shape in collisionShapes:
        if not shape.getEnemiesCollide():
            gc.SetBrush(wx.Brush(wx.Color(50, 128, 50, 92)))
            gc.SetPen(wx.Pen(wx.Color(64, 192, 64, 168), 3))
        else:
            gc.SetBrush(wx.Brush(wx.Color(0, 100, 0, 92)))
            gc.SetPen(wx.Pen(wx.Color(0, 128, 0, 168), 3))

        points = shape.getPoints()
        path = gc.CreatePath()
        path.MoveToPoint(points[0][0], points[0][1])
        for point in points[1:]:
            path.AddLineToPoint(point[0], point[1])
        path.CloseSubpath()
        gc.FillPath(path)
        gc.StrokePath(path)

def drawPOIs(display, gc, rect):
    # Draw all POIs.
    strokeSize = 3
    displaySize = 32
    pois = display.getMap().getPOIs()
    for poi in pois:
        type = poi.getType()
        if type == POI.PlayerStartTypeName:
            gc.SetBrush(wx.Brush(wx.Color(0, 0, 100, 92)))
            gc.SetPen(wx.Pen(wx.Color(0, 0, 128, 168), strokeSize))
        elif type == POI.InfastructurePointTypeName:
            gc.SetBrush(wx.Brush(wx.Color(0, 100, 100, 92)))
            gc.SetPen(wx.Pen(wx.Color(0, 128, 128, 168), strokeSize))
        elif type == POI.SpawnPointTypeName:
            gc.SetBrush(wx.Brush(wx.Color(0, 100, 0, 92)))
            gc.SetPen(wx.Pen(wx.Color(0, 128, 0, 168), strokeSize))
        else: continue

        pos = poi.getPos()
        gc.DrawEllipse(pos[0] - displaySize/2 + strokeSize - 1, pos[1] - displaySize/2 + strokeSize - 1,
                       displaySize - strokeSize*2 + 1, displaySize - strokeSize*2 + 1)

def drawGrid(display, gc, rect, amount):
    # Draw snap if I'm supposeda.
    (worldX, worldY) = display.getMap().getSize()
    
    tileSize = float(amount)
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

def drawSafeZone(display, gc, rect):
    (worldX, worldY) = display.getMap().getSize()
    bounds = [480/2, 320/2,
              worldX - 480/2,
              worldY - 320/2]
    
    gc.SetPen(wx.Pen(wx.Color(192, 48, 192, 128), 5))
    gc.StrokeLine(bounds[0], bounds[1], bounds[0], bounds[3])
    gc.StrokeLine(bounds[2], bounds[1], bounds[2], bounds[3])
    gc.StrokeLine(bounds[0], bounds[1], bounds[2], bounds[1])
    gc.StrokeLine(bounds[0], bounds[3], bounds[2], bounds[3])
    
