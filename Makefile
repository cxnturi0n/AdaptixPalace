CC_64=x86_64-w64-mingw32-gcc
# CC_86=i686-w64-mingw32-gcc
CFLAGS=-O1 -fno-jump-tables -shared -Wall -Wno-pointer-arith

.PHONY: all x64_simple_rdll_mask x64_simple_rdll x64_simple_rdll_free x64_runner clean

all: x64_simple_rdll_mask x64_simple_rdll x64_simple_rdll_free x64_runner

runner/bin:
	mkdir -p runner/bin

loaders/simple_rdll/bin:
	mkdir -p loaders/simple_rdll/bin

loaders/simple_rdll_mask/bin:
	mkdir -p loaders/simple_rdll_mask/bin

loaders/simple_rdll_free/bin:
	mkdir -p loaders/simple_rdll_free/bin

x64_simple_rdll_mask: loaders/simple_rdll_mask/bin
	$(CC_64) -DWIN_X64 $(CFLAGS) -c loaders/simple_rdll_mask/src/loader.c -o loaders/simple_rdll_mask/bin/loader.x64.o
	$(CC_64) -DWIN_X64 $(CFLAGS) -c loaders/simple_rdll_mask/src/mask.c -o loaders/simple_rdll_mask/bin/mask.x64.o
	$(CC_64) -DWIN_X64 $(CFLAGS) -c loaders/simple_rdll_mask/src/util.c -o loaders/simple_rdll_mask/bin/util.x64.o

x64_simple_rdll: loaders/simple_rdll/bin
	$(CC_64) -DWIN_X64 $(CFLAGS) -c loaders/simple_rdll/src/loader.c -o loaders/simple_rdll/bin/loader.x64.o
	$(CC_64) -DWIN_X64 $(CFLAGS) -c loaders/simple_rdll/src/util.c -o loaders/simple_rdll/bin/util.x64.o

x64_simple_rdll_free: loaders/simple_rdll_free/bin
	$(CC_64) -DWIN_X64 $(CFLAGS) -c loaders/simple_rdll_free/src/loader.c -o loaders/simple_rdll_free/bin/loader.x64.o
	$(CC_64) -DWIN_X64 $(CFLAGS) -c loaders/simple_rdll_free/src/util.c -o loaders/simple_rdll_free/bin/util.x64.o
	$(CC_64) -DWIN_X64 $(CFLAGS) -c loaders/simple_rdll_free/src/free.c -o loaders/simple_rdll_free/bin/free.x64.o

x64_runner: runner/bin
	$(CC_64) -DWIN_X64 -masm=intel -Wall -Wno-pointer-arith runner/src/run.c -o runner/bin/run.x64.exe -lws2_32

# x86_runner: runner/bin
# 	$(CC_86) -DWIN_X86 -masm=intel -Wall -Wno-pointer-arith runner/src/run.c -o runner/bin/run.x86.exe -lws2_32

clean:
	rm -rf bin loaders/simple_rdll_mask/bin loaders/simple_rdll/bin loaders/simple_rdll_free/bin runner/bin
