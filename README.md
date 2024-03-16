# LEG Eimulator
A LEG CPU simulator which coding with C. Currently it is a lib for user to link.

# Compile
Assuming you didn't need debug and test coverage with you program, you need find Cmakelists which have two varibles `DEBUGGER`, `COVERAGE`

Change then to false, then typing follow commands in you terminal:
```
mkdir build
cd build
cmake ..
make
```

Also you can test current lib:
```
make test
```

After `make` command execute, the lib will compile in `build/src` file which name is `libLEGCPU.a`

# Usage
Assuming you know how to link lib and include header, there is tutorial code:
```C
#include <stdio.h>
// main lib
#include "LEGcpu.h"
// assembly code, also you can make youself's assembly code
#include "LEGcpu_assembly_code.h"

int main()
{
    add_code(ADD + IMM12, 10, 2, REG0);
    
    execute_ticks(1);
    
    // check register 0
    printbit8(reg[0], BINARY);
    
    // you can export you code
    FILE* file = open("test.program","w+")
    export(file);
}
```
