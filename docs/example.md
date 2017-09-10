# Calculator example

## Motivation
Calculator example is a very simple compiler that takes a list of expressions seperated by `;` as input, then evalutes each and stores the answers in a file.

### input.txt
```bash
# Expr 1
1 + 2 * 3;

# Expr 2
(1 + 2) * 3;
```

### output.txt
```bash
Expression result: 7
Expression result: 9
```

## Project setup
The calculator project can be found under `examples/` in the repo root directory. In this example,
the content of the two main files will not be explained because they only provide the command line
interface to the executables.

### Structure
```
calculator
├── CMakeLists.txt
├── include
│   └── calculator
│       └── Calculator.h
├── resources
│   └── src
│       ├── input.txt
│       ├── lexical_config.json
│       ├── lexical_errors.json
│       ├── lexical_state_machine.json
│       ├── syntax_errors.json
│       └── syntax_grammar.json
└── src
    └── calculator
        ├── Calculator.cpp
        ├── maindev.cpp
        └── mainpro.cpp

```
### CMakeLists.txt
Refer to the <a href="../">home page</a> for the sample `CMakeLists.txt`
```cmake
cmake_minimum_required(VERSION 3.5)
project(calculator)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CALCULATOR_DEV_EXEC "calculatordev")
set(CALCULATOR_PRO_EXEC "calculator")

# Add easycc CMakeLists.txt to build it automatically
add_subdirectory(${PROJECT_SOURCE_DIR}/../../ ${PROJECT_BINARY_DIR}/EasyCC-CPP)

# Compile with -std=c++11 flag
add_compile_options(-std=c++11)

# Configure directory of output file
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include calculator header files (e.g. same as -I flag)
include_directories(include)
include_directories(../../include)
include_directories(../../rapidjson/include)

# Store cpp files in a variable
file(GLOB_RECURSE CALC_PROJECT_SOURCE_FILES src/*/*.cpp)
list(REMOVE_ITEM CALC_PROJECT_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/src/calculator/maindev.cpp"
        "${PROJECT_SOURCE_DIR}/src/calculator/mainpro.cpp")

# Add the executables
add_executable(${CALCULATOR_DEV_EXEC} ${CALC_PROJECT_SOURCE_FILES} src/calculator/maindev.cpp)
add_executable(${CALCULATOR_PRO_EXEC} ${CALC_PROJECT_SOURCE_FILES} src/calculator/mainpro.cpp)

# Link library to the executable
target_link_libraries(${CALCULATOR_DEV_EXEC} easyccdev)
target_link_libraries(${CALCULATOR_PRO_EXEC} easyccpro)

```

