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

For examples on how to use each library, check the calculator program in the `examples` directory.
