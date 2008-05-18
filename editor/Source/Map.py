import MapProperties

class Map:
    def __init__(self):
        self.properties = MapProperties.MapProperties()
        self.materialTiles = []
        self.isDirty = True
        self.hasSavedOnce = False
        self.observers = []

    def addObserver(self, observer):
        self.observers.append(observer)

    def removeObserver(self, observer):
        self.observers.remove(observer)

    def notify(self):
        for observer in self.observers: observer(self)

    def getIsDirty(self): return self.isDirty

    def getHasSavedOnce(self): return self.hasSavedOnce

    def saveToFile(self, f):
        output = {"version":1, "sizeX": self.properties.getSizeX(), "sizeY": self.properties.getSizeY()}
        output["materialTiles"] = self.materialTiles

        f.write(str(output))

        self.isDirty = False
        self.hasSavedOnce = True
        self.notify()

    def loadFromFile(self, name, f):
        self.properties.setName(name)
        
        dic = eval(f.readline())
        
        if dic['version'] != 1: raise ''
        self.resizeMaterialTiles(dic['sizeX'] - self.properties.getSizeX(), dic['sizeY'] - self.properties.getSizeY())
        self.properties.setSize(dic['sizeX'], dic['sizeY'])

        self.materialTiles = dic['materialTiles']

        self.isDirty = False
        self.hasSavedOnce = True
        self.notify()

    def getProperties(self): return self.properties

    def setProperties(self, properties):
        deltaX = properties.getSizeX() - self.properties.getSizeX()
        deltaY = properties.getSizeY() - self.properties.getSizeY()
        
        if self.properties.getName() != properties.getName():
            self.hasSavedOnce = False

        self.resizeMaterialTiles(deltaX, deltaY)
        self.properties = properties
        self.isDirty = True
        self.notify()

    def setMaterialTile(self, x, y, material):
        self.materialTiles[y][x] = material
        self.isDirty = True
        self.notify()

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
