@echo off

echo "+++++++++++++++++++++ Compilando Codigo HOST +++++++++++++++++++++++"

bcc -DDOS -w -ml pcicmeuc.c pcic44.lib > errorh.txt

type errorh.txt

