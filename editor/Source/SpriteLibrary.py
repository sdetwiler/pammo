import Sprite

sprites = {}

def getSprite(name):
    global sprites
    if name in sprites:
        return sprites[name]
    else:
        sprite = Sprite.Sprite(name)
        sprites[name] = sprite
        return sprite
