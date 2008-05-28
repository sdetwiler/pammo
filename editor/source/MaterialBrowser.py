import wx

import MaterialLibrary

border = 8

class MaterialBrowser(wx.VListBox):
    def __init__(self, parent, id):
        wx.VListBox.__init__(self, parent, id, size=(MaterialLibrary.getMaterialSize()+border*2 + 14, 100), style=wx.BORDER_SUNKEN)
        self.Bind(wx.EVT_LISTBOX, self.OnSelected, self)

        self.observers = []

        MaterialLibrary.addObserver(self)
        self.OnMaterialLibraryUpdated()

        self.selectedMaterial = self.materials[self.GetSelection()]

    def addObserver(self, observer): self.observers.append(observer)

    def removeObserver(self, observer): self.observers.remove(observer)

    def notify(self):
        for observer in self.observers: observer(self)

    def getSelectedMaterial(self):
        return self.selectedMaterial

    def OnDrawItem(self, dc, rect, n):
        size = MaterialLibrary.getMaterialSize()
        left = rect[0] + rect[2]/2 - size/2
        top = rect[1] + rect[3]/2 - size/2
        dc.DrawBitmap(self.materials[n].getBitmap(),left, top)

    def OnMeasureItem(self, n):
        return MaterialLibrary.getMaterialSize() + border
        
    def OnMaterialLibraryUpdated(self):
        self.materials = [MaterialLibrary.getMaterial(material) for material in MaterialLibrary.getAllMaterials()]
        self.SetItemCount(len(self.materials))
        self.SetSelection(0)
        self.Refresh()

    def OnSelected(self, event):
        self.selectedMaterial = self.materials[event.GetSelection()]
        self.notify()
