import wx

import os
import osfix

import Map
import MapEditor
import MapPreviewDialog
import ToolBrowser
import BinaryMap
import Backdrop

class MapEditorPanel(wx.Panel):
    def __init__(self, parent, id):
        wx.Panel.__init__(self, parent, id)
        self.browser = ToolBrowser.ToolBrowser(self, -1)
        self.mapNotebook = wx.Notebook(self, -1)
        self.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGED, self.onMapNotebookPageChanged, self.mapNotebook)

        leftSizer = wx.BoxSizer(wx.HORIZONTAL)
        leftSizer.Add(self.browser, 0, wx.EXPAND)
        leftSizer.Add(self.mapNotebook, 1, wx.EXPAND)
        self.SetSizer(leftSizer)

        # Build menu bar.
        self.newMenu = 101
        self.openMenu = 102
        self.closeMenu = 103
        self.saveMenu = 104
        self.saveAsMenu = 105
        self.backdropMenu = 106
        self.previewMenu = 107
        self.Bind(wx.EVT_MENU, self.onNewMenu, id=self.newMenu)
        self.Bind(wx.EVT_MENU, self.onOpenMenu, id=self.openMenu)
        self.Bind(wx.EVT_MENU, self.onCloseMenu, id=self.closeMenu)
        self.Bind(wx.EVT_MENU, self.onSaveMenu, id=self.saveMenu)
        self.Bind(wx.EVT_MENU, self.onSaveAsMenu, id=self.saveAsMenu)
        self.Bind(wx.EVT_MENU, self.onBackdropMenu, id=self.backdropMenu)
        self.Bind(wx.EVT_MENU, self.onPreviewMenu, id=self.previewMenu)
        fileMenu = wx.Menu()
        fileMenu.Append(self.newMenu, "&New Map...\tCtrl+N", "")
        fileMenu.Append(self.openMenu, "&Open Map...\tCtrl+O", "")
        fileMenu.Append(self.closeMenu, "&Close Map\tCtrl+W", "")
        fileMenu.AppendSeparator()
        fileMenu.Append(self.saveMenu, "&Save\tCtrl+S", "")
        fileMenu.Append(self.saveAsMenu, "&Save As...\tCtrl+Shift+S", "")
        fileMenu.AppendSeparator()
        fileMenu.Append(self.backdropMenu, "&Change Backdrop...\tCtrl+B", "")
        fileMenu.Append(self.previewMenu, "&iPhone View...\tCtrl+I", "")
        fileMenu.AppendSeparator()
        fileMenu.Append(wx.ID_EXIT, "&Quit\tCtrl+Q", "")
        self.menuBar = wx.MenuBar()
        self.menuBar.Append(fileMenu, "&File")

        self.selectedEditor = None

        # Hack Shit
        #properties = MapProperties.MapProperties()
        #properties.setSize(10, 10)
        #map = Map.Map()
        #map.setProperties(properties)
        #self.newEditorForMap(map)

        #f = open("/Users/jmarr/Desktop/Irradiated/data/maps/TheMap.map")
        #map = Map.Map()
        #map.loadFromFile("Desert", f)
        #self.newEditorForMap(map)

    def onNewMenu(self, event):
        backdrop = self.askChooseBackdrop()
        if not backdrop: return2
        
        # Find a unique name in this notebook.
        nameExtra = 2
        name = "Untitled"
        while not self.isUniqueMapName(name):
            name = name.split(" ")[0] + " " + str(nameExtra)
            nameExtra += 1

        map = Map.Map()
        map.setName(name)
        map.setBackdrop(backdrop)
        self.newEditorForMap(map)

    def onOpenMenu(self, event):
        base = osfix.path("../data/maps")
        dialog = wx.FileDialog(self,
            message="Open a map (This folder only!)",
            defaultDir=base, 
            defaultFile="",
            wildcard="Irradiated Map (*.map)|*.map",
            style=wx.OPEN)
        res = dialog.ShowModal()
        if res != wx.ID_OK: return

        path = osfix.path(dialog.GetPath())
        (newBase, name) = os.path.split(path)

        if newBase != base or name[-4:] != '.map':
            self.showMessage('Invalid map file.')
            return
        name = name[:-4]
        if not self.isUniqueMapName(name):
            self.tellAlreadyEditing(name)
            return

        f = open(path, "r")
        map = Map.Map()
        #map.loadFromFile(name, f)
        try:
            map.loadFromFile(name, f)
        except:
            self.showMessage('There was an error loading map "%s".' % name)
            return
        self.newEditorForMap(map)

    def onCloseMenu(self, event):
        index = self.mapNotebook.GetSelection()
        editor = self.mapNotebook.GetPage(index)
        if editor.getMap().getIsDirty():
            if not self.askSaveMap(editor.getMap(), "Save changes before closing?"): return
        self.mapNotebook.RemovePage(index)
        self.onMapNotebookPageChanged(None)
        editor.Destroy()

    def onSaveMenu(self, event):
        editor = self.mapNotebook.GetPage(self.mapNotebook.GetSelection())
        self.trySaveMap(editor.getMap())

    def onSaveAsMenu(self, event):
        editor = self.mapNotebook.GetPage(self.mapNotebook.GetSelection())
        self.trySaveMapAs(editor.getMap())

    def onBackdropMenu(self, event):
        backdrop = self.askChooseBackdrop()
        if not backdrop: return
        self.mapNotebook.GetPage(self.mapNotebook.GetSelection()).getMap().setBackdrop(backdrop)

    def onPreviewMenu(self, event):
        editor = self.mapNotebook.GetPage(self.mapNotebook.GetSelection())
        preview = MapPreviewDialog.MapPreviewDialog(self, -1, editor.getMap())
        preview.ShowModal()
        preview.Destroy()

    def onMapNotebookPageChanged(self, event):
        index = self.mapNotebook.GetSelection()
        if index == -1:
            self.selectedEditor = None
        else:
            self.selectedEditor = self.mapNotebook.GetPage(self.mapNotebook.GetSelection())
        self.updateMenuState()
        self.browser.onEditorChanged(self.selectedEditor)

    def onTimeToQuit(self):
        for i in range(self.mapNotebook.GetPageCount()):
            editor = self.mapNotebook.GetPage(i)
            if editor.getMap().getIsDirty():
                if not self.askSaveMap(editor.getMap(), "Save changes before quiting?"): return False
        while self.mapNotebook.GetPageCount():
            self.mapNotebook.RemovePage(0)
        return True

    def onPanelSelected(self, frame):
        frame.SetMenuBar(self.menuBar)
        self.updateMenuState()

    def newEditorForMap(self, map):
        map.addObserver(self.onMapChanged)
        mapEditor = MapEditor.MapEditor(self.mapNotebook, -1, map)
        self.mapNotebook.AddPage(mapEditor, "", select=True)
        self.updateMenuState()
        self.updateMapPageName(map)

    def isUniqueMapName(self, name, skipName=None):
        for i in range(self.mapNotebook.GetPageCount()):
            cur = self.mapNotebook.GetPage(i).getMap().getName()
            if cur == name: return False
        return True

    def showMessage(self, message):
        dialog = wx.MessageDialog(self, '', message, wx.OK | wx.ICON_EXCLAMATION)
        res = dialog.ShowModal()
        dialog.Destroy()

    def askChooseBackdrop(self):
        base = osfix.path("../data/backdrops")
        dialog = wx.FileDialog(self,
            message="Choose a backdrop (This folder only!)",
            defaultDir=base, 
            defaultFile="",
            wildcard="PNG (*.png)|*.png",
            style=wx.OPEN)
        res = dialog.ShowModal()
        if res != wx.ID_OK: return
        path = dialog.GetPath()
        dialog.Destroy()
        
        name = Backdrop.pathToName(path)
        if not name:
            self.showMessage('Invalid backdrop "%s"' % path)
            return None
        return Backdrop.Backdrop(name)

    def askSaveMap(self, map, caption):
        dialog = wx.MessageDialog(self, 'Map "%s" has been modified.' % map.getName(),
                                  caption, wx.YES_NO | wx.CANCEL | wx.ICON_QUESTION)
        res = dialog.ShowModal()
        dialog.Destroy()
        if res == wx.ID_CANCEL:
            return False
        if res == wx.ID_NO:
            return True
        if res == wx.ID_YES:
            return self.trySaveMap(map)

    def tellAlreadyEditing(self, name):
        self.showMessage('Map "%s" is already open.' % name)

    def trySaveMap(self, map):
        if not map.getHasSavedOnce():
            return self.trySaveMapAs(map)

        self.saveMap(map)
        return True

    def trySaveMapAs(self, map):
        base = osfix.path("../data/maps")
        dialog = wx.FileDialog(self,
            message="Save a map (This folder only!)",
            defaultDir=base, 
            defaultFile=map.getName() + '.map',
            wildcard="Irradiated Map (*.map)|*.map",
            style=wx.SAVE)
        res = dialog.ShowModal()
        if res != wx.ID_OK: return False
        path = dialog.GetPath()
        dialog.Destroy()

        path = osfix.path(path)
        if path[-4:] != '.map': path = path + '.map'
        (newBase, name) = os.path.split(path)
        if newBase != base:
            self.showMessage('Invalid map location.')
            return False
        
        if os.path.exists(path):
            dialog = wx.MessageDialog(self, '',
                'A map named "%s" already exists. Do you want to replace it?' % map.getName(),
                wx.YES_NO | wx.ICON_QUESTION)
            res = dialog.ShowModal()
            dialog.Destroy()
            if res == wx.ID_NO: return False

        map.setName(path[len(base)+1:-4])
        self.saveMap(map)

    def saveMap(self, map):
        path = osfix.path('../data/maps/%s.map' % map.getName())
        f = open(path, "w")
        map.saveToFile(f)
        #try: BinaryMap.save(map)
        #except: self.showMessage('Could not save map in binary format')
        BinaryMap.save(map)

    def updateMenuState(self):
        saveMenu = self.menuBar.FindItemById(self.saveMenu)
        saveAsMenu = self.menuBar.FindItemById(self.saveAsMenu)
        closeMenu = self.menuBar.FindItemById(self.closeMenu)
        backdropMenu = self.menuBar.FindItemById(self.backdropMenu)
        previewMenu = self.menuBar.FindItemById(self.previewMenu)

        if self.mapNotebook.GetPageCount() == 0:
            saveMenu.Enable(False)
            saveAsMenu.Enable(False)
            closeMenu.Enable(False)
            backdropMenu.Enable(False)
            previewMenu.Enable(False)
            return

        saveAsMenu.Enable(True)
        closeMenu.Enable(True)
        backdropMenu.Enable(True)
        previewMenu.Enable(True)
        
        editor = self.mapNotebook.GetPage(self.mapNotebook.GetSelection())
        if editor.getMap().getIsDirty():
            saveMenu.Enable(True)
        else:
            saveMenu.Enable(False)

    def onMapChanged(self, map):
        self.updateMapPageName(map)
        self.updateMenuState()

    def updateMapPageName(self, map):
        i = self.findMapNotebookIndex(map)
        assert(i != None)
        if map.getIsDirty():
            self.mapNotebook.SetPageText(i, "* " + map.getName())
        else:
            self.mapNotebook.SetPageText(i, map.getName())

    def findMapNotebookIndex(self, map):
        for i in range(self.mapNotebook.GetPageCount()):
            editor = self.mapNotebook.GetPage(i)
            if editor.getMap() == map: return i
        return None
