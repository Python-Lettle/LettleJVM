CC = g++
CFlags = -std=c++11 -g -c -I $(includePath)

includePath = include

s = src
t = target

VMKernel =  $(t)/main.o \
			$(t)/util.o \
			$(t)/ClassFile.o \
			$(t)/LVM.o

all: $(VMKernel)
	$(CC) $(CCflags) $(VMKernel) -o lvm

run: all
	./lvm

clean:
	rm -rf target
	rm lvm

$(t)/main.o: $(s)/main.cpp
	mkdir -p target
	$(CC) $(CFlags) -o $@ $<

$(t)/util.o: $(s)/util.cpp
	mkdir -p target
	$(CC) $(CFlags) -o $@ $<

$(t)/ClassFile.o: $(s)/ClassFile.cpp
	mkdir -p target
	$(CC) $(CFlags) -o $@ $<

$(t)/LVM.o: $(s)/LVM.cpp
	mkdir -p target
	$(CC) $(CFlags) -o $@ $<