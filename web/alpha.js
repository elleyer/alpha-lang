//The ALPHA Esoteric Programming Language v0.2 web

"use strict";

//Global alpha running flag
let ALPHA_RUNNING = false;

//Read something from input
const conio_read = function() {
    return new Promise(function(resolve, reject) {
        $("#a_input").keydown(function(evt) {
            if(evt.keyCode == 13) {
                const value = $("#a_input").val();
                resolve(value);
            }
            return;
        });
    });
};

//Write something to output
const conio_write = function() {
    let text = "";
    for(let i = 0; i < arguments.length; i++) {
        text += (arguments[i]);
    }
    $("#a_console").val($("#a_console").val() + text);
    return;
};

//ALPHA interpteret
const alpha = async function(argv) {
    //Check syntax for errors.
    const _check = function(src) {
        let quote = 0;
        let comment = 0;
        let loop = 0;
        let cond = 0;
        
        for(let i = 0; i < src.length; i++) {
            switch(src[i]) {
                case '{':   if((quote % 2) == 0) comment++; break;
                case '}':   if((quote % 2) == 0) comment--; break;
                case '[':   if((quote % 2) == 0) loop++;    break;
                case ']':   if((quote % 2) == 0) loop--;    break;
                case '(':   if((quote % 2) == 0) cond++;    break;
                case ')':   if((quote % 2) == 0) cond--;    break;
                case '\"':  quote++;                        break;
            }
        }
        
        return (((quote % 2) == 0) && (comment == 0) && (loop == 0) && (cond == 0));
    };
    
    //C-analog of ctype isdigit(char) function
    const _isdigit = function(char) {
        return (char == '0' || char == '1' || char == '2' || char == '3' || char == '4' ||
                char == '5' || char == '6' || char == '7' || char == '8' || char == '9');
    }

    console.clear();

    //Flags and args from '/run'
    let debug = argv.includes("debug");
    let clear = argv.includes("clear");
    
    let src = $("#a_src").val();
    if(!_check(src)) {
        conio_write("Syntax error.\n");
        return;
    }
    
    ALPHA_RUNNING = true;
    if(clear) {
        $("#a_console").val("");
        if(debug) {
            console.log("Cleared console");
        }
    }
    
    let memory = [];
    let memptr = 0;
    let acc = 0;
    let pos = 0;
    
    for(let i = 0; i < 0x10000; i++) {
        memory[i] = 0;
    }
    
    while((pos < src.length) && (ALPHA_RUNNING)) {
        if(debug) {
            console.log("Reading '" + src[pos] + "' at " + pos);
        }
        
        //If there is comment
        if(src[pos] == '{') {
            while(src[++pos] != '}');
            pos++;
            continue;
        }
        
        //If there is string to print
        if(src[pos] == '\"') {
            while(src[++pos] != '\"') {
                conio_write(src[pos]);
            }
            pos++;
            continue;
        }
        
        //if there is number
        if(_isdigit(src[pos]) || (src[pos] == '-' && _isdigit(src[pos + 1]))) {
            let num = src[pos];
            while(_isdigit(src[++pos])) {
                num += src[pos];
            }
            num = Number(num);
            acc = (!isNaN(num)) ? num : 0;
        }
        
        //Put accumulator value to memory
        if(src[pos] == ':') {
            memory[memptr & 0xFFFF] = acc;
            pos++;
            continue;
        }
        
        //Put memory value to accumulator
        if(src[pos] == ';') {
            acc = memory[memptr & 0xFFFF];
            pos++;
            continue;
        }
        
        //Next memory cell
        if(src[pos] == '>') {
            memptr++;
            pos++;
            continue;
        }
        
        //Previous memory cell
        if(src[pos] == '<') {
            memptr--;
            pos++;
            continue;
        }
        
        //Accumulator++
        if(src[pos] == 'i') {
            acc++;
            pos++;
            continue;
        }
        
        //Accumulator--
        if(src[pos] == 'd') {
            acc--;
            pos++;
            continue;
        }
        
        //Accumulator = cell + accumulator
        if(src[pos] == '+') {
            acc = memory[memptr & 0xFFFF] + acc;
            pos++;
            continue;
        }
        
        //Accumulator = cell - accumulator
        if(src[pos] == '-') {
            acc = memory[memptr & 0xFFFF] - acc;
            pos++;
            continue;
        }
        
        //Accumulator = cell * accumulator
        if(src[pos] == '*') {
            acc = memory[memptr & 0xFFFF] * acc;
            pos++;
            continue;
        }
        
        //Accumulator = cell / accumulator
        if(src[pos] == '/') {
            acc = (acc != 0) ? (memory[memptr & 0xFFFF] / acc) : 0;
            pos++;
            continue;
        }
        
        //Accumulator = cell % accumulator
        if(src[pos] == '%') {
            acc = (acc != 0) ? (memory[memptr & 0xFFFF] % acc) : 0;
            pos++;
            continue;
        }
        
        //Accumulator = cell & accumulator
        if(src[pos] == '&') {
            acc = memory[memptr & 0xFFFF] & acc;
            pos++;
            continue;
        }
        
        //Accumulator = cell | accumulator
        if(src[pos] == '|') {
            acc = memory[memptr & 0xFFFF] | acc;
            pos++;
            continue;
        }
        
        //Accumulator = cell ^ accumulator
        if(src[pos] == '^') {
            acc = memory[memptr & 0xFFFF] ^ acc;
            pos++;
            continue;
        }
        
        //Accumulator = cell == accumulator
        if(src[pos] == '=') {
            acc = +(memory[memptr & 0xFFFF] == acc);
            pos++;
            continue;
        }
        
        //!accumulator
        if(src[pos] == '_') {
            acc = (acc != 0) ? 0 : 1;
            pos++;
            continue;
        }
        
        //print accumulator value as char
        if(src[pos] == '!') {
            conio_write(String.fromCharCode(acc));
            pos++;
            continue;
        }
        
        //Print accumulator value as number
        if(src[pos] == '.') {
            conio_write(acc);
            pos++;
            continue;
        }
        
        //Read accumulator value as char (single from whole string)
        if(src[pos] == '\'') {
            await conio_read().then(function(value) {
                acc = value.toString().charCodeAt(0);
            });
            conio_write(String.fromCharCode(acc), '\n');
            pos++;
            continue;
        }
        
        //Read accumulator value as number
        if(src[pos] == ',') {
            let num = Number(await conio_read());
            acc = (!isNaN(num)) ? num : 0;
            conio_write(acc, '\n');
            pos++;
            continue;
        }
        
        //While loop
        if(src[pos] == '[') {
            pos++;
            continue;
        }
        
        //While loop end
        if(src[pos] == ']') {
            if(acc != 0) {
                let l = 1;
                while(l > 0) {
                        pos--;
                        switch(src[pos]) {
                            case '[':   l--;    break;
                            case ']':   l++;    break;
                    }
                }
            }
            pos++;
            continue;
        }
        
        //If-block
        if(src[pos] == '(') {
            if(acc == 0) {
                let l = 1;
                while(l > 0) {
                        pos++;
                        switch(src[pos]) {
                        case ')':   l--;    break;
                        case '(':   l++;    break;
                    }
                }
            }
            pos++;
            continue;
        }
        
        //If-block end
        if(src[pos] == ')') {
            pos++;
            continue;
        }
        
        //Any space
        if(/\s/.test(src[pos])) {
            pos++;
            continue;
        }
        
        //Stop executing
        if(src[pos] == '$') {
            break;
        }
        
        conio_write('\n', "Invalid char: ", src[pos], '\n');
        break;
    }
    
    ALPHA_RUNNING = false;
    conio_write("\nFinished.\n");
    return;
};

