@echo "+++++++++++++++++++++ Compilando HOST +++++++++++++++++++++++"

bcc -Ic:\pcic44\include -DDOS -Lc:\pcic44\libs\bor -w -ml pcimaoh.c pcic44.lib >errorh.txt
type errorh.txt

rem pause

@echo "++++++++++++++++++++++ compilando codigo DSP +++++++++++++++++++++++"

cl30 -ic:\dsp -v44 mao1d.c -z dsp.cmd -o mao1d.out > error1.txt
type error1.txt

rem pause

@echo "+++++++++++++++++++++ compilando codigo DSP ++++++++++++++++++++++++"

cl30 -ic:\dsp -v44 mao2d.c -z dsp.cmd -o mao2d.out > error2.txt
type error2.txt


