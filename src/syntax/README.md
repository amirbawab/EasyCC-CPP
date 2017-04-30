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

#### Note
* Terminals and Non-Terminals should not match any reserved word from the following list:
  * `:any`
  * `$`

### Configuration

#### JSON

```
{
    "parsing_phases": "..."
}
```

#### Explanation

<table>
  <tr>
    <th>Object</th>
    <th>Value</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>parsing_phases</td>
    <td>number</td>
    <td>The number of parsing iterations. This is useful for parsing languages where during the first parsing iteration the 
    classes are built, and in the second iteration the semantic check is validated.</td>
  </tr>
</table>

### Error messages

#### JSON

```
{
    "default_message": "...",
    "error_messages": [
        {
            "non_terminal": "...",
            "terminal": "...",
            "message": "..."
        },
        ...
    ]
}
```

#### Explanation

<table>
  <tr>
    <th>Object</th>
    <th>Data</th>
    <th>Value</th>
  </tr>
  <tr>
    <td>-</td>
    <td>default_message</td>
    <td>
    Default message displayed in case no specific message was found. To display information about the token, one can use the following special variables:<br/>
    - `${lexical[.next|.previous]*.value}`: Display the value of the token object.<br/>
    - `${lexical[.next|.previous]*.column}`: Display the column number in the line starting from value 1.<br/>
    - `${lexical[.next|.previous]*.line}`: Display the line number in the text starting from value 1.<br/>
    </td>
  </tr>
 <tr>
    <td rowspan="3">error_messages</td>
    <td>non-terminal</td>
    <td>
      Non-terminal in which the error occurred at<br/>
      Use `:any` to specify any non-terminal
    </td>
  </tr>
 <tr>
    <td>terminal</td>
    <td>
      Terminal that was not expected<br/>
      Use `:any` to specify any terminal
    </td>
  </tr>
 <tr>
    <td>message</td>
    <td>
      Message printed when an error happens while processing a non-terminal and an unexpected terminal is scanned. To display information about the token, one can use the following special variables:<br/>
    - `${lexical[.next|.previous]*.value}`: Display the value of the token object.<br/>
    - `${lexical[.next|.previous]*.column}`: Display the column number in the line starting from value 1.<br/>
    - `${lexical[.next|.previous]*.line}`: Display the line number in the text starting from value 1.<br/> 
    </td>
  </tr>
</table>
