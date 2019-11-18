@ECHO OFF
echo "compilando codigo DSP"
cl30 -ic:\dsp -v40 fgcmeuc.c -z link.cmd -o fgcmeuc.out
cl30 -ic:\dsp -v44 euc2.c -z link.cmd -o euc2.out
cl30 -ic:\dsp -v44 euc1.c -z link.cmd -o euc1.out

PAUSE
