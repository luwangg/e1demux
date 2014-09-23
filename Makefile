TARGET           = e1demux
VERSION          = "0.0.1"

SRCS = e1demux.c e1.c e1ts.c frmsyn.c e1rcv.c params.c
OBJS = $(SRCS:.c=.o)
CFLAGS += -DVERSION=\"$(VERSION)\"
CFLAGS += -Wall -Werror --std=gnu99
CFLAGS += -DDEBUG

$(TARGET): $(OBJS) 

all: $(TARGET) 

clean:
	rm -f $(OBJS)

distclean: clean
	rm -f $(TARGET)
