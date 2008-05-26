import SpriteLibrary

class Entity:
    def __init__(self, sprite='', pos=(0, 0), scale=1, rot=0):
        self.sprite = sprite
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
        data['sprite'] = self.sprite
        data['pos'] = self.pos
        data['scale'] = self.scale
        data['rot'] = self.rot
        return data

    def loadFromDic(self, data):
        self.sprite = data['sprite']
        self.pos = data['pos']
        self.scale = data['scale']
        self.rot = data['rot']
        self._notify()

    def getSprite(self): return SpriteLibrary.getSprite(self.sprite)

    def setSprite(self, sprite):
        notify = False
        if self.sprite != sprite: notify = True
        self.sprite = sprite
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
        size = (self.getSprite().getBitmap().GetWidth(), self.getSprite().getBitmap().GetHeight())
        return(self.pos[0] - size[0]/2*self.scale,
        self.pos[1] - size[1]/2*self.scale,
        self.pos[0] + size[0]/2*self.scale,
        self.pos[1] + size[1]/2*self.scale)
