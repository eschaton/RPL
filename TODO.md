# RPL To-Do List

This is the current to-do list for RPL.


## General

- Use ICU for to implement case-insensitivity.


## Interpreter

- Make identifiers case-insensitive.


## Value Types

- Switch `rpl_real_t` to use `long double` or even 128-bit double where
  feasible.
- Implement `rpl_list_t` in terms of `rpl_adjbuffer_t`.
- Implement `rpl_program_t` in terms of calls to the interpreter.
- Make `rpl_name_t` case-insensitive.


## Operations

- Add basic arithmetic operations
- Add basic trigonometric operations
- Add basic variable operations


## Tests

- Create a minimal test for each value type.
- Create a harness for interpreter tests.
