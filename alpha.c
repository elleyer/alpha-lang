//The ALPHA Programming Language

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define ALPHA_NEXT(P) P++; continue
#define ALPHA_STACK 0x10000
#define ALPHA_MASK  (ALPHA_STACK - 1)

//Check source code for errors (unclosed quotes, braces and other
int checks(char *src)
{
    //Data
    char *p = src;
    int quote = 0;
    int comment = 0;
    int loop = 0;
    int cond = 0;

    while(*p) {
        switch(*p) {
            case '{':   if((quote % 2) == 0) comment++; break;
            case '}':   if((quote % 2) == 0) comment--; break;
            case '[':   if((quote % 2) == 0) loop++;    break;
            case ']':   if((quote % 2) == 0) loop--;    break;
            case '(':   if((quote % 2) == 0) cond++;    break;
            case ')':   if((quote % 2) == 0) cond--;    break;
            case '\"':  quote++;                        break;
        }

        p++;
    }

    return (((quote % 2) == 0) && (comment == 0) && (loop == 0) && (cond == 0));
}

//Do interpretation
//ALPHA have stack and accumulator
void alpha(char *src)
{
    //Detect invalid syntax
    if(!checks(src)) {
        printf("Invalid syntax.\n");
        return;
    }

    //Data
    char *p = src;
    long int ptr = 0, acc = 0;
    long int mem[ALPHA_STACK] = { 0 };

    while(*p) {
        //If there is a comment
        if(*p == '{') {
            while(*(++p) != '}');
            ALPHA_NEXT(p);
        }

        //If there is a string
        if(*p == '\"') {
            while(*(++p) != '\"') {
                putchar(*p);
            }
            ALPHA_NEXT(p);
        }

        //If there is a number
        if(isdigit(*p) || ((*p == '-') && (isdigit(*(p + 1))))) {
            acc = strtol(p, &p, 0);
            continue;
        }

        //Memory-Accumulator instructions
        //Now ALPHA have not stack but can add value to accumulator from tape, etc...
        //Accumulator->Cell
        if(*p == ':') {
            mem[ptr & ALPHA_MASK] = acc;
            ALPHA_NEXT(p);
        }

        //Cell->Accumulator
        if(*p == ';') {
            acc = mem[ptr & ALPHA_MASK];
            ALPHA_NEXT(p);
        }

        //Next cell
        if(*p == '>') {
            ptr++;
            ALPHA_NEXT(p);
        }

        //Previous cell
        if(*p == '<') {
            ptr--;
            ALPHA_NEXT(p);
        }

        //Accumulator++
        if(tolower(*p) == 'i') {
            acc++;
            ALPHA_NEXT(p);
        }

        //Accumulator--
        if(tolower(*p) == 'd') {
            acc--;
            ALPHA_NEXT(p);
        }

        //Accumulator = cell + accumulator
        if(*p == '+') {
            acc = mem[ptr & ALPHA_MASK] + acc;
            ALPHA_NEXT(p);
        }

        //Accumulator = cell - accumulator
        if(*p == '-') {
            acc = mem[ptr & ALPHA_MASK] - acc;
            ALPHA_NEXT(p);
        }

        //Accumulator = cell * accumulator
        if(*p == '*') {
            acc = mem[ptr & ALPHA_MASK] * acc;
            ALPHA_NEXT(p);
        }

        //Accumulator = cell / accumulator
        if(*p == '/') {
            acc = (acc) ? (mem[ptr & ALPHA_MASK] / acc) : 0;
            ALPHA_NEXT(p);
        }

        //Accumulator = cell % accumulator
        if(*p == '%') {
            acc = (acc) ? (mem[ptr & ALPHA_MASK] % acc) : 0;
            ALPHA_NEXT(p);
        }

        //Accumulator = cell & accumulator
        if(*p == '&') {
            acc = mem[ptr & ALPHA_MASK] & acc;
            ALPHA_NEXT(p);
        }

        //Accumulator = cell | accumulator
        if(*p == '|') {
            acc = mem[ptr & ALPHA_MASK] | acc;
            ALPHA_NEXT(p);
        }

        //Accumulator = cell ^ accumulator
        if(*p == '^') {
            acc = mem[ptr & ALPHA_MASK] ^ acc;
            ALPHA_NEXT(p);
        }

        //Accumulator = (Accumulator == cell)
        if(*p == '=') {
            acc = (acc == mem[ptr & ALPHA_MASK]) & 1;
            ALPHA_NEXT(p);
        }

        //!Accumulator
        if(*p == '_') {
            acc = !acc;
            ALPHA_NEXT(p);
        }

        //Print accumulator value as char
        if(*p == '!') {
            putchar(acc);
            ALPHA_NEXT(p);
        }

        //Print accumulator value as number
        if(*p == '.') {
            printf("%ld", acc);
            ALPHA_NEXT(p);
        }

        //Read accumulator value as char
        if(*p == '\'') {
            acc = getchar();
            ALPHA_NEXT(p);
        }

        //Read accumulator value as number
        if(*p == ',') {
            scanf("%ld", &acc);
            ALPHA_NEXT(p);
        }

        //Loop start - do nothing.
        if(*p == '[') {
            ALPHA_NEXT(p);
        }

        //Loop end
        if(*p == ']') {
            if(acc) {
                int l = 1;
                while((l > 0) && (*(p - 1))) {
                    p--;
                    switch(*p) {
                        case '[':   l--;    break;
                        case ']':   l++;    break;
                    }
                }
            }

            ALPHA_NEXT(p);
        }

        //Conditional start
        if(*p == '(') {
            if(!acc) {
                int l = 1;
                while((l > 0) && (*(p + 1))) {
                    p++;
                    switch(*p) {
                        case ')':   l--;    break;
                        case '(':   l++;    break;
                    }
                }
            }

            ALPHA_NEXT(p);
        }

        //Conditional end
        if(*p == ')') {
            ALPHA_NEXT(p);
        }

        //Stop interptering
        if(*p == '$') {
            break;
        }

        //All whitespace are ignored
        if(isspace(*p)) {
            ALPHA_NEXT(p);
        }

        printf("\nUnknown char: '%c'\n", *p);
        return;
    }

    putchar('\n');
    return;
}

int main(int argc, char **argv)
{
    //If not enough args
    if(argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    //Print message
    printf("The ALPHA Esoteric Programming Language v0.2\n");
    printf("============================================\n\n");

    //Do simple parsing
    FILE *f;
    int length = 0;
    char *buffer = NULL;

    //Open file and check for errors
    f = fopen(argv[1], "rb");
    if(!f) {
        perror(argv[1]);
        return 1;
    }

    //Get length of file
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    rewind(f);

    //Create new buffer
    buffer = malloc(length + 1);
    memset(buffer, 0, length);

    //Read file to buffer
    fread(buffer, 1, length, f);
    buffer[length] = 0;
    fclose(f);

    //Run program
    printf(">>> %s\n", buffer);
    alpha(buffer);
    free(buffer);

    return 0;
}
