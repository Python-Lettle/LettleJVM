CC = g++
CFlags = -g -c -I $(includePath)

includePath = include

s = src
t = target

VMKernel =  $(t)/main.o \
			$(t)/util.o \
			$(t)/ClassFile.o

all: $(VMKernel)
	$(CC) $(CCflags) $(VMKernel) -o lvm

run: all
	./lvm

$(t)/main.o: $(s)/main.cpp
	$(CC) $(CFlags) -o $@ $<

$(t)/util.o: $(s)/util.cpp
	$(CC) $(CFlags) -o $@ $<

$(t)/ClassFile.o: $(s)/ClassFile.cpp
	$(CC) $(CFlags) -o $@ $<
