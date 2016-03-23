# Author:  David Rigert
# Created: 3/6/2016
# CS344 Project 4 makefile

C = gcc
CFLAGS = -Wall
PROGS = otp_enc_d otp_enc otp_dec_d otp_dec keygen
OBJS = socketio.o
SOBJS = crypto.o server.o
CLOBJS = client.o

all: $(PROGS)

otp_dec_d otp_enc_d: $(SOBJS) $(OBJS) $(@:=.c)
	$(C) $(CFLAGS) $(@:=.c) $(SOBJS) $(OBJS) -o $(@)

otp_enc otp_dec: $(CLOBJS) $(OBJS) $(@:=.c)
	$(C) $(CFLAGS) $(@:=.c) $(CLOBJS) $(OBJS) -o $(@)

keygen: $(@:=.c)
	$(C) $(CFLAGS) $(@:=.c) -o $(@)

$(OBJS) $(CLOBJS) $(SOBJS): $(@:.o=.c)
	$(C) $(CFLAGS) -c $(@:.o=.c)

clean:
	$(RM) $(PROGS) $(SOBJS) $(CLOBJS) $(OBJS)
