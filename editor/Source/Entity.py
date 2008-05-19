
class Entity:
    def __init__(self, sprite="", pos=(0, 0), scale=1, rot=0):
        self.sprite = sprite
        self.pos = pos
        self.scale = scale
        self.rot = rot

    def getSprite(self): return self.sprite
    def setSprite(self, sprite): self.sprite = sprite

    def getPos(self): return self.pos
    def setPos(self, pos): self.pos = pos

    def getScale(self): return self.scale
    def setScale(self, scale): self.scale = scale

    def getRot(self): return self.rot
    def setRot(self, rot): self.rot = rot
