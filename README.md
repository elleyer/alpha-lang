# The ALPHA Esoteric programming language
## Hello world
Simple as heck
```
"Hello world!"10!$
```

## Syntax
#### Strings
To print string to screen just write it like that:
```
"This is a string"
```

So you can put newlines in the string like that:
```
"The quick brown fox
jumps over the lazy dog"
```

#### Accumulator
ALPHA machine have 32-bit stack with 65536 cells and 32-bit accumulator.  
To set value to accumulator just write it:
```
{something other ...}12345{... something other...}
```

To increase or decrease accumulator value do this:
```
] {increases accumulator by 1}
[ {decreases accumulator by 1}
```

To push value from accumulator to stack:
```
: {Pushes value to stack}
```

To pop value from stack to accumulator:
```
; {Pops value from stack}
```

#### Stack calculating
ALPHA can add, subtract, multiple, divide, remainder divide, do bitwise and logical operations.
```
+-*/%&|^~>< {All stack arithmetical and logical operations}
```

#### IO
Printing:
```
10! {Print newline}
10. {Print 10}
```

Reading:
```
' {getchar}
, {scanf %d}
```

#### Conditional jumping and looping
To create jump label just write `@`  
```
255:255:=_?"I shall not appear!"@"I shall appear!"
{if 255 == 255 then jump to next standing label}

```

```
@"What recursion?"10!1#
{If 1 > 0 then jump to previous standing label}
```

#### Equals and not
Equals `=` operator pops two elements from stack and sets accumulator to 1 if they are equal, else sets it to zero. Then pushes elements back.  
The not `_` operator just doing accumulator = !accumulator

## Examples
#### Print ASCII (8-bit) table
```
@;;!]:255:=_#$
```

#### Print something with new line 10 times
```
@;;"Something printed "." times":10!;]:10:=_#$
```

#### Cat (Copy STDIN to STDOUT)
```
@'!:0xFFFFFFFF:=_#
```
