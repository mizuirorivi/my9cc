#!/bin/bash
assert(){
    expected="$1"
    input="$2"

    ./mizuirocc "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else 
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

assert 21 "5+20-4"
assert 90 "80+20-10"
assert 41 "12 + 34 - 5 "
assert 47 '5+6*7'
assert 15 '5*(9-6)'
assert 4 '(3+5)/2'
assert 2 "(80+20-10)*2-100+20/2+2-90"
echo OK

