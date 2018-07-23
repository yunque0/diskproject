SOURCES:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SOURCES))
ELF:=diskserver
CC:=gcc
CFLAGS:=-Wall   -I../include
$(ELF):$(OBJS)
		 gcc -g $^ -o $@ -lpthread -lmysqlclient
clean:
	  rm -rf $(OBJS) $(ELF)
