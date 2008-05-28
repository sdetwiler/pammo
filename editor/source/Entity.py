import PropLibrary

class Entity:
    def __init__(self, prop='', pos=(0, 0), scale=1, rot=0):
        self.prop = prop
        self.pos = pos
        self.scale = scale
        self.rot = rot

        self.observers = []

    def addObserver(self, observer): self.observers.append(observer)
    def removeObserver(self, observer): self.observers.remove(observer)

    def _notify(self):
        for observer in self.observers: observer(self)

    def saveToDic(self):
        data = {}
        data['prop'] = self.prop
        data['pos'] = self.pos
        data['scale'] = self.scale
        data['rot'] = self.rot
        return data

    def loadFromDic(self, data):
        self.prop = data['prop']
        self.pos = data['pos']
        self.scale = data['scale']
        self.rot = data['rot']
        self._notify()

    def getProp(self): return PropLibrary.getProp(self.prop)

    def setSprite(self, prop):
        notify = False
        if self.prop != prop: notify = True
        self.prop = prop
        if notify: self._notify()

    def getPos(self): return self.pos
    def setPos(self, pos):
        notify = False
        if self.pos != pos: notify = True
        self.pos = pos
        if notify: self._notify()

    def getScale(self): return self.scale
    def setScale(self, scale):
        notify = False
        if self.scale != scale: notify = True
        self.scale = scale
        if notify: self._notify()

    def getRot(self): return self.rot
    def setRot(self, rot):
        notify = False
        if self.rot != rot: notify = True
        self.rot = rot
        if notify: self._notify()

    def getRect(self):
        size = (self.getProp().getBitmap().GetWidth(), self.getProp().getBitmap().GetHeight())
        return(self.pos[0] - size[0]/2*self.scale,
        self.pos[1] - size[1]/2*self.scale,
        self.pos[0] + size[0]/2*self.scale,
        self.pos[1] + size[1]/2*self.scale)
