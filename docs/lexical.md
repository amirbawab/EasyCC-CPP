# Lexical analysis

## Description
The lexical analysis is the first phase in compiler design where the user input is scanned and converted 
into a sequence of tokens. The generated lexical tokens are then provided as input to the syntax analyzer.

## I - State machine
A state machine is a graph composed of vertices connected by edges. The graph has a single starting vertex
and one or more middle or final tokens. Landing on a final vertex means that a new token is created.
Edges allow input characters to traverse the graph by assigning a letter label to each edge.  

### JSON structure
<b>Global keys table</b>
<table>
    <tr>
        <th>Key</th>
        <th>Description</th>
        <th>Value</th>
        <th>Required</th>
    </tr>
    <tr>
        <td>states</td>
        <td>List of states/vertices of the state machine</td>
        <td>Array of <a href="#state-object">state objects</a></td>
        <td>Yes</td>
    </tr>
    <tr>
        <td>transitions</td>
        <td>List of transitions/edges of the state machine</td>
        <td>Array of <a href="#transition-object">transition objects</a></td>
        <td>Yes</td>
    </tr>
</table>

<b id="state-object">State object table</b>
<table>
    <tr>
        <th>Key</th>
        <th>Key description</th>
        <th>Value</th>
        <th>Value description</th>
        <th>Required</th>
    </tr>
    <tr>
        <td rowspan="3">type</td>
        <td rowspan="3">Type of the state</td>
        <td><code>Initial</code></td>
        <td>Initial state. Exactly one state must be marked as initial</td>
        <td rowspan="3">Yes</td>
    </tr>
    <tr>
        <td><code>NORMAL</code></td>
        <td>Normal state.</td>
    </tr>
    <tr>
        <td><code>FINAL</code></td>
        <td>Final state.</td>
    </tr>
    <tr>
        <td>id</td>
        <td>Unique id for each state</td>
        <td>0, 1, 2, ...</td>
        <td>Counting starts from 0 and should be consecutive</td>
        <td>Yes</td>
    </tr>
    <tr>
        <td>token</td>
        <td>Name of the token created when landing on a final state</td>
        <td>string (e.g. <code>T_SEMICOLON</code>)</td>
        <td>Cannot be a <a href="syntax/#reserved-syntax-keyword">reserved syntax keyword</a></td>
        <td>Only for final states</td>
    </tr>
    <tr>
        <td rowspan="2">backtrack</td>
        <td rowspan="2">Backtrack one character in the user input. This is required when the only possible 
        way to know that the token value has ended is by reading a character that is not part of the token 
        value.</td>
        <td>false</td>
        <td>Do not backtrack</td>
        <td rowspan="2">Only for final states</td>
    </tr>
    <tr>
        <td>true</td>
        <td>Backtrack</td>
    </tr>
</table>

<b id="transition-object">Transition object table </b>
<table>
    <tr>
        <th>Key</th>
        <th>Key description</th>
        <th>Value</th>
        <th>Value description</th>
        <th>Required</th>
    </tr>
    <tr>
        <td>from</td>
        <td>State id, from source</td>
        <td>0, 1, 2, ...</td>
        <td>State id must exist and cannot correspond to a final state.</td>
        <td>Yes</td>
    </tr>
    <tr>
        <td>to</td>
        <td>State id, to destination</td>
        <td>0, 1, 2, ...</td>
        <td>State id must exist and cannot correspond to the initial state.</td>
        <td>Yes</td>
    </tr>
    <tr>
        <td>chars</td>
        <td>The list of characters that will traverse this edge</td>
        <td>Array of <a href="#char-values">char values</a></td>
        <td>Check table</td>
        <td>Yes, cannot be empty.</td>
    </tr>
</table>

<b id="char-values">Char values table</b>
<table>
    <tr>
        <th>Value</th>
        <th>Description</th>
    </tr>
    <tr>
        <td>Character (e.g. <code>=</code>)</td>
        <td>A single character</td>
    </tr>
    <tr>
        <td><code>EOF</code></td>
        <td>End of file</td>
    </tr>
    <tr>
        <td><code>NEW_LINE</code></td>
        <td>New line: <code>\n</code></td>
    </tr>
    <tr>
        <td><code>RETURN</code></td>
        <td>Return: <code>\r</code></td>
    </tr>
    <tr>
        <td><code>SPACE</code></td>
        <td>Single whitespace</td>
    </tr>
    <tr>
        <td><code>TAB</code></td>
        <td>Single tab: <code>\t</code></td>
    </tr>
    <tr>
        <td><code>LOWER_CASE_LETTER</code></td>
        <td>A letter from a to z</td>
    </tr>
    <tr>
        <td><code>UPPER_CASE_LETTER</code></td>
        <td>A letter from A to Z</td>
    </tr>
    <tr>
        <td><code>POSITIVE</code></td>
        <td>A positive digit from 1 to 9</td>
    </tr>
    <tr>
        <td><code>OTHER</code></td>
        <td>Any other character. Every non-final state must have a transition on <code>OTHER</code></td>
    </tr>
</table>

### GUI to JSON
#### Introduction
A state machine can get large and it will become hard to keep track of states and transitions. To reduce the
complexity of building state machines, use the `graph.html` tool located under `tools/gui/` in order to
build the machine using a graphical user interface and then export it to JSON. 

*Note: The tool is written in TypeScript and needs to be compiled before using it.*

#### Compile tool
```bash
cd tools/gui/ts/
tsc @compile
```

## II - Lexical configuration
Lexical configuration allow manipulating and tagging generated tokens.

### JSON structure

