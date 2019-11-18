@ECHO OFF
bcc -DDOS -w -ml fgpci.c pcic44.c c4xapp.lib lbcfglib.lib pcic44.lib >ERROR.TXT
type ERROR.TXT
PAUSE
