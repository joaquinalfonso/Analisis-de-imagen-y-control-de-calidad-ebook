@ECHO OFF
echo "compilando codigo DSP"
cl30 -ic:\dsp -v40 video.c cmorf1_d.c -z link.cmd -o cmorf1_d.out > ERROR.txt
type ERROR.txt
PAUSE
