class CollisionShape:
    def __init__(self):
        self.points = []
        self.playerCollide = True
        self.enemiesCollide = True

        self.observers = []

    def addObserver(self, observer): self.observers.append(observer)
    def removeObserver(self, observer): self.observers.remove(observer)

    def _notify(self):
        for observer in self.observers: observer(self)

    def saveToDic(self):
        data = {}
        data['points'] = self.points
        data['playerCollide'] = self.playerCollide
        data['enemiesCollide'] = self.enemiesCollide
        return data

    def loadFromDic(self, data):
        self.points = data['points']
        self.playerCollide = data['playerCollide']
        self.enemiesCollide = data['enemiesCollide']

    def getPoints(self): return self.points
    def setPoints(self, points):
        self.points = points
        self._notify()

    def getPlayerCollide(self): return self.playerCollide
    def setPlayerCollide(self, playerCollide):
        self.playerCollide = playerCollide
        self._notify()

    def getEnemiesCollide(self): return self.enemiesCollide
    def setEnemiesCollide(self, enemiesCollide):
        self.enemiesCollide = enemiesCollide
        self._notify()
