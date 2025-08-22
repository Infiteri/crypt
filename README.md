Crypt - a small cpp JIT script runner

# Requirements:
  1. g++
  2. make
  3. C++17 (or above)

# Build
After cloning this repo. Head in the root folder and call "make" from the shell/terminal
This will create a 'bin' folder with a 'crypt' executable

# How to use
From the root directory you can run

```
bin/crypt test.cpp
```

This will compile and run 'test.cpp' 
Note that unchanged files will not be recompiled, in this way the same script doesnt need to be recompiled in order to be ran

# TODO 

1. Output paths
2. Put all executables under one folder in /CRYPT/crypts
3. Add library and compile flags support
4. Multiple files
5. Custom crypt library for easier use
6. Interop with C
