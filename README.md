## About Qod

Qod is an experimental system-level compiled language, that I slowly develop in my spare time.

The source code origins root deep into history. I took as a basis the source code of [the educational language Context by Andrei Hohlov](https://avhohlov.narod.ru/). Context was initially developed for MS-DOS in the mid 90's. Later it was ported to Win32 and Linux.

The source code wasn't well-designed nor perfectly suited for my purposes, but hacking on it was fun. So I spent a fair amount of time refactoring the codebase, improving the generated code, and redesigning the language. The language itself is still far from what I planned, and the languages design is in development. So no language specification is available right now.

The compiler is intended to remain hobby-scale and pleasant to hack on. Although the project is being developed with the expectation of becoming a practical tool some day, the primary goal of the project is fun.

The size of the code base is now about 10k lines, excluding tests and samples. It may grow several times more as support for different architectures is added.

The language is self-hosted. You can build the compiler from the sources with compiler itself, with the prebuilt binaries provided. (32-bit binaries for Linux and Windows.) The only extra dependency is FASM, and its prebuilt copy is also provided in the repository.

## The language and the frontend

TODO. Drafts to be published... Probably soon, or not.

## Backend

In the current implementation, the compiler doesn't make use of any intermediate language (IL) for the code representation, nor provides any optimizations based on full-scale control graph analysis.

Instead the code generation is directed by the abstract syntax tree.

That kind of code generation is typically done in rather straightforward and simple manner. However, as the code base evolved, and I tried to optimize the generated code for "this obvious case and that obvious case", it may have gotten a little out of control.

I'm not sure what this kind of code generation is properly called, so let's name it just AST/PHO codegen for short. *(Abstract Syntax Tree-directed translation with Peephole Optimization)*

The compiler is capable of producing good enough code at the level of individual expressions or short sequences of expressions.

The compiler is also able to analyze functions and take into account the resulting characteristics. For example, optimize the case where a function never returns control.

However, the intermediate layer between these two is effectively absent. The compiler does not break the algorithm into a graph of basic blocks, does not discover the actual lifetime of variables, does not perform global register allocation and so on. This limitation is due to the lack of a suitable intermediate representation.

Supported and planned targets:

* ISAs:
  * [x] i386
  * [ ] TBD: AMD64 aka x86_64
  * [ ] TBD: 16-bit x86
  * [ ] TBD: ARM32, ARM64 aka AArch64
  * [ ] TBD: RISC-V32, RISC-V64
  * [ ] TBD: m68k (a 32-bit ISA)
  * [ ] TBD: PDP-11 (a 16-bit ISA)
  * [ ] TBD: Z80/i8080 (a 8-bit ISA)
* Non-hardware ISAs:
  * [ ] TBD: [QBE](https://c9x.me/compile/)
* OS specific:
  * [x] Linux
  * [x] Windows
  * [ ] TBD: 16-bit DOS applications
  * [ ] TBD: 32-bit DOS applications under a DOS extender
  * [ ] TBD: FreeBSD
  * [ ] TBD: NetBSD

**Details:**

**Why m68k, PDP-11.** The current design of the code generator is well suited for CISC architectures. It works well for i386, and should work even better for those ISAs. I should check it out.

**Why DOS.** It's a way back to the root and a tribute to Andrei. Context was initially developed at and targeted DOS. The Qod itself isn't capable of running as a 16-bit DOS application due to memory limitations. But it is theoretically possible to run it under a 32-bit DOS extender. So if I restore the support of 16-bit mode in the backend, Qod can be used to build 16-bit and 32-bit DOS programs natively.

**Why Z80/i8080.** Looks like a challenging task. I should try. It would be nice if the compiler could be used to make games for MSX, ZX Spectrum etc.

**Why ARM, RISC-V.** These are modern platforms. The compiler have to support them in order to be more than a toy. Although the architecture of the code generator doesn't seem very suitable for these ISAs.

**Why QBE.** QBE is an independent backend that provides a proper level of abstractions for performing optimizations. It is also small and simple that fits well with the Qod design and goals. It supports AMD64, ARM64, RISC-V64 out of box. With a well-defined intermediate language, it is theoretically possible to transparently replace vanilla QBE with another implementation that provides a better level of optimizations.

## Licensing

* [src/](src/), [precompiled/](precompiled/): The qodc source codes and binaries are provided under the term specified in [LICENSE.TXT](LICENSE.TXT)
* [src/tests/](src/tests/): The qodc test files are provided under the term of [0BSD license](src/tests/LICENSE.TXT). (See also: https://opensource.org/license/0bsd)
* Any files in the [extra/](extra/) directory are provided under the terms of 0BSD license, unless otherwise stated.
* Third party software included in this repository:
  * [3rd-party/fasm/](3rd-party/fasm/): FASM is a software by Tomasz Grysztar, provided under the term specified in [3rd-party/fasm/license.txt](3rd-party/fasm/license.txt). The project's web site: https://flatassembler.net/
  * [3rd-party/context/](3rd-party/context/): Context Compiler is a software by Andrei Hohlov, provided under the term specified in [3rd-party/context/README](3rd-party/context/README). The project's web site: https://avhohlov.narod.ru/

