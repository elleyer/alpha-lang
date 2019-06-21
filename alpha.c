//The ALPHA Programming Language

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

//Do interpretation
//ALPHA have stack and accumulator
void alpha(char *src)
{
    //Data
    uint16_t sptr = 0;
    uint32_t stack[0x10000] = { 0 };
    uint16_t accumulator = 0;

    char *p = src;
    while(*p) {
        //If there is comment then skip
        if(*p == '{') {
            while(*(++p) != '}') {
                //If user entered wrong things then print error message and quit
                if(!*p) {
                    fprintf(stderr, "\nUNEXPECTED EOF: Cannot find end of comment\n");
                    exit(1);
                }
            }
            p++;
            continue;
        }

        //Any string -> print it
        if(*p == '\"') {
            while(*(++p) != '\"') {
                //If user entered wrong things then print error message and quit
                if(!*p) {
                    fprintf(stderr, "\nUNEXPECTED EOF: Cannot find end of string\n");
                    exit(1);
                }

                putchar(*p);
            }
            p++;
            continue;
        }

        //Directly put number to accumulator
        if(isdigit(*p)) {
            accumulator = strtol(p, &p, 0);
            continue;
        }

        //Increment accumulator
        if(*p == ']') {
            accumulator++;
            p++;
            continue;
        }

        //Decrement accumulator
        if(*p == '[') {
            accumulator++;
            p++;
            continue;
        }

        //Push value from accumulator to stack
        if(*p == ':') {
            stack[sptr++] = accumulator;
            p++;
            continue;
        }

        //Pop value from stack to accumulator
        if(*p == ';') {
            accumulator = stack[--sptr];
            stack[sptr] = 0;
            p++;
            continue;
        }

        //pop pop add push
        if(*p == '+') {
            uint16_t b = stack[--sptr]; stack[sptr] = 0;
            uint16_t a = stack[--sptr]; stack[sptr] = 0;
            stack[sptr++] = (a + b);
            p++;
            continue;
        }

        //pop pop sub push
        if(*p == '-') {
            uint16_t b = stack[--sptr]; stack[sptr] = 0;
            uint16_t a = stack[--sptr]; stack[sptr] = 0;
            stack[sptr++] = (a + b);
            p++;
            continue;
        }

        //pop pop mul push
        if(*p == '*') {
            uint16_t b = stack[--sptr]; stack[sptr] = 0;
            uint16_t a = stack[--sptr]; stack[sptr] = 0;
            stack[sptr++] = (a * b);
            p++;
            continue;
        }

        //pop pop div push
        if(*p == '/') {
            uint16_t b = stack[--sptr]; stack[sptr] = 0;
            uint16_t a = stack[--sptr]; stack[sptr] = 0;
            stack[sptr++] = (b) ? (a / b) : 0;
            p++;
            continue;
        }

        //pop pop mod push
        if(*p == '%') {
            uint16_t b = stack[--sptr]; stack[sptr] = 0;
            uint16_t a = stack[--sptr]; stack[sptr] = 0;
            stack[sptr++] = (b) ? (a % b) : 0;
            p++;
            continue;
        }

        //pop pop and push
        if(*p == '&') {
            uint16_t b = stack[--sptr]; stack[sptr] = 0;
            uint16_t a = stack[--sptr]; stack[sptr] = 0;
            stack[sptr++] = (a & b);
            p++;
            continue;
        }

        //pop pop or push
        if(*p == '|') {
            uint16_t b = stack[--sptr]; stack[sptr] = 0;
            uint16_t a = stack[--sptr]; stack[sptr] = 0;
            stack[sptr++] = (a | b);
            p++;
            continue;
        }

        //pop pop xor push
        if(*p == '^') {
            uint16_t b = stack[--sptr]; stack[sptr] = 0;
            uint16_t a = stack[--sptr]; stack[sptr] = 0;
            stack[sptr++] = (a ^ b);
            p++;
            continue;
        }

        //pop not push
        if(*p == '~') {
            uint16_t a = stack[--sptr]; stack[sptr] = 0;
            stack[sptr++] = ~a;
            p++;
            continue;
        }

        //pop pop shl push
        if(*p == '<') {
            uint16_t b = stack[--sptr]; stack[sptr] = 0;
            uint16_t a = stack[--sptr]; stack[sptr] = 0;
            stack[sptr++] = (a << b);
            p++;
            continue;
        }

        //pop pop shr push
        if(*p == '>') {
            uint16_t b = stack[--sptr]; stack[sptr] = 0;
            uint16_t a = stack[--sptr]; stack[sptr] = 0;
            stack[sptr++] = (a >> b);
            p++;
            continue;
        }

        //print accumulator as char
        if(*p == '!') {
            putchar(accumulator);
            p++;
            continue;
        }

        //print accumulator as number
        if(*p == '.') {
            printf("%u", accumulator);
            p++;
            continue;
        }

        //read accumulator as char
        if(*p == '\'') {
            accumulator = getchar();
            p++;
            continue;
        }

        //read accumulator as number
        if(*p == ',') {
            scanf("%d", &accumulator);
            p++;
            continue;
        }

        //Conditional label (useless but here!)
        if(*p == '@') {
            p++;
            continue;
        }

        //if accumulator is not zero then jump to previuos label
        if(*p == '#') {
            if(accumulator) {
                while(*(--p) != '@') {
                    //If user entered wrong things then print error message and quit
                    if(!*p) {
                        fprintf(stderr, "\nUNEXPECTED EOF: Cannot find previous label\n");
                        exit(1);
                    }
                }
            }
            p++;
            continue;
        }

        //if accumulator is zero then jump to next label
        if(*p == '?') {
            if(!accumulator) {
                while(*(++p) != '@') {
                    //If user entered wrong things then print error message and quit
                    if(!*p) {
                        fprintf(stderr, "\nUNEXPECTED EOF: Cannot find next label\n");
                        exit(1);
                    }
                }
            }
            p++;
            continue;
        }

        //set accumulator to 1 if pop == pop, else set to zero
        if(*p == '=') {
            uint16_t b = stack[--sptr];
            uint16_t a = stack[--sptr];
            accumulator = ((a == b) & 1);
            sptr++;
            sptr++;
            p++;
            continue;
        }

        //BOOLEAN inverse accumulator
        if(*p == '_') {
            accumulator = !accumulator;
            p++;
            continue;
        }

        //Stop interptereter right now
        if(*p == '$') {
            break;
        }

        p++;
    }

    printf("\nFinished.\n");
    return;
}

int main(int argc, char **argv)
{
    //If no args then crash
    if(argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    //Open file to read
    FILE *f = fopen(argv[1], "rb");
    if(!f) {
        perror(argv[1]);
        return -1;
    }

    //Get length of file
    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    rewind(f);

    //Read data to buffer then close file
    char *buffer = malloc(len);
    fread(buffer, 1, len, f);
    fclose(f);

    //Replace all terminators to spaces
    for(int i = 0; i < len; i++) {
        if(!buffer[i]) {
            buffer[i] = ' ';
        }
    }

    alpha(buffer);

    return 0;
}
