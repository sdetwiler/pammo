import MapProperties
import Entity

class Map:
    def __init__(self):
        self.properties = MapProperties.MapProperties()
        self.materialTiles = []
        self.isDirty = True
        self.hasSavedOnce = False
        self.observers = []
        self.entities = []
        self.collisionGroups = []

    def addObserver(self, observer):
        self.observers.append(observer)

    def removeObserver(self, observer):
        self.observers.remove(observer)

    def _notify(self):
        for observer in self.observers: observer(self)

    def getIsDirty(self): return self.isDirty

    def getHasSavedOnce(self): return self.hasSavedOnce

    def onEntityChanged(self, entity):
        self.isDirty = True
        self._notify()

    def saveToFile(self, f):
        output = {"version":3, "sizeX": self.properties.getSizeX(), "sizeY": self.properties.getSizeY()}
        output["materialTiles"] = self.materialTiles
        
        entities = []
        for s in self.entities:
            entities.append(s.saveToDic())
        output['entities'] = entities

        output['collisionGroups'] = self.collisionGroups

        f.write(str(output))

        self.isDirty = False
        self.hasSavedOnce = True
        self._notify()

    def loadFromFile(self, name, f):
        self.properties.setName(name)
        
        dic = eval(f.readline())
        
        if dic['version'] != 3: raise ''
        self.resizeMaterialTiles(dic['sizeX'] - self.properties.getSizeX(), dic['sizeY'] - self.properties.getSizeY())
        self.properties.setSize(dic['sizeX'], dic['sizeY'])

        self.materialTiles = dic['materialTiles']

        for s in dic['entities']:
            e = Entity.Entity()
            e.loadFromDic(s)
            e.addObserver(self.onEntityChanged)
            self.entities.append(e)

        # Load collision groups if they exist.
        if 'collisionGroups' in dic:
            self.collisionGroups = dic['collisionGroups']

        self.isDirty = False
        self.hasSavedOnce = True
        self._notify()

    def getProperties(self): return self.properties

    def setProperties(self, properties):
        deltaX = properties.getSizeX() - self.properties.getSizeX()
        deltaY = properties.getSizeY() - self.properties.getSizeY()
        
        if self.properties.getName() != properties.getName():
            self.hasSavedOnce = False

        self.resizeMaterialTiles(deltaX, deltaY)
        self.properties = properties
        self.isDirty = True
        self._notify()

    def setMaterialTile(self, x, y, material):
        if self.materialTiles[y][x] == material: return

        self.materialTiles[y][x] = material
        self.isDirty = True
        self._notify()

    def getMaterialTile(self, x, y):
        return self.materialTiles[y][x]

    def resizeMaterialTiles(self, deltaX, deltaY):
        # Shrink in Y?
        if deltaY < 0:
            self.materialTiles = self.materialTiles[:deltaY]
        # Grow in Y?
        elif deltaY > 0:
            for i in range(deltaY):
                self.materialTiles.append(["" for tmp in range(self.properties.getSizeX())])
        
        # Shrink in X?
        if deltaX < 0:
            for i in range(self.properties.getSizeY() + deltaY):
                self.materialTiles[i] = self.materialTiles[i][:deltaX]
        elif deltaX > 0:
            for i in range(self.properties.getSizeY() + deltaY):
                self.materialTiles[i].extend(["" for tmp in range(deltaX)])

    def addEntity(self, entity):
        self.entities.append(entity)
        entity.addObserver(self.onEntityChanged)
        self.isDirty = True
        self._notify()

    def removeEntity(self, entity):
        entity.removeObserver(self.onEntityChanged)
        self.entities.remove(entity)
        self.isDirty = True
        self._notify()

    def getEntities(self):
        return self.entities

    def getCollisionGroups(self):
        return self.collisionGroups

    def setCollisionGroups(self, collisionGroups):
        self.collisionGroups = collisionGroups
        self.isDirty = True
        self._notify()
