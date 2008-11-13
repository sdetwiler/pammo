import os, osfix, math
from struct import *
import wx
import POI

# Visuals File Format
# 0x50 0x49 0x56 0x02 // 'PIV' 3 (Paradise Irradiated Visuals 3)
# uint16_t sizeX, sizeY
# char* backdropName\0
# uint16_t numProps
#    char* propName\0
# uint16_t numEntities
#    uint16_t propId
#    float posx, posy
#    float size, rot

# Overlays File Format
# 0x50 0x49 0x4F 0x01 // 'PIO' 1 (Paradise Irradiated Overlays 1)
# uint16_t numCollisionShapes
#   uint16_t properties
#   uint16_t numPoints
#     float x
#     float y
# uint16_t numPOIs
#   uint16_t properties
#   float x, y

def dumpHex(str):
    for i in str:
        print ('%.2x ' % ord(i)),
    print ''

def accumulate(store, name):
    if name in store:
        return store.index(name)

    l = len(store)
    store.append(name)
    return l

def save(map):
    saveVisuals(map)
    saveOverlays(map)
    saveBackdrop(map)
    saveMinimap(map)

def saveVisuals(map):
    output = ''

    # Visuals headers.
    output += pack('4B', 0x50, 0x49, 0x56, 0x03)

    # Backdrop
    output += pack('!H', map.getSizeX())
    output += pack('!H', map.getSizeY())
    output += map.getBackdrop().getName() + pack('x')

    # Entities and props
    store = []
    body = pack('!H', len(map.getEntities()))
    for e in map.getEntities():
        name = accumulate(store, e.getProp().getName())
        pos = e.getPos()
        body += pack('!H4f', name, pos[0], pos[1], e.getScale(), e.getRot())
    header = pack('!H', len(store))
    for s in store:
        header += s + pack('x')
    output += header + body

    # Save file.
    path = osfix.path("../data/maps/%s.vmap" % map.getName())
    f = open(path, "w+b")
    f.write(output)

def saveOverlays(map):
    output = ''

    # Overlays headers.
    output += pack('4B', 0x50, 0x49, 0x4F, 0x01)

    # Collions shapes.
    shapes = map.getCollisionShapes()
    output += pack('!H', len(shapes))
    for shape in shapes:
        points = shape.getPoints()
        properties = 0
        if shape.getPlayerCollide(): properties |= 1<<0
        if shape.getEnemiesCollide(): properties |= 1<<1

        output += pack('!H', properties)
        output += pack('!H', len(points))
        for x, y in points:
            output += pack('!2f', x, y)

    # Spawn points.
    count = 0
    poisOutput = ''
    for poi in map.getPOIs():
        type = poi.getType()
        property = POI.TypeNames.index(type)
        count += 1
        pos = poi.getPos()
        poisOutput += pack('!H', property)
        poisOutput += pack('!2f', pos[0], pos[1])
    output += pack('!H', count)
    output += poisOutput
    
    # Save file.
    path = osfix.path("../data/maps/%s.omap" % map.getName())
    f = open(path, "w+b")
    f.write(output)

def saveTile(pngpath, pvrtcpath, image):
    image.SaveFile(pngpath, wx.BITMAP_TYPE_PNG)
    
    try: os.system("/Developer/Platforms/iPhoneOS.platform/usr/bin/texturetool -e PVRTC -o %s %s" % (pvrtcpath, pngpath))
    except: pass

