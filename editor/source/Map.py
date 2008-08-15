import Backdrop
import Entity
import POI

class Map:
    def __init__(self):
        self.name = None
        self.isDirty = True
        self.hasSavedOnce = False
        self.observers = []

        self.backdrop = None
        self.entities = []
        self.collisionGroups = []
        self.pois = []

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

    def onPOIChanged(self, poi):
        self.isDirty = True
        self._notify()

    def saveToFile(self, f):
        output = {'version': 5, 'backdrop': self.backdrop.getName()}
        
        entities = []
        for s in self.entities:
            entities.append(s.saveToDic())
        output['entities'] = entities

        output['collisionGroups'] = self.collisionGroups

        pois = []
        for s in self.pois:
            pois.append(s.saveToDic())
        output['pois'] = pois

        f.write(str(output))

        self.isDirty = False
        self.hasSavedOnce = True
        self._notify()

    def loadFromFile(self, name, f):
        self.name = name
        dic = eval(f.readline())
        if dic['version'] < 5: raise ''

        # Load backdrop.
        self.backdrop = Backdrop.Backdrop(dic['backdrop'])

        # Load entities.
        for s in dic['entities']:
            e = Entity.Entity()
            e.loadFromDic(s)
            e.addObserver(self.onEntityChanged)
            self.entities.append(e)

        # Load collision groups.
        self.collisionGroups = dic['collisionGroups']

        # Load pois.
        for s in dic['pois']:
            p = POI.POI()
            p.loadFromDic(s)
            p.addObserver(self.onPOIChanged)
            self.pois.append(p)

        self.isDirty = False
        self.hasSavedOnce = True
        self._notify()

    def getName(self):
        return self.name

    def setName(self, name):
        self.name = name
        self.isDirty = True
        self._notify()

    def getSize(self):
        return (self.getSizeX(), self.getSizeY())

    def getSizeX(self):
        if self.backdrop: return self.backdrop.getBitmap().GetWidth()
        else: return 0

    def getSizeY(self):
        if self.backdrop: return self.backdrop.getBitmap().GetHeight()
        else: return 0

    def getBackdrop(self):
        return self.backdrop

    def setBackdrop(self, backdrop):
        self.backdrop = backdrop
        self.isDirty = True
        self._notify()

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

    def addPOI(self, poi):
        self.pois.append(poi)
        poi.addObserver(self.onPOIChanged)
        self.isDirty = True
        self._notify()

    def removePOI(self, poi):
        poi.removeObserver(self.onPOIChanged)
        self.pois.remove(poi)
        self.isDirty = True
        self._notify()

    def getPOIs(self):
        return self.pois