## Lexical analysis
### I - lexical_state_machine.json 
An image of the state machine is provided <a href="#state-machine-image">below</a>.  
Refer to the <a href="../lexical/#i-state-machine">state machine section</a> for more details.
```
{
    "states": [
        {
            "type": "INITIAL",
            "id": 0
        },
        {
            "type": "NORMAL",
            "id": 1
        },
        {
            "type": "FINAL",
            "id": 2,
            "token": "T_INTEGER",
            "backtrack": true
        },
        {
            "type": "FINAL",
            "id": 3,
            "token": "T_PLUS",
            "backtrack": false
        },
        {
            "type": "FINAL",
            "id": 4,
            "token": "T_MINUS",
            "backtrack": false
        },
        {
            "type": "FINAL",
            "id": 5,
            "token": "T_MULTIPLY",
            "backtrack": false
        },
        {
            "type": "FINAL",
            "id": 6,
            "token": "T_DIVIDE",
            "backtrack": false
        },
        {
            "type": "FINAL",
            "id": 7,
            "token": "T_SEMICOLON",
            "backtrack": false
        },
        {
            "type": "FINAL",
            "id": 8,
            "token": "T_INVALID_CHAR",
            "backtrack": false
        },
        {
            "type": "FINAL",
            "id": 9,
            "token": "T_INTEGER",
            "backtrack": false
        },
        {
            "type": "FINAL",
            "id": 10,
            "token": "T_OPEN_PAR",
            "backtrack": false
        },
        {
            "type": "FINAL",
            "id": 11,
            "token": "T_CLOSE_PAR",
            "backtrack": false
        },
        {
            "type": "NORMAL",
            "id": 12
        },
        {
            "type": "FINAL",
            "id": 13,
            "token": "T_COMMENT",
            "backtrack": true
        }
    ],
    "transitions": [
        {
            "from": 0,
            "to": 0,
            "chars": ["SPACE","RETURN","TAB","NEW_LINE","EOF"]
        },
        {
            "from": 0,
            "to": 1,
            "chars": ["POSITIVE"]
        },
        {
            "from": 1,
            "to": 1,
            "chars": ["POSITIVE","0"]
        },
        {
            "from": 1,
            "to": 2,
            "chars": ["OTHER"]
        },
        {
            "from": 0,
            "to": 3,
            "chars": ["+"]
        },
        {
            "from": 0,
            "to": 4,
            "chars": ["-"]
        },
        {
            "from": 0,
            "to": 5,
            "chars": ["*"]
        },
        {
            "from": 0,
            "to": 6,
            "chars": ["/"]
        },
        {
            "from": 0,
            "to": 7,
            "chars": [";"]
        },
        {
            "from": 0,
            "to": 8,
            "chars": ["OTHER"]
        },
        {
            "from": 0,
            "to": 9,
            "chars": ["0"]
        },
        {
            "from": 0,
            "to": 10,
            "chars": ["("]
        },
        {
            "from": 0,
            "to": 11,
            "chars": [")"]
        },
        {
            "from": 0,
            "to": 12,
            "chars": ["#"]
        },
        {
            "from": 12,
            "to": 12,
            "chars": ["OTHER"]
        },
        {
            "from": 12,
            "to": 13,
            "chars": ["NEW_LINE","EOF"]
        }
    ]
}
```

The graph below was generated by the tool described in the 
<a href="../lexical/#gui-to-json">lexical analysis page</a>.
<img id="state-machine-image" align="center" width="300" src="../state-machine.png" alt="state machine">

### II - lexical_config.json
* The lexical tokens `T_COMMENT` are ignored, so they will not be passed to the syntax analyzer.
* The lexical tokens `T_INVALID_CHAR` are marked as error tokens and will report 
<a href="#iii-lexical_errorsjson">error messages</a> once created.
* The lexical tokens `T_INTEGER` which have the value `0` will be renamed to `T_ZERO`.

Refer to the <a href="../lexical/#ii-lexical-configuration">lexical configuration section</a> for more details.
```json
{
    "newline": "LF",

    "ignore": {
        "prefix": "",
        "suffix": "",
        "include": ["T_COMMENT"],
        "exclude": []
    },

    "error": {
        "prefix": "T_INVALID_CHAR",
        "suffix": "",
        "include": [],
        "exclude": []
    },

    "reserved": {
        "T_INTEGER" : {
            "0" : "T_ZERO"
        }
    }
}
```

### III - lexical_errors.json
* Default message is printed if no message was assigned to an error token (In this case, default message is
never printed).
* `T_INVALID_CHAR` will print its assigned message once created.
* Placeholders are replaced with their corresponding values. Check the full list of 
<a href="../lexical/#optional-placeholders">lexical error message placeholders</a>.

Refer to the <a href="../lexical/#iii-lexical-error-messages">lexical error message section</a> for 
more details.
```
{
    "default_message": "${filename}: Error found '${value}' at line ${line} and column ${column}",
    "error_messages": {
        "T_INVALID_CHAR" : "${filename}: Invalid character '${value}' found at line ${line} and column ${column}"
    }
}
```

## Syntax analysis
### I - syntax_grammar.json
* The grammar takes care of the operator precedence.
* Notice the 4 different types of items in the grammar:
    * Terminal
    * Non-terminal
    * Epsilon
    * Action token

