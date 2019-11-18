@ECHO OFF
echo "compilando codigo DSP"
cl30 -ic:\dsp -v40 fgcmmao.c -z link.cmd -o fgcmmao.out
cl30 -ic:\dsp -v44 mao2.c -z link.cmd -o mao2.out
cl30 -ic:\dsp -v44 mao1.c -z link.cmd -o mao1.out
rem ..\cl\cl30 -i..\cl -v40 video.c fgcmmao.c -z link.cmd -o fgcmmao.out > ERROR.txt
rem ..\cl\cl30 -i..\cl -v44 video.c mao2.c -z link.cmd -o mao2.out > ERROR2.txt
rem ..\cl\cl30 -i..\cl -v44 video.c mao1.c -z link.cmd -o mao1.out > ERROR1.txt

PAUSE
