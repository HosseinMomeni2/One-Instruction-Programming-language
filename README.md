# One Instruction Programming Language

This is a minimal simulation of an OISC (One Instruction Set Computer) which implements only the SBN (Subtract and Branch if Negative) instruction.  
It has been proven that a machine capable of executing SBN is [turing-complete](https://en.wikipedia.org/wiki/Turing_machine), and therefore can solve any computable problem. (See: [OISC](https://en.wikipedia.org/wiki/One-instruction_set_computer))

## Features

- No dependency but the g++ compiler
- Support of labels instead of line numbers for branch operation

## Installation

You can clone the repository and make the project using the Makefile:
```bash
git clone https://github.com/HosseinMomeni2/One-Instruction-Programming-language.git
cd One-Instruction-Programming-language
make
```

Then, run the examples:
```bash
./OISC.out example/E1
```

or make your own sbn code and run it:
```bash
./OISC.out <file_name>
```
note that you can use following flags after the file name:
```text
-d    debug
```

#### note:
The commands above was tested on ubuntu24 and will need `git` and `make` to be installed. But the project can be built and execute on any other operating system with other tools. please contact me if you have issues building of running this project.

## Quick start
To confirm a prope build, run the E2.sbn file:
```bash
./OISC.out example/E2.sbn
```
which should output like this:
```text
r2:
6
r3:
5
hello world

program finished with exit code 0
```

## Architecture

### Registers
The simulator provides 16 general-purpose registers, named r0 through r15. All registers can store integer values (both positive and negative).

### Program Counter (PC)
The Program Counter is an internal pointer that tracks the current execution line. It is not directly accessible as a register, but you can manipulate it through branching operations.
By default, the PC increments by 1 after each instruction unless a branch occurs.  
When a branch is taken, the PC is set to the specified target.

### Execution Flow
- The simulator loads the entire program file into memory. 
- Execution starts at line 0 (the first instruction).
- Each sbn instruction is executed in order.
- The program halts when it reaches the end of the file or encounters an invalid instruction.

⚠️ **Important**: Since syntax error handling is not yet fully implemented, the simulator may behave unexpectedly or crash if your code contains syntax errors. Please read the syntax guid below very carefully and always double-check your instruction formatting.

## Syntax

### Comments
Comment lines begin with `#` character:
```text
# this is a comment
```
note: inline comments aren't implemented yet and may (or may not) cause unexpected behaviour.

### SBN
The shape of an SBN operation line will be like:
```text
sbn, <source>, <target>, <destination>, <branch>
```
- `<source>` and `<target>` can be either literal values (e.g., `12`, `-5`) or registers (e.g., `r6`).
- `<destination>` must be a register.
- The operation computes `source - target` and stores the result in `<destination>`.
- If the resulting value (now stored in `<destination>`) is negative, the program counter jumps to the line immediately following the specified `<LABEL>` (which must be defined elsewhere in the file).
- `<branch>` could be a label's name, a literal line number, a line number stored in a register,  or a value to add (like +14 or -7)

#### example:

```test
SBN, 12, 5, r3, myLabel
# the branch won't happen and r3 becomes 6

SBN, r1, 5, r3, 17
# r3 will become r1-5 and pc will jump to 17 if result is negative

# some more examples:
SBN, 12, r2, r3, myLabel12
SBN, r1, r2, r3, +1
SBN, r1, r2, r3, -10
SBN, r1, r2, r3, r7
```

### Label Definition
A label should be defined in a particular line in the following format:
```test
L <label_name>
```

- A label's name should only contain english letters (in both uppercase and lowercase) and digits
- A Label's name can not start with a digit

#### example:
```text
L foo
# pc will jump to this line whenever the branch to foo occures

L next
```

### Links
It is possible to store a line number in a register during runtime using `link` keyword. It is meant to provide the "jump and link" ability and can be very useful to call functions.

#### example:
In this example a function foo is called and returned. (look at Example3 for a better understanding)
```text
L foo
# this is the function foo
# ...

# this line will return the pc to where the function was called
#we assume the r15 is used to link
sbn, 0, 1, r12, r15

L main
# we want to call foo from here

# first link to r15
link r15
# add 5 for a proper return
sbn, r15, -5, r15, +1
# jump
sbn, 0, 1, r12, foo

# pc returns here after foo
out finish

```

### Inputs
The `inp` keyword will take numerical inputs:
```text
inp <register>
```

#### example:
```text
inp r7
# waits for input and stores it into r7
```

### Outputs
The keyword `out` provides a simple output mechanism. It should be continued by a register (like r12) or a string between two qoutation marks:
```test
out <string_or_register>
```

#### example:
```text
out "hello world"
# prints hello world

out "hello\nworld"
# prints hello world in two lines

out r1
# prints the current value of register 1
```  

## Future Plans
- [ ] Syntax error management
- [X] Add ability to print texts with spaces in the middle
- [X] Add an input mechanism
- [ ] Add a load/store command
- [ ] Seperate functions into multi files
- [ ] Add some flags (e.g, --help)

## Contribution
I welcome your feedback and suggestions — feel free to open an issue or reach out via [email](h.momeni.sh@gmail.com). Contributions are also very much appreciated.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
