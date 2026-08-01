# One Instruction Programming Language

This a minimal simulation of an OISC (One Instruction Set Computer) which is only able to run the SBN (Subtract and Branch if Negative) command.  
It can be proved that a machine which can run an SBN is a [turing machine](https://en.wikipedia.org/wiki/Turing_machine), therefor it could solve any problem. (refer to [OISC](https://en.wikipedia.org/wiki/One-instruction_set_computer))

## Features

- No dependency but the g++ compiler
- Support of labels instead of line numbers for branch operation

## Installation

You can clone the repository and make the project:
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


## Syntax

It is a one instruction set language so there is not much about the syntax, but here is some details to know.

### Comments
You can write a comment using # character:
```text
# this is a comment
```

### SBN
The shape of an SBN operation line will be like:
```text
sbn, <source>, <target>, <destination>, <LABEL>
```
in which the source and destination could be literal values (like 12 or 5) or they could be registers (like r6) and the destination could only be a register. The result of `source - target` will be calculated and stored in `destination`. After that it will be checked if the result (which is now the value in destination) is negative or not. if it is, the program counter will jump to the next line of the `LABEL` (which should be present somewhere in the file).

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
# pc will jump to this line whenever foo is called
```

## Contributing
This project is not in an stable state yet, therefor it's not possible to accept contributors, but I will always be happy to take your advice.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
