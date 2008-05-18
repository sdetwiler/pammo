import os
import Material

import wx

materials = {}
observers = []

def reloadFromFilesystem():
    global materials
    materials = {}
    materialsDir = 'Materials'
    for f in os.listdir(materialsDir):
        m = Material.tryLoad(materialsDir + '/' + f)
        if not m:continue 
        name = m.getName()
        materials[name] = m

    _notifyObservers()

def getAllMaterials():
    global materials
    return materials.keys()

def getMaterial(name):
    if name in materials:
        return materials[name]
    else:
        return None

def getMaterialSize():
    return 128

def addObserver(observer):
    global observers
    observers.append(observer)

def removeObserver(observer):
    global observers
    observers.remove(observer)

def _notifyObservers():
    global observers
    for observer in observers:
        observer.OnMaterialLibraryUpdated()

