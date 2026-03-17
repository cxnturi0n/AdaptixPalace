# AdaptixPalace

Adaptix C2 ships an agent DLL which does not provide any evasion mechanism: it gets loaded into memory with RWX permissions, no SleepMask, garbage call stacks, etc.. 
The goal of this repo is to show how to wrap AdaptixC2 agent in a Crystal Palace Reflective DLL Loader, along with examples implementing different maldev techniques.

# Build
```
make clean; make all
```

# Link agent DLL to specific loader
```
cp/link loaders/simple_rdll/loader.spec agent.x64.dll loaders/simple_rdll/bin/agent.bin
```

# Use default Crystal Palace runner to run the PIC
```
runner/bin/run.x64.exe loaders/simple_rdll/bin/agent.bin
```

# To Do
- Provide other samples: Api Hooking, Call Stack Spoofing, Execution Guardrails, SleepMask, Indirect Syscalls, ...
- AdaptixC2 Integration via Service Extenders

# Contribute
You are deeply welcome to contribute by adding your own samples :)

[!IMPORTANT]
Provided samples are intended strictly for educational purposes and authorized security engagements only.