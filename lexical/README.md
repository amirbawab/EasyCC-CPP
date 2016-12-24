# Lexial analysis

### State machine

#### JSON
The state machine is provided as a json file with the follwoing syntax:
```
{
  "states": [
    {
      "type": "...",
      "id": "...",
      "token": "...",
      "backtrack": "..."
    },
    ...
  ],
  "transitions": [
    {
      "from": "...",
      "to": "...",
      "chars": [
        "...", ...
      ]
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
    <th>Description</th>
  </tr>
  <tr>
    <td rowspan="4">state</td>
    <td>type</td>
    <td>`INITIAL`, `NORMAL` or `FINAL`</td>
    <td>
      A state type:<br/>
      - `INITIAL`: Initial state. Exactly one state must be marked as initial<br/>
      - `NORMAL`: Normal state<br/>
      - `FINAL`: Final state. A final state must define the values for "token" and "backtrack"<br/>
    </td>
  </tr>
  <tr>
    <td>id</td>
    <td>0,1,2,...</td>
    <td>Unique id for each state</td>
  </tr>
  <tr>
    <td>token</td>
    <td>Token name. Example: `T_INTEGER`, `T_ERROR_LEADING_ZERO`, ...</td>
    <td>A Token name is very important for the next phase of the compiler which is the syntax analyzer. The grammar will be constructed based on a sequence of token names. This must be defined for final states only.</td>
  </tr>
  <tr>
    <td>backtrack</td>
    <td>`true` or `false`</td>
    <td>Backtrack is required when the only possible way to know that the token value has ended is by reading a character that is not part of the token value. This must be defined for final states only.</td>
  </tr>
  <tr>
    <td rowspan="3">transition</td>
    <td>from</td>
    <td>State id</td>
    <td>Transition starting state. State id must exist</td>
  </tr>
  <tr>
    <td>to</td>
    <td>State id</td>
    <td>Transition ending state. State id must exist</td>
  </tr>
  <tr>
    <td>chars</td>
    <td>Array of transition values</td>
    <td>
    Each value in the array will be treated as an independent transition that goes from `from` to `to`.<br/>
    The following are special values that will facilitate building the state machine:<br/>
    - `EOF`: End of file<br/>
    - `LOWER_CASE_LETTER`: A letter from a to z<br/>
    - `UPPER_CASE_LETTER`: A letter from A to Z<br/>
    - `DIGIT`: A digit between (inclusive) 0 and 9<br/>
    - `POSITIVE`: A positive digit<br/>
    - `OTHER`: Any other character which no special path has been defined for it. This is often useful for tokens that need to backtrack on the final state and for detecting syntax errors. All INITIAL and NORMAL states must have a transition labeled `OTHER`<br/>
    </td>
  </tr>
</table>

### Configuration

#### JSON

```
{
    "ignore": {
        "prefix": "...",
        "suffix": "...",
        "include": ["...", ...],
        "exclude": ["...", ...]
    },

    "error": {
        "prefix": "...",
        "suffix": "...",
        "include": ["...", ...],
        "exclude": ["...", ...]
    }
}
```

#### Explanation

<table>
  <tr>
    <th>Object</th>
    <th>Definition</th>
    <th>Data</th>
    <th>Value</th>
    <th>Description</th>
  </tr>
  <tr>
    <td rowspan="4">ignore</td>
    <td rowspan="4">
      Ignored tokens will not pass to the next phase of the compiler which is the Syntax analysis. Ignoring tokens is usually useful when parsing comments. Such tokens can be placed anywhere in the grammar and should not result in any error.
    </td>
    <td>prefix</td>
    <td>Prefix of token names</td>
    <td>Ignore token names starting with `prefix`</td>
  </tr>
  <tr>
    <td>suffix</td>
    <td>Suffix of token names</td>
    <td>Ignore token names ending with `suffix`</td>
  </tr>
  <tr>
    <td>include</td>
    <td>List of token names</td>
    <td>Ignore token names that exist in the array</td>
  </tr>
  <tr>
    <td>exclude</td>
    <td>List of token names</td>
    <td>Don't ignore token names that exist in the array, even if they appear in `prefix`, `suffix` or `include`</td>
  </tr>
  <tr>
    <td rowspan="4">error</td>
    <td rowspan="4">
    Error tokens will stop the program at the end of the lexical analysis phase. If an error message is assigned to the token name, it will be displayed on the screen.
    </td>
    <td>prefix</td>
    <td>Prefix of token names</td>
    <td>Consider token names starting with `prefix` as error tokens</td>
  </tr>
  <tr>
    <td>suffix</td>
    <td>Suffix of token names</td>
    <td>Consider token names ending with `suffix` as error tokens</td>
  </tr>
  <tr>
    <td>include</td>
    <td>List of token names</td>
    <td>Consider token names that exist in the array as error tokens</td>
  </tr>
  <tr>
    <td>exclude</td>
    <td>List of token names</td>
    <td>Don't consider token names that exist in the array as error tokens, even if they appear in `prefix`, `suffix` or `include`</td>
  </tr>
</table>

### Error messages

#### JSON

```
{
    "messages": {
        "..." : "...",
        ...
    }
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
    <td>messages</td>
    <td>Token name. If the token name does not exist or is not classified as error token, then the message will never be displayed.</td>
    <td>
    Error message. To display information about the token, one can use the following special variables:<br/>
    - `${lexical.value}`: Display the value of the token object.<br/>
    - `${lexical.column}`: Display the column number in the line starting from value 1.<br/>
    - `${lexical.line}`: Display the line number in the text starting from value 1.<br/>    
    </td>
  </tr>
  
</table>
