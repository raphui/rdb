TARGET := rdb
PROJECT_HOME := $(PWD)
INCLUDES_HEADERS := -I$(PROJECT_HOME)/include
INCLUDES_LIBS_HEADERS := -I/usr/local/include/ -I/usr/include/
GCC := gcc
DEBUGFLAGS_C-Compiler := -g -O0 -fno-omit-frame-pointer -pipe -Wall
#LIBS := -lvlc -lspotify -lasound -lpthread
LIBS := -lpthread

OBJS := main.o	\
		utils/common.o	\
		utils/thread.o	\
		utils/trace.o	\
		utils/zmemory.o	\
		network/server.o \
		db.o	\
		cli.o	


%.o: %.c
	@@echo "CC " $<
	@$(GCC) $(DEBUGFLAGS_C-Compiler) $(INCLUDES_HEADERS) $(INCLUDES_LIBS_HEADERS) -c -fmessage-length=0 -o $@ $<

all: $(OBJS)
	@@echo "LD " $<
	@$(GCC) $(OBJS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(OBJS) rm -f $(TARGET)


