# Syntax analysis

### Grammar

#### Productions syntax

```
START -> HEADER BODY FOOTER
HEADER -> HELLO EASYCC
BODY -> WORD BODY | NUMBER BODY
FOOTER -> BYE EASYCC
HELLO -> 'T_HELLO'
NUMBER -> 'T_NUMBER'
EASYCC -> 'T_EASYCC'
WORD -> 'T_WORD'
```

Each line should be in one of the following forms:

```
NONTERMINAL -> NONTERMINAL NONTERMINAL ...
```
or 
```
NONTERMINL -> 'TERMINAL'
```
or 
```
| NONTERMINAL NONTERMINAL NONTERMINAL ...
```
or 
```
| TERMINAL
```

#### Definitions
* `->` is a delimiter in a production
* `|` is an `or` symbol. To avoid writting the same left-hand side non-terminal for different productions, the `|` allows defining another set of tokens for the same non-terminal
* `NONTERMINAL` can be compsed of upper case letters and underscores only
* `TERMINAL` must begin and end with a single quote. The text in between the single quotes must be a defined lexical token name (case sensitive) and should not contain spaces.
* `EPSILON` represents an epsilon production
* Whitespaces between tokens are ignored

#### LL(1) - (left to right, leftmost derivation, 1 lookahead)
The grammar provided by the user should satisfy the following conditions:
* No left recursion
* Given a production: `A -> B | C`, First(B) ∩ First(C) = {}
* Given a production: `A -> B | C`, if ε ∈ First(A) then First(A) ∩ Follow(A) = {} 
