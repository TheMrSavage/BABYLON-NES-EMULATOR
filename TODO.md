# TODO
Just a list of stuff to do to make things simpler and more organized

## Workflow/"Clean" code 

- [x] Create an "debugger/dev" option in `CMake` workflow with debugging flags (like [those](https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html)), to make easier to debug.
- [ ] Replace *all* possible raw pointers to smart one's
- [ ] Integrate `Valgrind` with `CMake` workflow
- [x] Put `AddressSanitizer` and `UndefinedBehaviorSanitizer` flags in `CMake` workflow
- [ ] Refactor the test suite for use TDD properly
- [ ] Create Windows compilation workflow
- [ ] Write documentation for *each* emulator component
- [ ] Write a proper `README`
- [ ] Write an CI/CD for each relevant build option (with tests!)

## Features

### Basic

- [x] Implement 6502 core
- [ ] Implement BUS logic
- [ ] Implement PPU
- [ ] Implement APU
- [ ] Implement Mappers

### Others

- [ ] Implement save-state logic
- [ ] Implement joystick compatibility
