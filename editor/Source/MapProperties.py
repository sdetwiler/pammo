class MapProperties:
    def __init__(self):
        self.name = "Untitled"
        self.sizeX = 0
        self.sizeY = 0

    def getName(self): return self.name
    def setName(self, name): self.name = name
    
    def getSizeX(self): return self.sizeX
    def setSizeX(self, x): self.sizeX = x
    
    def getSizeY(self): return self.sizeY
    def setSizeY(self, y): self.sizeY = y
    
    def getSize(self): return (self.sizeX, self.sizeY)
    def setSize(self, x, y): self.sizeX, self.sizeY = (x, y)