Refer to the <a href="../syntax/#i-grammar">grammar section</a> for more details.
```
{
    "S"             : ["A T_SEMICOLON #print# S #end#","EPSILON"],
    "A"             : ["C B"],
    "B"             : ["EPSILON","T_PLUS C #plus# B","T_MINUS C #minus# B"],
    "C"             : ["E D"],
    "D"             : ["EPSILON","T_MULTIPLY E #multiply# D","T_DIVIDE #divide# E D"],
    "E"             : ["T_OPEN_PAR A T_CLOSE_PAR","NUMBER #push#"],
    "NUMBER"        : ["'T_INTEGER'","'T_ZERO'"],
    "T_PLUS"        : ["'T_PLUS'"],
    "T_MINUS"       : ["'T_MINUS'"],
    "T_MULTIPLY"    : ["'T_MULTIPLY'"],
    "T_DIVIDE"      : ["'T_DIVIDE'"],
    "T_OPEN_PAR"    : ["'T_OPEN_PAR'"],
    "T_CLOSE_PAR"   : ["'T_CLOSE_PAR'"],
    "T_SEMICOLON"   : ["'T_SEMICOLON'"]
}
```

### II - syntax_errors.json
* Default message is printed if the syntax analyzer is in panic mode and did not find an error message
associated to the situation.
* If the syntax analyzer was expecting anything `:any` but a `T_INTEGER`, then it will print the
corresponding error message.
* Similarly, if the syntax was expecting anything `:any` but a `T_ZERO`, then it will print the 
corresponding error message.
* Placeholders are replaced with their corresponding values. Check the full list of 
<a href="../syntax/#optional-placeholders">syntax error message placeholders</a>.
* For the list of special values for `non_terminal` and `terminal` keys, check the table in the 
<a href="../syntax/#error-message-object">syntax error messages section</a>.

Refer to the <a href="../syntax/#ii-syntax-error-messages">syntax error messages section</a> for more 
details.
```
{
    "default_message": "${filename}: Error found: ${lexical.value} at line ${lexical.line} and column ${lexical.column}",
    "error_messages": [
        {
            "non_terminal": ":any",
            "terminal": "T_INTEGER",
            "message": "${filename}: Expecting a symbol before '${lexical.value}' at line ${lexical.line} and column ${lexical.column}"
        },
        {
            "non_terminal": ":any",
            "terminal": "T_ZERO",
            "message": "${filename}: Expecting a symbol before '0' at line ${lexical.line} and column ${lexical.column}"
        }
    ]
}
```

## Semantic analysis & Code generation
### Calculator.cpp
#### Calculator::compile()
* Initialize handler (explained <a href="#calculatorinithandlers">below</a>)
* Open output file for code generation.
* Configure EasyCC library.
    * `setParsingPhase(0)`: sets the phase value which is passed as argument to action handlers. 
For languages that require analyzing the syntax mutliple times, the phase value can be set to a counter
that increments on every iteration.
    * `setSilentSyntaxErrorMessages(false)`: If a language is parsed several times, then only one iteration
needs to report syntax errors. 
    * `setSilentSemanticEvent(false)`: If `true` is passed as argument, then no action handlers will be
executed.
    * `setOnSyntaxError(function)`: Register a function that is executed when a syntax error occurs. In the
calculator example, if a syntax error occurs the program writes a message to the output file and closes it.
* Loop on input files and compile them.
```c++
int Calculator::compile(std::vector<std::string> inputFiles, std::string outputFile) {

    // Initialize semantic action handlers
    initHandlers();

    // Set output file
    m_output.open(outputFile);

    // Configure easycc
    m_easyCC->setParsingPhase(0);
    m_easyCC->setSilentSyntaxErrorMessages(false);
    m_easyCC->setSilentSemanticEvents(false);
    m_easyCC->setOnSyntaxError([&](){
        m_easyCC->setSilentSemanticEvents(true);
        m_output << "Error evaluating expression" << std::endl;
        m_output.close();
    });

    // Compile all files
    for(std::string fileName : inputFiles) {
        int code = m_easyCC->compile(fileName);
        if(code != ecc::IEasyCC::OK_CODE) {
            return code;
        }
    }
    return 0;
}
```

#### Calculator::initHandlers()

* Register an action handler for `#push#`
    * Once a number is parsed, push it to the stack
* Register an action handler for `#plus#`
    * Once a `+` is parsed, pop the last two numbers and push the result to the stack