//Parse input
const alpha_cmdparse = async function(value) {
    if(value.startsWith('/')) {
        value = value.substring(1);
        value = value.split(/\s/);
        let command = value.shift();
        
        switch(command.toLowerCase()) {
            case "clear":   $("#a_console").val("");                                        break;
            case "run":     alpha(value);                                                   break;
            case "github":  window.open("https://github.com/undbsd/alpha-lang", "_blank");  break;
            case "restart": window.location.reload(false);                                  break;
            case "echo":    conio_write(value.join(' '), '\n');                             break;
            default:        conio_write("Invalid command\n");                               break;
        }
    }
};

//Global escape keypress
$(document).keydown(function(evt) {
    if (evt.keyCode == 27) {
        if(ALPHA_RUNNING) {
            conio_write("\nEscape press detected. Alpha stopped...\n");
        }
        ALPHA_RUNNING = false;
    }
    return;
});

//Main function that do anything you want.
(async function() {
    conio_write("The ALPHA Esoteric Programming Language v0.2W\n");
    conio_write("============================================\n\n");
    conio_write("Write '/run' to execute the code.\n");
    conio_write("Press enter to submit your input\n");
    
    await (async function loop() {
        let thing = await conio_read();
        $("#a_input").val("");
        await alpha_cmdparse(thing);
        setTimeout(loop, 100);
    })();
    
})();
