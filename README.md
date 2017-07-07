EasyCC C++
============

EasyCC C++ (Easy Compiler Compiler) is a library allowing users to easily develop their own programming language. The project does not require wrting any line of code for the lexical and syntax analysis phases. The configurations of the latters are provided as JSON files to the library. Adding the logic for a programming language is done by simply registering semantic action handlers in order to gradually build the structure of the input and eventually generating output code.

To learn more about how it works, read the documentation and/or check the step by step tutorial below.

### Difference between EasyCCDev and EasyCCPro
Both libraries use the same Lexical and Syntax algorithms, and switching from one library to another requires minor changes. 
* EasyCCDev (EasyCC development mode) takes the JSON files as arguments in the final executable. This is useful during the development phase because the program does not need to be recompiled to apply changes in the JSON files.
* EasyCCPro (EasyCC production mode) takes the JSON files as arguments to the CMake. The compile process for your program will be composed of two steps. First is embedding the JSON files into the executable. Second is compiling your program with the generated files.

### Links
* Project UML: <a href="https://drive.google.com/file/d/0B8fWEFscW3Z4SzlpMnpETkdPOGs/view">UML Draw.io</a>
* Lexical Analysis: <a href="src/lexical">Documentation</a>
* Syntax Analysis: <a href="src/syntax">Documentation</a>

### Learn by examples
#### Calculator programming language
This example is a step by step tutorial on how to use EasyCC-C++ to build a very simple programming language. The program will read an input file and produce an output file. The input file is composed of integer numbers (operands) and arithmetic symbols (operatos). The output produced is the mathematical result of the calculation written in the input file.

#### Step 1: Create a "lexical_state_machine.json" file
File details: <a href="src/lexical#state-machine">Doc</a>

Here is part of what the file content looks like. 
```
{
    "states": [
        {
            "type": "INITIAL",
            "id": 0
        },
        ....
    ],
    "transitions": [
        {
            "from": 0,
            "to": 0,
            "chars": ["SPACE","RETURN","TAB","NEW_LINE","EOF"]
        },
        ...
    ]
}
```
Because it is hard to keep track of the state machine component
inside a json file, use the graph tool: <a href="tools/gui">GUI</a> which reduces the complexity of the task to simple
web forms. Once done populating the graph using the GUI, simply generate the json file and save it under the specified name above.
