# CC=i686-w64-mingw32-gcc
CC_64=x86_64-w64-mingw32-gcc
CFLAGS=-O1 -fno-jump-tables -shared -Wall -Wno-pointer-arith

.PHONY: all x64_simple_rdll_mask x64_simple_rdll x64_runner clean

all: x64_simple_rdll_mask x64_simple_rdll x64_runner

runner/bin:
	mkdir -p runner/bin

loaders/simple_rdll/bin:
	mkdir -p loaders/simple_rdll/bin
	
loaders/simple_rdll_mask/bin:
	mkdir -p loaders/simple_rdll_mask/bin

# x86: bin
# 	$(CC) -DWIN_X86 $(CFLAGS) -c loader/src/loader.c -o loader/bin/loader.x86.o
# 	$(CC) -DWIN_X86 $(CFLAGS) -c loader/loader/src/mask.c -o loader/bin/mask.x86.o

x64_simple_rdll_mask: loaders/simple_rdll_mask/bin
	$(CC_64) -DWIN_X64 $(CFLAGS) -c loaders/simple_rdll_mask/src/loader.c -o loaders/simple_rdll_mask/bin/loader.x64.o
	$(CC_64) -DWIN_X64 $(CFLAGS) -c loaders/simple_rdll_mask/src/mask.c -o loaders/simple_rdll_mask/bin/mask.x64.o
	$(CC_64) -DWIN_X64 $(CFLAGS) -c loaders/simple_rdll_mask/src/util.c -o loaders/simple_rdll_mask/bin/util.x64.o

x64_simple_rdll: loaders/simple_rdll/bin
	$(CC_64) -DWIN_X64 $(CFLAGS) -c loaders/simple_rdll/src/loader.c -o loaders/simple_rdll/bin/loader.x64.o
	$(CC_64) -DWIN_X64 $(CFLAGS) -c loaders/simple_rdll/src/util.c -o loaders/simple_rdll/bin/util.x64.o

x64_runner: runner/bin
	$(CC_64) -DWIN_X64 -masm=intel -Wall -Wno-pointer-arith runner/src/run.c -o runner/bin/run.x64.exe -lws2_32
	
clean:
	rm -rf bin loaders/simple_rdll_mask/bin loaders/simple_rdll/bin runner/bin
