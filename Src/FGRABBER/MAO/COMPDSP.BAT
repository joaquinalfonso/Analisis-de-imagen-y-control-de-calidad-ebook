@ECHO OFF
echo "compilando codigo DSP"
cl30 -ic:\dsp -v40 cmmao.c -z link.cmd -o cmmao.out
PAUSE
