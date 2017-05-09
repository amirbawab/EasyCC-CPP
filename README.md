EasyCC C++
============

### Requirements
* Boost 1.63.0
```
# Remove old Boost (Optional)
sudo apt-get autoremove libboost1.58-dev

# Start a new installation
cd /tmp
# Download Boost 1.63.0 http://www.boost.org/users/history/version_1_63_0.html
wget https://downloads.sourceforge.net/project/boost/boost/1.63.0/boost_1_63_0.tar.gz?r=&ts=1485798206&use_mirror=pilotfiber
tar xvf boost_1_63_0.tar.gz*
cd boost_1_63_0/
sudo apt-get update
sudo apt-get install build-essential g++ python-dev autotools-dev libicu-dev build-essential libbz2-dev libboost-all-dev

# Build then install
./bootstrap.sh --prefix=/usr/local
./b2
sudo ./b2 install
```
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

### To do
* ~~Handle extra special characters for lexical phase (\n,\r, space and tab)~~
* Replace shared_ptr by star
* Handle auto detection of new line in lexical analysis, and remove it from configuration