* Register an action handler for `#minus#`
    * Once a `-` is parsed, pop the last two numbers and push the result to the stack
* Register an action handler for `#multiply#`
    * Once a `*` is parsed, pop the last two numbers and push the result to the stack
* Register an action handler for `#divide#`
    * Once a `/` is parsed, pop the last two numbers and push the result to the stack
* Register an action handler for `#print#`
    * Write final result to the file at the end of each expression.
* Register an action handler for `#end#`
    * Close the output file at the end of the input file

```c++
void Calculator::initHandlers() {
    /**
     * Register 'push' semantic action
     * Every time an integer is read it will be pushed into the stack
     */
    m_easyCC->registerSemanticAction("#push#",[&](int phase, Tokens &lexicalVector, int index){
        m_operands.push(std::stoi(lexicalVector[index]->getValue()));
    });

    /**
     * Register 'plus' semantic action
     * Once the two operands are pushed, add them and push the result into the stack
     */
    m_easyCC->registerSemanticAction("#plus#",[&](int phase, Tokens &lexicalVector, int index){
        int popR = m_operands.top();
        m_operands.pop();
        int popL = m_operands.top();
        m_operands.pop();
        m_operands.push(popL + popR);
    });

    /**
     * Register 'minus' semantic action
     * Once the two operands are pushed, subtract them and push the result into the stack
     */
    m_easyCC->registerSemanticAction("#minus#",[&](int phase, Tokens &lexicalVector, int index){
        int popR = m_operands.top();
        m_operands.pop();
        int popL = m_operands.top();
        m_operands.pop();
        m_operands.push(popL - popR);
    });

    /**
     * Register 'multiply' semantic action
     * Once the two operands are pushed, multiply them and push the result into the stack
     */
    m_easyCC->registerSemanticAction("#multiply#",[&](int phase, Tokens &lexicalVector, int index){
        int popR = m_operands.top();
        m_operands.pop();
        int popL = m_operands.top();
        m_operands.pop();
        m_operands.push(popL * popR);
    });

    /**
     * Register 'divide' semantic action
     * Once the two operands are pushed, divide them and push the result into the stack
     */
    m_easyCC->registerSemanticAction("#divide#",[&](int phase, Tokens &lexicalVector, int index){
        int popR = m_operands.top();
        m_operands.pop();
        int popL = m_operands.top();
        m_operands.pop();
        m_operands.push(popL / popR);
    });

    /**
     * Register 'print' semantic action
     * At the end of the expression, output the result to a file
     */
    m_easyCC->registerSemanticAction("#print#",[&](int phase, Tokens &lexicalVector, int index){
        m_output << "Expression result: " << m_operands.top() << std::endl;
        m_operands.pop();
    });

    /**
     * Register 'end' semantic action
     * Once the end of file is reached, close the output file
     */
    m_easyCC->registerSemanticAction("#end#",[&](int phase, Tokens &lexicalVector, int index){
        m_output.close();
    });
}
```

## Build the Calculator project
### cmake
```
cmake . \
    -DSYNTAX_ERRORS="resources/src/syntax_errors.json"\
    -DSYNTAX_GRAMMAR="resources/src/syntax_grammar.json"\
    -DLEXICAL_ERRORS="resources/src/lexical_errors.json"\
    -DLEXICAL_CONFIG="resources/src/lexical_config.json"\
    -DLEXICAL_STATE_MACHINE="resources/src/lexical_state_machine.json"

```

### make `calculator` production mode
```
# Generate header file
make generate_files

# Run cmake again (section above)
# cmake . ...

# Build project
make calculator

```

### make `calculator` developer mode
```
make calculatordev
```

## Use Calculator compiler

### Production mode
```
./bin/calculator resources/src/input.txt -o output.txt
```

### Developer mode
```
./bin/calculatordev \
    -s resources/src/lexical_state_machine.json \
    -c resources/src/lexical_config.json \
    -e resources/src/lexical_errors.json \
    -g resources/src/syntax_grammar.json \
    -E resources/src/syntax_errors.json \
    -o /tmp/result.txt \
    -v \
    resources/src/input.txt
```
