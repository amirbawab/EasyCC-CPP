# Lexical analysis

### State machine

#### JSON
The state machine is provided as a json file with the follwoing syntax:
```
{
  "states": [
    {
      "type": "...",
      "id": ...,
      "token": "...",
      "backtrack": ...
    },
    ...
  ],
  "transitions": [
    {
      "from": ...,
      "to": ...,
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
    - `NEW_LINE`: New line (\n)<br/>
    - `RETURN`: Return (\r)<br/>
    - `SPACE`: " " (without quotes)<br/>
    - `TAB`: Tab (\t)<br/>
    - `LOWER_CASE_LETTER`: A letter from a to z<br/>
    - `UPPER_CASE_LETTER`: A letter from A to Z<br/>
    - `POSITIVE`: A positive digit 1-9<br/>
    - `OTHER`: Any other character which no special path has been defined for it. This is often useful for tokens that need to backtrack on the final state and for detecting syntax errors.<br/>
    All INITIAL and NORMAL states must have a transition labeled `OTHER` which handles the case of no match found.<br/>
    If a character was defined as a transition between two states, and a special transition value that includes the same character was also defined between the same two states, then the priority goes to the one defined explicitly.
    </td>
  </tr>
</table>

#### Additional details
- A token value contains only characters that appear on a transition from any source state to any non-initial state. For example, if the initial state has a transition on letter `A` that goes to itself. Then `A` will never appear at the beginning of any token value. However, if the initial state have a transition on letter `B` that goes to another normal state, then `B` will be recorded as part of the token value.
- The folloing tokens names are reserved and should not be used by the user:
  * `:any`
  * `$`

### Configuration

#### JSON

```
{
    "newline": "...",
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
    },

    "reserved": {
        "...": {
            "...": "...",
            ...
        }
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
    <td>-</td>
    <td>-</td>
    <td>newline</td>
    <td>
    Line separator can be:<br/>
    - `CR`: \r<br/>
    - `LF`: \n<br/>
    - `CRLF`: \r\n<br/>
    </td>
    <td>-</td>
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
  <tr>
    <td>reserved</td>
    <td>Sometimes a meaningfull token can be generated by an existing path in the state machine. For example, consider a final state that generates a token name `T_WORD` which has a value composed of lower case letters only. And let's say we are interested in keepting track of a token name `T_IF` for a value of `if`. Since `if` is composed of lower case letters only, then it will automatically be labeled `T_WORD`. Therefore, we can use the `reserved` object to allow the compiler to verify if the generated token has a value that is reserved. If a match was found, then the token name will be updated with the new assigned one.</td>
    <td>
   Token name. The name of the token for which a possible new token name might replace the existing one. Token name must exist in the state machine.
    </td>
    <td>
   Map of keys with their corresponding values. (check next row for more details)
    </td>
    <td>Mutliple entries can be placed in the scope of the value object. Those information will be validated directly after generating the tokens by the state machine.</td>
  </tr>
  <tr>
    <td>Token name</td>
    <td>The token name defined in the previous row</td>
    <td>Token value</td>
    <td>Token name</td>
    <td>
      Values existing in the map keys will have their token names updated twice in the program. The first time is done by the state machine. And the second time is done by the program while verifying if the token value is reserved by another token name. <br/>
      Note: This strategy is very powerful and saves a lot of work that originally would have been done on the state machine. However, avoid using this method if the value can be constructed by a path where no self-loop exists on a state that belongs to that path, except for the inital state.<br/>
      For example, a bad usage of the `reserved` object is as follow:<br/>
      Consider the state machine has two states 0, an initial state, and 1, a final state with token name `T_AB`. To go from state 0 to state 1, it needs to read a character `A` or a character `B`. Then in the `reserved` object, the token name `T_AB` has two map entries with keys and values `A`: `T_A` and `B`: `T_B`. This situation makes the program do more work by updating the token name twice while it can easily be done on the state machine.<br/>
      A good usage of the `reserved` object is as described previously, if have a token name which has its value composed of lower case letters only, and multiple words that belongs to this token name need to be updated by different token names.<br/>
      Note: An error token cannot be part of the `reserved` object because that means that the state machine is incomplete and the new token name can be added to the state machine.<br/>
    </td>
  </tr>
</table>

### Error messages

#### JSON

```
{
    "default_message": "...",
    "error_messages": {
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
    <td>-</td>
    <td>default_message</td>
    <td>
    Default message displayed in case no specific message was found for a token name. To display information about the token, one can use the following special variables:<br/>
    - `${value}`: Display the value of the token object.<br/>
    - `${column}`: Display the column number in the line starting from value 1.<br/>
    - `${line}`: Display the line number in the text starting from value 1.<br/>
    </td>
  </tr>
 <tr>
    <td>error_messages</td>
    <td>Token name. If the token name does not exist or is not classified as error token, then the message will never be displayed.</td>
    <td>
    Error message. To display information about the token, one can use the following special variables:<br/>
    - `${value}`: Display the value of the token object.<br/>
    - `${column}`: Display the column number in the line starting from value 1.<br/>
    - `${line}`: Display the line number in the text starting from value 1.<br/>
    </td>
  </tr>
  
</table>
