@ECHO OFF
rem echo "compilando codigo FG"
rem bcc -DDOS -w -ml -c fgpci.c > ERROR.TXT
rem echo "Compilando codigo PCI"
rem bcc -DDOS -w -ml -c pcic44.c > ERROR1.TXT
rem echo "Linkando"
rem bcc fgpci.obj pcic44.obj c4xapp.lib lbcfglib.lib pcic44.lib> ERROR2.TXT
rem type ERROR2.txt
bcc -DDOS -w -ml fgpci.c pcic44.c c4xapp.lib lbcfglib.lib pcic44.lib >ERROR.TXT
type ERROR.TXT
PAUSE
