@ECHO OFF
echo Compilando codigo HOST
bcc -Ic:\proyec~1\fgrabber\qpccmeuc -Lc:\proyec~1\fgrabber\qpccmeuc -ml cldist_h.c c4xapp.lib lbcfglib.lib > ERROR.txt
type ERROR.txt
PAUSE
