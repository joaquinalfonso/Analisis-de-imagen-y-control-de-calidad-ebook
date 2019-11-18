@ECHO OFF
echo "compilando codigo DSP"
cl30 -ic:\dsp -v40 video.c cmorf_d.c -z link.cmd -o cmorf_d.out > ERROR.txt
type ERROR.txt
PAUSE
