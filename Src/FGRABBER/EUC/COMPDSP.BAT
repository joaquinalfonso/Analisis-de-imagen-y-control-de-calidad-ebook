@ECHO OFF
echo "compilando codigo DSP"
cl30 -ic:\dsp -v40 cmeuc.c -z link.cmd -o cmeuc.out
rem ..\..\cl\cl30 -i..\..\cl -v40 video.c cmeuc.c -z link.cmd -o cmeuc.out > ERROR.txt
PAUSE
