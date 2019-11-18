@ECHO OFF
echo "compilando codigo DSP"
cl30 -ic:\dsp -v40 video.c capt_d.c -z link.cmd -o capt_d.out > ERROR.txt
type ERROR.txt
PAUSE