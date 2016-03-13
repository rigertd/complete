# Author:  David Rigert
# Created: 3/6/2016
# CS344 Project 4 makefile

C = gcc
CFLAGS = -Wall
PROGS = otp_enc_d otp_enc keygen
OBJS = socketio.o
SOBJS = crypto.o server.o
COBJS = client.o

all: $(PROGS)

otp_enc_d: $(SOBJS) $(OBJS) otp_enc_d.c
	$(C) $(CFLAGS) otp_enc_d.c $(SOBJS) $(OBJS) -o $(@)

opt_enc: $(COBJS) $(OBJS) otp_enc.c
	$(C) $(CFLAGS) otp_enc.c $(COBJS) $(OBJS) -o $(@)

keygen: keygen.c
	$(C) $(CFLAGS) keygen.c -o $(@)

$(OBJS) $(COBJS) $(SOBJS): $(@:.o=.c)
	$(C) $(CFLAGS) -c $(@:.o=.c)

clean:
	$(RM) $(PROGS) $(SOBJS) $(COBJS) $(OBJS)
