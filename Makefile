
TARGET = iopccfg_json.elf

CFLAGS += -I$(SDKSTAGE)/usr/include/
CFLAGS += -I$(SDKSTAGE)/usr/include/iopccfg
CFLAGS += -I$(SDKSTAGE)/usr/include/iopccommon
CFLAGS += -I$(SDKSTAGE)/usr/include/json-c

LDFLAGS += -L$(SDKSTAGE)/usr/lib/ 
LDFLAGS += -lc -lpthread -lrt
LDFLAGS += -ljson-c
LDFLAGS += -liopccommon

SRC = 
SRC += main.c

include Makefile.include.app
