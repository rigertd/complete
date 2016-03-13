# Author:  David Rigert
# Created: 3/6/2016
# CS344 Project 4 makefile

C = gcc
CFLAGS = -Wall
PROGS = otp_enc_d otp_enc keygen
SOBJS = crypto.o socketio.o server.o
COBJS = client.o socketio.o

all: $(PROGS)

otp_enc_d: $(SOBJS) otp_enc_d.c
	$(C) $(CFLAGS) otp_enc_d.c $(SOBJS) -o $(@)

opt_enc: $(COBJS) otp_enc.c
	$(C) $(CFLAGS) otp_enc.c $(COBJS) -o $(@)

keygen: keygen.c
	$(C) $(CFLAGS) keygen.c -o $(@)

$(OBJS): $(@:.o=.c)
	$(C) $(CFLAGS) -c $(@:.o=.c)

clean:
	$(RM) $(PROGS) $(SOBJS) $(COBJS)
