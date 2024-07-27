CC = g++
CFlags = -std=c++11 -g -c -I $(includePath)

includePath = include

s = src
t = target

VMKernel =  $(t)/util.o \
			$(t)/ClassFile.o \
			$(t)/DataArea.o \
			$(t)/ExecEngine.o \
			$(t)/main.o 

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

$(t)/DataArea.o: $(s)/DataArea.cpp
	mkdir -p target
	$(CC) $(CFlags) -o $@ $<

$(t)/ExecEngine.o: $(s)/ExecEngine.cpp
	mkdir -p target
	$(CC) $(CFlags) -o $@ $<