import wx

import MapProperties

class MapPropertiesDialog(wx.Dialog):
    def __init__(self, parent, id, title, properties):
        wx.Dialog.__init__(self, parent, id, title)
        self.nameField, nameBox = self.newField("Name:", properties.getName())
        self.sizeXField, sizeXBox = self.newField("Size X:", str(properties.getSizeX()))
        self.sizeYField, sizeYBox = self.newField("Size Y:", str(properties.getSizeY()))

        line = wx.StaticLine(self, -1, style=wx.LI_HORIZONTAL)

        btnsizer = wx.StdDialogButtonSizer()
        btn = wx.Button(self, wx.ID_OK)
        btn.SetDefault()
        btnsizer.AddButton(btn)
        btn = wx.Button(self, wx.ID_CANCEL)
        btnsizer.AddButton(btn)
        btnsizer.Realize()

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(nameBox, 0, wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL, 5)
        sizer.Add(sizeXBox, 0, wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL, 5)
        sizer.Add(sizeYBox, 0,wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL, 5)
        sizer.Add(line, 0, wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.RIGHT | wx.TOP, 5)
        sizer.Add(btnsizer, 0, wx.ALIGN_CENTER_VERTICAL | wx.ALL, 5)
        self.SetSizer(sizer)
        sizer.Fit(self)

    def getProperties(self):
        properties = MapProperties.MapProperties()
        properties.setName(self.nameField.GetValue())
        properties.setSizeX(int(self.sizeXField.GetValue()))
        properties.setSizeY(int(self.sizeYField.GetValue()))
        return properties

    def newField(self, label, value):
        box = wx.BoxSizer(wx.HORIZONTAL)
        labelText = wx.StaticText(self, -1, label)
        text = wx.TextCtrl(self, -1, value)
        box.Add(labelText, 0, wx.ALIGN_CENTER | wx.ALL, 5)
        box.Add(text, 1, wx.ALIGN_CENTER | wx.ALL, 5)
        text.Bind(wx.EVT_SET_FOCUS, self.onFieldFocus)
        return (text, box)

    def onFieldFocus(self, event):
        event.GetEventObject().SetSelection(-1, -1)
        
