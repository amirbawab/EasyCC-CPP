EasyCC C++
============

### Project UML
* <a href="https://drive.google.com/file/d/0B8fWEFscW3Z4SzlpMnpETkdPOGs/view">UML Draw.io</a>

### Lexical Analysis
* <a href="https://github.com/amirbawab/EasyCC-CPP/tree/master/src/lexical">Documentation</a>

### Syntax Analysis
* <a href="https://github.com/amirbawab/EasyCC-CPP/tree/master/src/syntax">Documentation</a>

### Semantic Analysis
```
typedef std::vector<std::shared_ptr<ecc::LexicalToken>> Tokens;
easyCC.registerSemanticAction("#MyAction#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
    std::cout << "Action `MyAction` handled!" << std::endl;
});
```
### Difference between EasyCCDev and EasyCCPro
Both libraries use the same Lexical and Syntax algorithms, and switching from one library to another requires minor changes. 
* EasyCCDev (EasyCC development mode) takes the JSON files as arguments in the final executable. This is useful during the development phase because the program does not need to be recompiled to apply changes in the JSON files.
* EasyCCPro (EasyCC production mode) takes the JSON files as arguments to the CMake. The compile process for your program will be composed of two steps. First is embedding the JSON files into the executable. Second is compiling your program with the generated files.

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
