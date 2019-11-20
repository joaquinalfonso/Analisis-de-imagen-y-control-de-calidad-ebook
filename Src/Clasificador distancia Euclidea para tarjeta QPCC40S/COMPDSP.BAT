@ECHO OFF
echo Compilando codigo DSP
cl30 -ic:\dsp -v40 video.c cldist_d.c -z link.cmd -o cldist_d.out > ERROR.txt
type ERROR.txt
PAUSE
