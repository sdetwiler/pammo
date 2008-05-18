import os
import Material

import wx

materials = {}
materialTransitions = {}
observers = []

def reloadFromFilesystem():
    global materials
    global materialTransitions
    materials = {}
    materialTransitions = {}
    materialsDir = 'Materials'
    for f in os.listdir(materialsDir):
        m = Material.tryLoad(materialsDir + '/' + f)
        if not m:continue 
        name = m.getName()
        materials[name] = m
        materialTransitions[makeTransitionName(name, name, name, name)] = m.getBitmap()

    materialTransitionsDir = 'Material-Transitions'
    for f in os.listdir(materialTransitionsDir):
        m0, m1 = f[:-4].split('-')
        loadTransition(materialTransitionsDir + '/' + f, m0, m1)

    _notifyObservers()

def makeTransitionName(m0, m1, m2, m3):
    return '%s-%s-%s-%s' % (m0, m1, m2, m3)

def makeTransition(image, x, y, m0, m1, m2, m3):
    size = getMaterialSize()
    name = makeTransitionName(m0, m1, m2, m3)
    bitmap = image.GetSubBitmap((x*size, y*size, size, size))
    materialTransitions[name] = bitmap

def loadTransition(path, m0, m1):
    bitmap = wx.Image(path, wx.BITMAP_TYPE_PNG).ConvertToBitmap()

    makeTransition(bitmap, 0, 0, m0, m0, m0, m1)
    makeTransition(bitmap, 1, 0, m0, m0, m1, m1)
    makeTransition(bitmap, 2, 0, m0, m0, m1, m0)
    makeTransition(bitmap, 4, 0, m1, m1, m1, m0)
    makeTransition(bitmap, 5, 0, m1, m1, m0, m1)
    makeTransition(bitmap, 0, 1, m0, m1, m0, m1)
    makeTransition(bitmap, 2, 1, m1, m0, m1, m0)
    makeTransition(bitmap, 4, 1, m1, m0, m1, m1)
    makeTransition(bitmap, 5, 1, m0, m1, m1, m1)
    makeTransition(bitmap, 0, 2, m0, m1, m0, m0)
    makeTransition(bitmap, 1, 2, m1, m1, m0, m0)
    makeTransition(bitmap, 2, 2, m1, m0, m0, m0)
    makeTransition(bitmap, 4, 2, m1, m0, m0, m1)
    makeTransition(bitmap, 5, 2, m0, m1, m1, m0)

def getAllMaterials():
    global materials
    return materials.keys()

def getMaterial(name):
    if name in materials:
        return materials[name]
    else:
        return None

def getMaterialTransition(m0, m1, m2, m3):
    global materialTransitions
    fullName = makeTransitionName(m0, m1, m2, m3)
    if fullName in materialTransitions:
        return materialTransitions[fullName]
    else:
        return None

def getMaterialSize():
    return 32

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

