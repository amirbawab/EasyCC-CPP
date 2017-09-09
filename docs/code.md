# Code generation

## Description
Code generation is the phase where the the user input is translated into a language that can be 
processed by another program or at a lower level.

## How to generate code?
Code generation can be done inside the action handler function by writing to files or printing on the 
console. The phase number, passed as argument to the action handler, can be useful for code generation since
the last phase can be dedicated for generating code while the previous ones can validate the input semantics.
