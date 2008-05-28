import wx

import os

import Map
import MapEditor
import MapProperties
import MapPropertiesDialog
import MapPreviewDialog
import ToolBrowser
import BinaryMap

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
        self.deleteMenu = 105
        self.propertiesMenu = 106
        self.previewMenu = 107
        self.Bind(wx.EVT_MENU, self.onNewMenu, id=self.newMenu)
        self.Bind(wx.EVT_MENU, self.onOpenMenu, id=self.openMenu)
        self.Bind(wx.EVT_MENU, self.onCloseMenu, id=self.closeMenu)
        self.Bind(wx.EVT_MENU, self.onSaveMenu, id=self.saveMenu)
        self.Bind(wx.EVT_MENU, self.onDeleteMenu, id=self.deleteMenu)
        self.Bind(wx.EVT_MENU, self.onPropertiesMenu, id=self.propertiesMenu)
        self.Bind(wx.EVT_MENU, self.onPreviewMenu, id=self.previewMenu)
        fileMenu = wx.Menu()
        fileMenu.Append(self.newMenu, "&New Map...\tCtrl+N", "")
        fileMenu.Append(self.openMenu, "&Open Map...\tCtrl+O", "")
        fileMenu.Append(self.closeMenu, "&Close Map\tCtrl+W", "")
        fileMenu.AppendSeparator()
        fileMenu.Append(self.saveMenu, "&Save Map\tCtrl+S", "")
        fileMenu.AppendSeparator()
        fileMenu.Append(self.deleteMenu, "&Delete Map", "")
        fileMenu.AppendSeparator()
        fileMenu.Append(self.propertiesMenu, "&Map Properties...\tCtrl+P", "")
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

        #f = open("/Users/jmarr/Desktop/Pammo/editor/maps/Desert.map")
        #map = Map.Map()
        #map.loadFromFile("Desert", f)
        #self.newEditorForMap(map)

    def onNewMenu(self, event):
        properties = MapProperties.MapProperties()
        properties.setSize(10, 10)

        # Find a unique name in this notebook.
        nameExtra = 2
        while not self.isUniqueMapName(properties.getName()):
            properties.setName(properties.getName().split(" ")[0] + " " + str(nameExtra))
            nameExtra += 1

        # Show dialog.
        dialog = MapPropertiesDialog.MapPropertiesDialog(self, -1, "New Map", properties)
        dialog.CenterOnScreen()
        res = dialog.ShowModal()
        dialog.Destroy()
        
        if res == wx.ID_OK:
            properties = dialog.getProperties()

            # Verify that this is a unique name.
            if not self.isUniqueMapName(properties.getName()):
                self.tellAlreadyEditing(properties.getName())
                return

            map = Map.Map()
            map.setProperties(properties)
            self.newEditorForMap(map)

    def onOpenMenu(self, event):
        base = os.path.abspath(os.getcwd()+"/maps")
        dialog = wx.FileDialog(self,
            message="Choose a map (This folder only!)",
            defaultDir=base, 
            defaultFile="",
            wildcard="Pammo Map (*.map)|*.map",
            style=wx.OPEN)
        res = dialog.ShowModal()
        if res != wx.ID_OK: return

        path = dialog.GetPath()
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

    def onDeleteMenu(self, event):
        index = self.mapNotebook.GetSelection()
        editor = self.mapNotebook.GetPage(index)
        if not self.askDeleteMap(editor.getMap()): return
        os.remove(os.getcwd() + '/maps/%s.map' % editor.getMap().getProperties().getName())
        self.mapNotebook.RemovePage(index)
        self.onMapNotebookPageChanged(None)
        editor.Destroy()

    def onPropertiesMenu(self, event):
        editor = self.mapNotebook.GetPage(self.mapNotebook.GetSelection())
        dialog = MapPropertiesDialog.MapPropertiesDialog(self, -1, "Map Properties", editor.getMap().getProperties())
        dialog.CenterOnScreen()
        res = dialog.ShowModal()
        
        if res == wx.ID_OK:
            properties = dialog.getProperties()

            # If the name changed, verify it is unique.
            if editor.getMap().getProperties().getName() != properties.getName() and not self.isUniqueMapName(properties.getName()):
                self.tellAlreadyEditing(properties.getName())
                return

            editor.getMap().setProperties(properties)

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
            cur = self.mapNotebook.GetPage(i).getMap().getProperties().getName()
            if cur == name: return False
        return True

    def showMessage(self, message):
        dialog = wx.MessageDialog(self, '', message, wx.OK | wx.ICON_EXCLAMATION)
        res = dialog.ShowModal()
        dialog.Destroy()

    def askSaveMap(self, map, caption):
        dialog = wx.MessageDialog(self, 'Map "%s" has been modified.' % map.getProperties().getName(),
                                  caption, wx.YES_NO | wx.CANCEL | wx.ICON_QUESTION)
        res = dialog.ShowModal()
        dialog.Destroy()
        if res == wx.ID_CANCEL:
            return False
        if res == wx.ID_NO:
            return True
        if res == wx.ID_YES:
            return self.trySaveMap(map)

    def askDeleteMap(self, map):
        dialog = wx.MessageDialog(self, '', ('Are you sure you want to delete map "%s"?' % map.getProperties().getName()),
                                  wx.YES_NO | wx.ICON_QUESTION)
        res = dialog.ShowModal()
        dialog.Destroy()
        if res == wx.ID_YES: return True
        else: return False

    def tellAlreadyEditing(self, name):
        self.showMessage('You are already editing a map named "%s".' % name)

    def trySaveMap(self, map):
        path = os.getcwd() + '/maps/%s.map' % map.getProperties().getName()
        if not map.getHasSavedOnce():
            if os.path.exists(path):
                dialog = wx.MessageDialog(self, '',
                    'A map named "%s" already exists. Do you want to replace it?' % map.getProperties().getName(),
                    wx.YES_NO | wx.ICON_QUESTION)
                res = dialog.ShowModal()
                dialog.Destroy()
                if res == wx.ID_NO: return False
        f = open(path, "w")
        map.saveToFile(f)
        try: BinaryMap.save(map)
        except: self.showMessage('Could not save map in client data folder')
        return True

    def updateMenuState(self):
        saveMenu = self.menuBar.FindItemById(self.saveMenu)
        closeMenu = self.menuBar.FindItemById(self.closeMenu)
        deleteMenu = self.menuBar.FindItemById(self.deleteMenu)
        propertiesMenu = self.menuBar.FindItemById(self.propertiesMenu)
        previewMenu = self.menuBar.FindItemById(self.previewMenu)

        if self.mapNotebook.GetPageCount() == 0:
            saveMenu.Enable(False)
            closeMenu.Enable(False)
            deleteMenu.Enable(False)
            propertiesMenu.Enable(False)
            previewMenu.Enable(False)
            return

        closeMenu.Enable(True)
        propertiesMenu.Enable(True)
        previewMenu.Enable(True)
        
        editor = self.mapNotebook.GetPage(self.mapNotebook.GetSelection())
        if editor.getMap().getIsDirty():
            saveMenu.Enable(True)
            deleteMenu.Enable(False)
        else:
            saveMenu.Enable(False)
            deleteMenu.Enable(True)

    def onMapChanged(self, map):
        self.updateMapPageName(map)
        self.updateMenuState()

    def updateMapPageName(self, map):
        i = self.findMapNotebookIndex(map)
        assert(i != None)
        if map.getIsDirty():
            self.mapNotebook.SetPageText(i, "* " + map.getProperties().getName())
        else:
            self.mapNotebook.SetPageText(i, map.getProperties().getName())

    def findMapNotebookIndex(self, map):
        for i in range(self.mapNotebook.GetPageCount()):
            editor = self.mapNotebook.GetPage(i)
            if editor.getMap() == map: return i
        return None
