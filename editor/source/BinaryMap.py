import osfix
from struct import *

# Map File Format
# uint16_t numMaterials
#    char* materialName\0
# uint16_t sizeX
# uint16_t sizeY
#    uint16_t tiles
# uint16_t numProps
#    char* propName\0
# uint16_t numEntities
#    uint16_t propId
#    float posx, posy
#    float size, rot

# Collision File Format
# uint16_t numShapes
#   uint16_t numPoints
#     float x
#     float y

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
    saveMap(map)
    saveCollision(map)

def saveMap(map):
    properties = map.getProperties()

    output = ''

    # Save tiles and materials.
    store = []
    body = output + pack('!2H', properties.getSizeX(), properties.getSizeY())
    for y in range(properties.getSizeY()):
        for x in range(properties.getSizeX()):
            n = accumulate(store, map.getMaterialTile(x, y))
            body += pack('!H', n)
    header = pack('!H', len(store))
    for s in store:
        header += s + pack('x')
    output += header + body

    # Save entities and props
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
        
    path = osfix.path("../data/bmaps/%s.bmap" % properties.getName())
    f = open(path, "w+b")
    f.write(output)

def saveCollision(map):
    output = ''

    groups = map.getCollisionGroups()
    output += pack('!H', len(groups))
    for group in groups:
        output += pack('!H', len(group))
        for x, y in group:
            output += pack('!2f', x, y)
    
    path = osfix.path("../data/collision/%s.col" % map.getProperties().getName())
    f = open(path, "w+b")
    f.write(output)
