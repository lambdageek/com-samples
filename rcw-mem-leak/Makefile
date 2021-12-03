
.PHONY: all clean

CC?=clang
CFLAGS?=-g -O

CFLAGS+=-fvisibility=hidden


DEST=out/native

LIBEXT=dylib


SOURCES= \
	mini-container.h \
	mini-container.c \
	mem-leak-lib.h \
	mem-leak-lib.c

C_SOURCES=$(filter-out %.h,$(SOURCES))

OUTLIB=$(DEST)/libmem-leak-lib.$(LIBEXT)

all: $(OUTLIB)

$(DEST):
	mkdir -p $(DEST)


$(OUTLIB): $(SOURCES) | $(DEST)
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o $@ $(C_SOURCES)

clean:
	-rm -rf $(DEST)
