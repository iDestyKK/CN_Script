# CN\_Script
*A language designed to make C easier to program in.*

## About
C is my favourite programming language.
But, it isn't as trivial to do some things in it compared to other languages.
Therefore, I wrote CN\_Script as an experiment on how I can improve on C myself.

## Features
**Implemented**
* Functions w/ Arguments
* Include other CNS files via `#import "file.cns"`
  * Nested includes are possible.
  * Also supports `#import <io>`, which allows making C libraries to import.
* Inline C
* Generates valid GNU89 Standard C code which compiles with `gcc`.

**Planned**
* Data Structure support via `#import <structures>`
* Dynamic typing support

## Why CN\_Script?
It's more of a concept language at the moment, but when it is done, it will
allow programmers to write programs that will transpile into C. What this means
for you is that your code will be able to execute with the speed of native C
code (depending on what compiler you use to compile the generated C code).

## What file extensions are available?
`.cns` is the default file extension for CN\_Script.

## How do I transpile CN\_Script into C?
**Synopsis**
```bash
cns file
```

**Example:** Transpile `main.cns` into `main.c`:

```bash
cns main.cns > main.c
```
