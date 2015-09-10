CC = x86_64-w64-mingw32-gcc
EXE =						\
	windows-event-log-simple-logger.exe	\
	windows-event-log-logger.exe

all: $(EXE)

clean:
	rm -f $(EXE)

.SUFFIXES: .c .exe

.c.exe:
	$(CC) -Wall -o $@ $<
