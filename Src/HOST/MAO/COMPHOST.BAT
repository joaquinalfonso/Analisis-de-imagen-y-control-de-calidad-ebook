@ECHO OFF
bcc -DDOS -w -ml host.c  > ERROR.TXT
type ERROR.TXT
PAUSE
