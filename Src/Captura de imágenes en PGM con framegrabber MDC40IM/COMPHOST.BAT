@ECHO OFF
echo "compilando codigo HOST"
bcc -Ic:\proyec~1\c40board\app_lib -Lc:\proyec~1\c40board\app_lib\bor -ml capt_h.c c4xapp.lib lbcfglib.lib > ERROR.txt
type ERROR.txt
PAUSE
