# Syntax analysis

### Grammar

#### Productions syntax

```
START: HEADER BODY FOOTER
HEADER: HELLO EASYCC
BODY: WORD BODY | NUMBER BODY
FOOTER: BYE EASYCC
HELLO: 'T_HELLO'
NUMBER: 'T_NUMBER'
EASYCC: 'T_EASYCC'
WORD: 'T_WORD'
```

Each line should be in one of the following forms:

* A non-terminal (Left-hand side of `:`) is defined as a set of a non-terminals (Right-hand sde of `:`)
```
NONTERMINAL : NONTERMINAL NONTERMINAL ...
```

* A non-terminal (Left-hand side of `:`) is defined as a terminal (Right-hand side of `:`). This is required for a better error recovery
```
NONTERMINL : 'TERMINAL'
```

* A vertical bar followed by a set of non-terminals or one trminal
```
| NONTERMINAL NONTERMINAL NONTERMINAL ...
```
or 
```
| TERMINAL
```

#### Definitions
* `:` is a delimiter in a production. Sometimes represented as an arrow `->`
* `|` is an `or` symbol. To avoid writting the same left-hand side non-terminal for different productions, the `|` allows defining another set of non-terminal for the same non-terminal
* `NONTERMINAL` can be compsed of upper case letters and underscores only
* `TERMINAL` must begin and end with a single quote. The text in between the single quotes must be a defined lexical token name (case sensitive).
* Whitespaces between tokens are ignored
