### Building the program
* Run cmake script: `./run_cmake.sh`
* Run make scripts: `./run_make_pro.sh` and `./run_make_dev.sh`

### Running the program
* Calculator - pro: `bin/calculator resources/src/input.txt -o /tmp/result.txt`
* Calculator - dev: `bin/calculatordev -s resources/src/lexical_state_machine.json -c resources/src/lexical_config.json -e resources/src/lexical_errors.json -g resources/src/syntax_grammar.json -E resources/src/syntax_errors.json resources/src/input.txt -o /tmp/result.txt -v `
