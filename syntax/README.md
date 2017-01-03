# Syntax analysis

### Grammar

#### Syntax

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

* A non-terminal is defined as a set of a non-terminals
```
NONTERMINAL : NONTERMINAL NONTERMINAL ...
```

* A non-terminal is defined as a terminal (This is required for a better error recovery)
```
NONTERMINL : 'TERMINAL'
```

* A vertical bar followed by a set of non-terminals
```
| NONTERMINAL NONTERMINAL NONTERMINAL ...
```
#### Definitions

* `NONTERMINAL` must be compsed of upper case letters only
* `TERMINAL` must begin and end with a single quote. The text in between the single quotes must be a defined lexical token name (case sensitive).
