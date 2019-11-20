@echo off

echo "+++++++++++++++++++++ Compilando Codigo HOST +++++++++++++++++++++++"

bcc -DDOS -w -ml pcicmmao.c pcic44.lib > errorh.txt

type errorh.txt

