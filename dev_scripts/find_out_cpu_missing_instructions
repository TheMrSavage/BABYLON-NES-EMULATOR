#!/bin/bash

MISSING="$(cat -n ../src/libs/nes/rp2a03/cpu/Cpu_6502.cpp | grep -E "void CPU::[a-Z]+\((([a-Z|0-9|_]+ ([a-Z]|0-9|_)+)|)\)\{\}")"

echo "Missing instructions ($(echo "$MISSING" | wc -l)):"
echo "$MISSING"
