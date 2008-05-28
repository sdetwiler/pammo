import wx, string

class NumValidator(wx.PyValidator):
    def __init__(self):
        wx.PyValidator.__init__(self)
        self.Bind(wx.EVT_CHAR, self.onChar)

    def Clone(Self):
        return NumValidator()
    
    def onChar(self, event):
        key = event.GetKeyCode()
        cur = self.GetWindow().GetValue()

        if cur.find(".") == -1 and key == ord('.'):
            event.Skip()
            return

        if key < wx.WXK_SPACE or key == wx.WXK_DELETE or key > 255:
            event.Skip()
            return

        if chr(key) in string.digits:
            event.Skip()
            return