def saveBackdrop(map):
    name = map.getBackdrop().getName()
    image = wx.ImageFromBitmap(map.getBackdrop().getBitmap())
    fullx, fully = image.GetSize()
    
    try: os.mkdir(osfix.path("../data/backdroptiles/%s" % name))
    except: pass
    try: os.mkdir(osfix.path("../data/backdroppvrtc/%s" % name))
    except: pass
    
    ts = 496
    et = 16
    
    scaled = image.Scale(ts+et, ts+et)
    pngpath = osfix.path("../data/backdroptiles/%s/preview.png" % name)
    pvrtcpath = osfix.path("../data/backdroppvrtc/%s/preview.pvrtc" % name)
    saveTile(pngpath, pvrtcpath, scaled)
    scaled.Destroy()
    
    for x in range(int(math.ceil(float(map.getSizeX())/ts))):
        for y in range(int(math.ceil(float(map.getSizeY())/ts))):
            px, py = x*ts, y*ts
            sx, sy = ts + et, ts + et
            
            if px + sx > fullx: sx = fullx - px
            if py + sy > fully: sy = fully - py
            
            cropped = image.GetSubImage(wx.Rect(px, py, sx, sy))
            resized = cropped.Size((ts+et, ts+et), (0, 0), 0, 0, 0)
            pngpath = osfix.path("../data/backdroptiles/%s/%d-%d.png" % (name, x, y))
            pvrtcpath = osfix.path("../data/backdroppvrtc/%s/%d-%d.pvrtc" % (name, x, y))
            saveTile(pngpath, pvrtcpath, resized)
            resized.Destroy()
            cropped.Destroy()
    image.Destroy()

def saveMinimap(map):
    # Create mask bitmap.
    maskBitmap = wx.EmptyBitmap(64, 64)
    dc = wx.MemoryDC(maskBitmap)
    gc = wx.GraphicsContext.Create(dc)
    gc.SetBrush(wx.Brush(wx.Color(255, 255, 255, 255)))
    gc.SetPen(wx.NullPen)
    gc.DrawEllipse(1, 1, 62, 62)
    gc.Destroy()
    dc.Destroy()
    maskImage = wx.ImageFromBitmap(maskBitmap)
    #path = osfix.path("../../mask.png")
    #maskImage.SaveFile(path, wx.BITMAP_TYPE_PNG)
    
    # Create collision bitmap.
    collisionBitmap = wx.EmptyBitmapRGBA(64, 64, 0, 0, 0, 0)
    dc = wx.MemoryDC(collisionBitmap)
    gc = wx.GraphicsContext.Create(dc)
    sx = 64./map.getSizeX()
    sy = 64./map.getSizeY()
    for shape in map.getCollisionShapes():
        if not shape.getPlayerCollide():
            continue
        
        gc.SetBrush(wx.Brush(wx.Color(255, 255, 255, 255)))
        gc.SetPen(wx.NullPen)

        points = shape.getPoints()
        path = gc.CreatePath()
        path.MoveToPoint(sx*points[0][0], sy*points[0][1])
        for point in points[1:]:
            path.AddLineToPoint(sx*point[0], sy*point[1])
        path.CloseSubpath()
        gc.FillPath(path)
        gc.StrokePath(path)
    gc.Destroy()
    dc.Destroy()
    collisionImage = wx.ImageFromBitmap(collisionBitmap)
    #path = osfix.path("../../pre-collision.png")
    #collisionImage.SaveFile(path, wx.BITMAP_TYPE_PNG)
    
    for x in range(64):
        for y in range(64):
            if maskImage.GetAlpha(x, y) == 0:
                collisionImage.SetAlpha(x, y, 0)
            else:
                a = collisionImage.GetAlpha(x, y)
                a = a/255.* 80 + 100
                collisionImage.SetAlpha(x, y, a)
                collisionImage.SetRGB(x, y, 50, 230, 10)
    collisionBitmap = wx.BitmapFromImage(collisionImage)
    #path = osfix.path("../../collision.png")
    #collisionImage.SaveFile(path, wx.BITMAP_TYPE_PNG)
    
    # Create final bitmap.
    bitmap = wx.EmptyBitmap(64, 64)
    dc = wx.MemoryDC(bitmap)
    gc = wx.GraphicsContext.Create(dc)
    gc.DrawBitmap(collisionBitmap, 0, 0, 64, 64)
    gc.SetBrush(wx.NullBrush)
    gc.SetPen(wx.Pen(wx.Color(30, 15, 4, 220), 2))
    gc.DrawEllipse(1, 1, 62, 62)
    gc.Destroy()
    dc.Destroy()
    image = wx.ImageFromBitmap(bitmap)
    path = osfix.path("../data/interface/minimap.png")
    image.SaveFile(path, wx.BITMAP_TYPE_PNG)