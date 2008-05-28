import Prop

props = {}

def getProp(name):
    global props
    if name in props:
        return props[name]
    else:
        prop = Prop.Prop(name)
        props[name] = prop
        return prop

