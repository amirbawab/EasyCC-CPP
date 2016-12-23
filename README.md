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
  "edges": [
    {
      "from": ...
      "to": ...
      "chars": [
        ...
      ]
    }
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
    <td>edge</td>
  </tr>
</table>
