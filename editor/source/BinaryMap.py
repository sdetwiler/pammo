import osfix
from struct import *
import POI

# Visuals File Format
# 0x50 0x49 0x56 0x02 // 'PIV' 1 (Paradise Irradiated Visuals 2)
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

def saveVisuals(map):
    output = ''

    # Visuals headers.
    output += pack('4B', 0x50, 0x49, 0x56, 0x02)

    # Backdrop
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
    groups = map.getCollisionGroups()
    output += pack('!H', len(groups))
    for group in groups:
        output += pack('!H', 0)
        output += pack('!H', len(group))
        for x, y in group:
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
