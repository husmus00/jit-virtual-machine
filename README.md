# Stack-Based VM w/ JIT

(Currently W.I.P)

A stack-based bytecode VM with multi-arch JIT compilation 

Planned features:
- Tagged Objects
- Multi arch JIT targeting x86_64 & RISC-V

Instruction set:

- ADD
- SUB
- PSH
- POP
- STR ; Store
- LOD ; Load
- DUP 
- PRT ; Print value
- ASC ; Print in ASCII mode
- INC
- ; for comments
- @ for addresses