@ECHO OFF
echo Compilando codigo DSP
cl30 -ic:\dsp -v40 video.c clmao_d.c -z link.cmd -o clmao_d.out > ERROR.txt
type ERROR.txt
PAUSE
