EasyCC C++
============

### Lexial analysis
The state machine is provided as a json file with the follwoing syntax:
```
{
  "states": [
    {
      "type": ...
      "id": ...
      "token": ...
      "backtrack": ...
    },
    ...
  ],
  "transitions": [
    {
      "from": ...
      "to": ...
      "chars": [
        ...
      ]
    },
    ...
  ]
}
```
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
    <td>Example: `T_INTEGER`, `T_ERROR_LEADING_ZERO`</td>
    <td>Must be defined for final states only. Otherwise it will be ignored.</td>
  </tr>
  <tr>
    <td>backtrack</td>
    <td>`true` or `false`</td>
    <td>Must be defined for final states only. Otherwise it will be ignored.</td>
  </tr>
  <tr>
    <td rowspan="3">transition</td>
    <td>from</td>
    <td>State id</td>
    <td>State id must exist</td>
  </tr>
  <tr>
    <td>to</td>
    <td>State id</td>
    <td>State id must exist</td>
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
    - `OTHER`: Any other character which no special path has been defined for it. This is often useful for tokens that need to backtrack on the final state and for detecting syntax errors.<br/>
    </td>
  </tr>
</table>
