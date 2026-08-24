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

## Syntax

As a one-instruction language, the syntax is minimal. Nonetheless, here are a few important details.

### Comments
Comment lines begin with `#` character:
```text
# this is a comment
```

### SBN
The shape of an SBN operation line will be like:
```text
sbn, <source>, <target>, <destination>, <LABEL>
```
- `<source>` and `<target>` can be either literal values (e.g., `12`, `-5`) or registers (e.g., `r6`).
- `<destination>` must be a register.
- The operation computes `source - target` and stores the result in `<destination>`.
- If the resulting value (now stored in `<destination>`) is negative, the program counter jumps to the line immediately following the specified `<LABEL>` (which must be defined elsewhere in the file).

#### example:

```test
#the branch won't happen and r3 becomes 6
SBN, 12, 5, r3, myLabel
```

### Label Definition
A label should be defined in a particular line in the following format:
```test
L <label_name>
```

#### example:
```text
L foo
# pc will jump to this line whenever the branch to foo occures
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
The keyword `out` provides a simple output mechanism:
```test
out <text_or_register>
```

#### example:
```text
out hello_world
# prints hello_world

out r1
# prints the current value of register 1

out \right_starts_with_r_but_is_not_a_register!
# prints the literal text (since it's starting with \)
```

## Future Plans
- [ ] Add ability to print texts with spaces in the middle
- [X] Add an input mechanism
- [ ] Add a load/store command
- [ ] Seperate functions into multi files
- [ ] Add some flags (e.g, --help)

## Contribution
I welcome your feedback and suggestions — feel free to open an issue or reach out via [email](h.momeni.sh@gmail.com). Contributions are also very much appreciated.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
