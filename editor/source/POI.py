PlayerStartTypeName = 'Player Start'
SpawnPoint1TypeName = 'Spawn Point 1'
SpawnPoint2TypeName = 'Spawn Point 2'
SpawnPoint3TypeName = 'Spawn Point 3'
SpawnPoint4TypeName = 'Spawn Point 4'
SpawnPoint5TypeName = 'Spawn Point 5'

TypeNames = [PlayerStartTypeName,
             SpawnPoint1TypeName,
             SpawnPoint2TypeName,
             SpawnPoint3TypeName,
             SpawnPoint4TypeName,
             SpawnPoint5TypeName]

PlayerStartTypeIndex = TypeNames.index(PlayerStartTypeName)
SpawnPoint1TypeIndex = TypeNames.index(SpawnPoint1TypeName)
SpawnPoint2TypeIndex = TypeNames.index(SpawnPoint2TypeName)
SpawnPoint3TypeIndex = TypeNames.index(SpawnPoint3TypeName)
SpawnPoint4TypeIndex = TypeNames.index(SpawnPoint4TypeName)
SpawnPoint5TypeIndex = TypeNames.index(SpawnPoint5TypeName)

class POI:
    def __init__(self, pos=(0, 0), type=SpawnPoint1TypeName):
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
