# Author:  David Rigert
# Created: 3/6/2016
# CS344 Project 4 makefile

C = gcc
CFLAGS = -Wall
PROGS = otp_enc_d otp_enc keygen
OBJS = crypto.o

all: $(PROGS)

otp_enc_d: $(OBJS) otp_enc_d.c
	$(C) $(CFLAGS) otp_enc_d.c $(OBJS) -o $(@)

opt_enc: otp_enc.c
	$(C) $(CFLAGS) otp_enc.c -o $(@)

keygen: keygen.c
	$(C) $(CFLAGS) keygen.c -o $(@)

$(OBJS): $(@:.o=.c)
	$(C) $(CFLAGS) -c $(@:.o=.c)

clean:
	$(RM) $(PROGS) $(OBJS)
