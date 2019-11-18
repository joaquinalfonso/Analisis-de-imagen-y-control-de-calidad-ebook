@echo off

echo "++++++++++++++++++++++ compilando codigo DSP1 +++++++++++++++++++++++"

cl30 -ic:\dsp -v44 cm.c -z link.cmd -o cm.out > error1.txt
type error1.txt

pause

echo "+++++++++++++++++++++ compilando codigo DSP ++++++++++++++++++++++++"

cl30 -ic:\dsp -v44 mao.c -z link.cmd -o mao.out > error2.txt

type error2.txt


