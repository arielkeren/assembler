# Assembler

An assembler, written in C, for a made-up assembly language.

## Background
This assembler was made as the final project for a course in C in The Open University of Israel.

## How It Works
### List of Instructions
These are all the instructions in the language:<br>
* `mov` - Copies the source operand into the destination operand.
* `cmp` - Compares the 2 operands and updates the Z flag accordingly.
* `add` - The destination operand gets updated to the sum of the 2 operands.
* `sub` - The destination operand gets updated to the difference of the source operand from the destination operand.
* `lea` - Loads the address of the source operand into the destination operand.
* `clr` - Sets the operand to zero.
* `not` - Flips the bits of the operand.
* `inc` - Increments the operand by 1.
* `dec` - Decrements the operand by 1.
* `jmp` - Jumps to the address of the operand.
* `bne` - Jumps to the address of the operand if the Z flag is zero.
* `red` - Reads a character from the standard input `stdin` into the operand.
* `prn` - Prints the character in the operand to the standard output `stdout`.
* `jsr` - Calls a function.
* `rts` - Returns from a function.
* `stop` - Ends the program.

### Operands
Each operand can be:
1. **Immediate** - a hash symbol, followed by a number. Examples: #1, #-100.
2. **Direct** - a label name. Examples: END, loop1.
3. **Direct Register** - r0, r1, r2, r3, r4, r5, r6 or r7.
4. **Indirect Register** - *r0, *r1, *r2, *r3, *r4, *r5, *r6, *r7.

### Comments
Comments can be added using a semicolon as the first character of the line.<br>
**Examples:**
```
; This is a comment line.
```
```
mov r1, r2 ; This is invalid.
```

### Macros
It is also possible to declare macros:
```
macr <macroname>
  ...
  ...
  ...
endmacr
```
Macros get expanded in the pre-assembler stage into the `.am` output file.

### Labels
Before each line, a label can be added to be used in some part of the program (including before the definition).<br>
Labels must start with a lowercase or uppercase letter or with an underscore. For the rest, numbers can also be added to the name.<br>
**Examples:**
```
END: stop
```
```
label1: add #+123, r2
```

### Label Declarations
Labels can be declared as entry or as extern to interact with different source files in the program.<br>
These labels get appended, along with their addresses, to the `.ent` and `.ext` output files.<br>
**Examples:**
```
.entry END
```
```
.extern _label99
```

### Word Encoding
All the words in the program get appended in octal representation to the `.ob` output file.<br>
In the assembler, they are represented in binary, using 15 bits.<br>
Words that begin an instruction follow this structure:<br>
* **Bits 1-3** - A, R and E flags.
* **Bits 4-7** - the addressing mode of the destination operand.
* **Bits 8-11** - the addressing mode of the source operand.
* **Bits 12-15** - the code of the instruction.<br>
Each of the following optional words in an instruction follow different structures, based on the operands' addressing modes.<br>
1. **Immediate** - bit 3 is set to 1, and bits 4-15 encode the integer in a 2's complement binary representation.
2. **Direct** - bit 1 is set to 1 if extern, otherwise bit 2 is set to 1, and bits 4-15 encode the address of the label.
3. **Direct Register** - bit 3 is set to 1, and bits 4-6 encode the register's number if it is the source operand, otherwise bits 7-9.
4. **Indirect Register** - follows the direct register's structure.<br>
**Note:** If an instruction's operands are all registers, they will all be encoded into a single additional word, and not to one each as usual.


## Output Files
The assembler gets the names of all the source files with a `.as` extension (only the names, without the extension).<br>
All the files that the assembler creates for a given file have the same name, just different extensions.<br>
The files get compiled into at most **4** different files each:
1. `file.am`<br>
  Contains the contents of the source file after expanding all the macros.
2. `file.ob`<br>
  Contains the number of words in each of the 2 parts of the program when loaded into memory - code and data.<br>
  Then, for each word, specifies the address of the word (starting from decimal 100), followed by an octal representation of the content of the word.<br>
  The code part comes before the data part.
3. `file.ent`<br>
  Contains the addresses of the definitions of every label in the source file, that was specified in a `.entry` declaration.
  Alongside the addresses, the name of each label is also specified.
4. `file.ext`<br>
  Contains the addresses of the words where a label in the source file, that was specified in a `.extern` declaration, was used.
  Alongside the addresses, the name of each label is also specified.

## Instructions
* Clone this repository:
   ```bash
   git clone https://github.com/arielkeren/assembler.git
* Run the Makefile:
   ```bash
   makefile
* Run the assembler:
   ```bash
   ./assembler file1 file2 [...]
