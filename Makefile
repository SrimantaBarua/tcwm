# (C) 2019 Srimanta Barua

CC:=gcc

TARGET:=tcwm

SRC:=$(wildcard *.c)
OBJ:=$(SRC:.c=.o)
DEP:=$(OBJ:.o=.d)

CFLAGS?=
CPPFLAGS?=-D__SCWM_DEBUG__
LDFLAGS?=
LIBS?=

CFLAGS+=-O2 -MMD `pkg-config --cflags xcb`
CPPFLAGS+=
LDFLAGS+=
LIBS+=`pkg-config --libs xcb`

.PHONY: build clean

build: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

-include $(DEP)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f $(TARGET) $(OBJ) $(DEP)
