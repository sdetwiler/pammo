import os
from struct import *

# File Format
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

def dumpHex(str):
    for i in str:
        print ('%.2x ' % ord(i)),
    print ''

def moveFile(name):
    here = os.path.abspath('./' + name)
    there = os.path.abspath('../data/' + name)
    #print 'Move from %s to %s' % (here, there)

    f = open(here, "r")
    lines = f.read()
    f = open(there, "w+")
    f.write(lines)

def accumulate(store, name):
    if name in store: return store[name]

    l = len(store)
    store[name] = l
    return l

def save(map):
    properties = map.getProperties()

    output = ''

    # Save tiles and materials.
    store = {}
    body = output + pack('!2H', properties.getSizeX(), properties.getSizeY())
    for y in range(properties.getSizeY()):
        for x in range(properties.getSizeX()):
            n = accumulate(store, map.getMaterialTile(x, y))
            body += pack('!H', n)
    header = pack('!H', len(store))
    for s in store.keys():
        header += s + pack('x')
        if s: moveFile('materials/' + s + '.png')
    output += header + body

    # Save entities and props
    store = {}
    body = pack('!H', len(map.getEntities()))
    for e in map.getEntities():
        name = accumulate(store, e.getProp().getName())
        pos = e.getPos()
        body += pack('!H4f', name, pos[0], pos[1], e.getScale(), e.getRot())

    header = pack('!H', len(store))
    for s in store.keys():
        header += s + pack('x')
        if s: moveFile('props/' + s + '.png')
    output += header + body
        
    path = os.path.abspath("../data/maps/%s.bmap" % properties.getName())
    f = open(path, "w+")
    f.write(output)