<b>Global keys table</b>
<table>
    <tr>
        <th>Key</th>
        <th>Key description</th>
        <th>Value</th>
        <th>Value description</th>
        <th>Required</th>
    </tr>
    <tr>
        <td rowspan="3">newline</td>
        <td rowspan="3">Line separator</td>
        <td>CR</td>
        <td><code>\r</code></td>
        <td rowspan="3">Yes</td>
    </tr>
    <tr>
        <td>LF</td>
        <td><code>\n</code></td>
    </tr>
    <tr>
        <td>CRLF</td>
        <td><code>\r\n</code></td>
    </tr>
    <tr>
        <td>ignore</td>
        <td>List of lexical tokens to be ignored (e.g. code comments)</td>
        <td><a href="#ignore-object">Ignore object</a></td>
        <td>Check table</td>
        <td>Yes</td>
    </tr>
    <tr>
        <td>error</td>
        <td>List of lexical tokens resulting in lexical error</td>
        <td><a href="#error-object">Error object</a></td>
        <td>Check table</td>
        <td>Yes</td>
    </tr>
    <tr>
        <td>reserved</td>
        <td>Manipulate token name once created. This is useful when the user input has a reserved
        keyword and the state machine marked it as a more general name.</td>
        <td><a href="#reserved-object">Reserved object</a></td>
        <td>Check table</td>
        <td>Yes</td>
    </tr>
</table>

<b id="ignore-object">Ignore object table</b>
<table>
    <tr>
        <th>Key</th>
        <th>Description</th>
        <th>Value</th>
    </tr>
    <tr>
        <td>prefix</td>
        <td>Token name prefix</td>
        <td>string</td>
    </tr>
    <tr>
        <td>suffix</td>
        <td>Token name suffix</td>
        <td>string</td>
    </tr>
    <tr>
        <td>include</td>
        <td>List of all the token names not covered by the <code>prefix</code> or <code>suffix</code></td>
        <td>Array of strings</td>
    </tr>
    <tr>
        <td>exclude</td>
        <td>List of all the token names covered by the <code>prefix</code> or <code>suffix</code> but should
        not be considered.</td>
        <td>Array of strings</td>
    </tr>
</table>

<b id="error-object">Error object table</b>
<table>
    <tr>
        <th>Key</th>
        <th>Description</th>
        <th>Value</th>
    </tr>
    <tr>
        <td>prefix</td>
        <td>Token name prefix</td>
        <td>string</td>
    </tr>
    <tr>
        <td>suffix</td>
        <td>Token name suffix</td>
        <td>string</td>
    </tr>
    <tr>
        <td>include</td>
        <td>List of all the token names not covered by the <code>prefix</code> or <code>suffix</code></td>
        <td>Array of strings</td>
    </tr>
    <tr>
        <td>exclude</td>
        <td>List of all the token names covered by the <code>prefix</code> or <code>suffix</code> but should
        not be considered.</td>
        <td>Array of strings</td>
    </tr>
</table>

<b id="reserved-object">Reserved object table</b>
<table>
    <tr>
        <th>Key</th>
        <th>Description</th>
        <th>Value</th>
    </tr>
    <tr>
        <td>Token name (e.g. <code>T_IDENTIFIER</code>)</td>
        <td>The original token name assigned to the lexical token</td>
        <td><a href="#reserved-token-object">Reserved token object</a></td>
    </tr>
    <tr>
        <td colspan="3"><i>Note: The above row can be applied several times in this object</i></td>
    </tr>
</table>

<b id="reserved-token-object">Reserved token object table</b>
<table>
    <tr>
        <th>Key</th>
        <th>Key description</th>
        <th>Value</th>
        <th>Value description</th>
    </tr>
    <tr>
        <td>string (e.g. <code>if</code>)</td>
        <td>Input match</td>
        <td>string (e.g. <code>T_IF</code>)</td>
        <td>New token name</td>
    </tr>
    <tr>
        <td colspan="4"><i>Note: The above row can be applied several times in this object</i></td>
    </tr>
</table>

## III - Lexical error messages
Error messages reported by error tokens can be customized to provide meaningful information for the user.

#### JSON structure
<b>Global key table</b>
<table>
    <tr>
        <th>Key</th>
        <th>Key description</th>
        <th>Value</th>
        <th>Value description</th>
        <th>Required</th>
    </tr>
    <tr>
        <td>default_message</td>
        <td>A default message, used if no specific message is defined for the error token.</td>
        <td>string</td>
        <td>General error message. The string can contain <a href="#optional-placeholders">optional 
        placeholders</a>.</td>
        <td>Yes</td>
    <tr>
        <td>error_messages</td>
        <td>Specific message for an error token</td>
        <td>Object of <a href="#error-message-object">error message</a></td>
        <td>Check table</td>
        <td>Yes</td>
    </tr>
</table>

<b id="error-message-object">Error message object table</b>
<table>
    <tr>
        <th>Key</th>
        <th>Description</th>
        <th>Value</th>
    </tr>
    <tr>
        <td>Token name (e.g. <code>T_INVALID_CHAR</code>)</td>
        <td>Print error message when the error token is generated.</td>
        <td>Specific error message. The string can contain <a href="#optional-placeholders">optional
        placeholders</a></td>
    </tr>
</table>

<b id="optional-placeholders">Placeholders table</b>
<table>
    <tr>
        <th>Placeholder</th>
        <th>Description</th>
    </tr>
    <tr>
        <td><code>${value}</code></td>
        <td>Input value of the lexical token.</td>
    </tr>
    <tr>
        <td><code>${column}</code></td>
        <td>Horizontal position of the lexical token.</td>
    </tr>
    <tr>
        <td><code>${line}</code></td>
        <td>Line number where the lexical token is found.</td>
    </tr>
    <tr>
        <td><code>${filename}</code></td>
        <td>Input file name.</td>
    </tr>
</table>
