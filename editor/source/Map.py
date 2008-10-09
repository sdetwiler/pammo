import Backdrop
import Entity
import CollisionShape
import POI

class Map:
    def __init__(self):
        self.name = None
        self.isDirty = True
        self.hasSavedOnce = False
        self.observers = []

        self.backdrop = None
        self.entities = []
        self.collisionShapes = []
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

    def onCollisionShapeChanged(self, shape):
        self.isDirty = True
        self._notify()

    def onPOIChanged(self, poi):
        self.isDirty = True
        self._notify()

    def saveToFile(self, f):
        output = {'version': 7, 'backdrop': self.backdrop.getName(), 'backdrophash': self.backdrop.getHash()}
        
        entities = []
        for s in self.entities: entities.append(s.saveToDic())
        output['entities'] = entities

        collisionShapes = []
        for s in self.collisionShapes: collisionShapes.append(s.saveToDic())
        output['collisionShapes'] = collisionShapes

        pois = []
        for s in self.pois: pois.append(s.saveToDic())
        output['pois'] = pois

        f.write(str(output))

        self.isDirty = False
        self.hasSavedOnce = True
        self._notify()

    def loadFromFile(self, name, f):
        self.name = name
        dic = eval(f.readline())
        if dic['version'] < 6: raise ''

        # Load backdrop.
        self.backdrop = Backdrop.Backdrop(dic['backdrop'])
        
        # Check hash.
        newhash = self.backdrop.getHash()
        if 'backdrophash' in dic: oldhash = dic['backdrophash']
        else: oldhash = ''

        # Load entities.
        for d in dic['entities']:
            e = Entity.Entity()
            e.loadFromDic(d)
            e.addObserver(self.onEntityChanged)
            self.entities.append(e)

        # Load collision shapes.
        for d in dic['collisionShapes']:
            s = CollisionShape.CollisionShape()
            s.loadFromDic(d)
            s.addObserver(self.onCollisionShapeChanged)
            self.collisionShapes.append(s)

        # Load pois.
        for d in dic['pois']:
            p = POI.POI()
            p.loadFromDic(d)
            p.addObserver(self.onPOIChanged)
            self.pois.append(p)

        if newhash == oldhash:
            self.isDirty = False
        else:
            self.isDirty = True
            
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

    def addCollisionShape(self, collisionShape):
        self.collisionShapes.append(collisionShape)
        collisionShape.addObserver(self.onCollisionShapeChanged)
        self.isDirty = True
        self._notify()

    def removeCollisionShape(self, collisionShape):
        collisionShape.removeObserver(self.onCollisionShapeChanged)
        self.collisionShapes.remove(collisionShape)
        self.isDirty = True
        self._notify()

    def getCollisionShapes(self):
        return self.collisionShapes

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
