# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004.
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.


EE_OBJS_DIR = obj/
EE_LIB_DIR = lib/
EE_SRC_DIR = src/
EE_INC_DIR = include/

EE_OBJS = libcdvd.o ncmd.o scmd.o
EE_LIB = libcdvd.a

EE_LIB := $(EE_LIB:%=$(EE_LIB_DIR)%)
EE_OBJS := $(EE_OBJS:%=$(EE_OBJS_DIR)%)

all: $(EE_OBJS_DIR) $(EE_LIB_DIR) $(EE_LIB)

clean:
	rm -f -r $(EE_OBJS_DIR) $(EE_LIB_DIR)

include $(PS2SDKSRC)/Defs.make
include $(PS2SDKSRC)/ee/Rules.make
include $(PS2SDKSRC)/ee/Rules.release

