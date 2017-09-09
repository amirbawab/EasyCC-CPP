# Semantic analysis

## Description
Semantic anlysis is applied while the syntax analyzer matches the user code with its corresponding grammar
productions. The semantic analysis phase introduces action tokens, responsible for performing actions at 
specific locations in the language grammar.

## What is an action token?
An action token is a special token that can be injected at several locations in the language grammar. 
The added tokens do not affect the syntax of the language, but they inform the syntax analyzer that at this 
location in the grammar a particular action need to be performed. In its turn, the syntax anlyzer
passes the action to the semantic analyzer which can map each action to its corresponding handler function.

## What is an action handler?
An action handler is a function that is executed by the semantic analyzer when the syntax analyzer encounters
an action token. When an action hander is called, it will be provided with the last lexical token parsed
by the syntax analyzer, and the phase number which is usally helpful when an input file needs to be scanned
more than one time.

## I - Add action tokens to the grammar
An action token is represented by a string surrounded by a `#` symbol on both sides. Each action token in
the grammar reports information about the most recent lexical token that was consumed by the syntax analyzer.

For example, consider the following grammar:
```json
{
    "...",
    "CLASS_DEFINITION"  : "CLASS #newClass# CLASS_NAME #setClassName# CLASS_BODY #endClass#",
    "CLASS"             : "'class'",
    "CLASS_NAME"        : "'identifier'",
    "CLASS_BODY"        : "OPEN_CURLY STATEMENT CLOSE_CURLY",
    "..."
}
```
The above grammar demonstrates how action tokens help in constructing a class:

* `#newClass#` action handler can construct a new instance of a class.
* `#setClassName#` action handler can set the name of the created class. 
    * Notice that the action token is placed right after the lexical token `'identifier'` because it
provides the handler function with the necessary arguments to identify the exact lexical token.
* `#endClass#` action handler can delete the created class.

## II - Create action handlers
An action handler can be registered using the function`IEasyCC::registerSemanticAction()`. Here is a simple
code showing how the action tokens in the previous example can be registered.
```c++
typedef std::vector<std::shared_ptr<ecc::LexicalToken>> Tokens;

easyCC->registerSemanticAction("#newClass#",[&](int phase, Tokens &lexicalVector, int index){
    newClass = new MyClass();
});

easyCC->registerSemanticAction("#setClassName#",[&](int phase, Tokens &lexicalVector, int index){
    newClass->setName(lexicalVector[index]); // Get the last lexical token parsed
                                             // by the syntax analyzer
});

easyCC->registerSemanticAction("#endClass#",[&](int phase, Tokens &lexicalVector, int index){
    delete newClass;
});
```
