@ECHO OFF
bcc -DDOS -w -ml qpccmeuc.c c4xapp.lib lbcfglib.lib  >ERROR.TXT
type ERROR.TXT
PAUSE
