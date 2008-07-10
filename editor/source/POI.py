SpawnPointTypeName = 'Spawn Point'
DisabledTypeName = 'Disabled'

TypeNames = [SpawnPointTypeName, DisabledTypeName]

SpawnPointTypeIndex = TypeNames.index(SpawnPointTypeName)
DisabledTypeIndex = TypeNames.index(DisabledTypeName)

class POI:
    def __init__(self, pos=(0, 0), type=SpawnPointTypeName):
        self.pos = pos
        self.type = type

        self.observers = []

    def addObserver(self, observer): self.observers.append(observer)
    def removeObserver(self, observer): self.observers.remove(observer)

    def _notify(self):
        for observer in self.observers: observer(self)

    def saveToDic(self):
        data = {}
        data['pos'] = self.pos
        data['type'] = self.type
        return data

    def loadFromDic(self, data):
        self.pos = data['pos']
        self.type = data['type']

    def getPos(self): return self.pos
    def setPos(self, pos):
        self.pos = pos
        self._notify()

    def getType(self): return self.type
    def setType(self, type):
        self.type = type
        self._notify()
