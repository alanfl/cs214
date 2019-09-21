//
// Created by alanfl on 9/19/19.
//

#include <stdio.h>
#include <stdlib.h>

enum valid{no = 0, yes = 1};

int validate_logical_expression(char *, int start, int end);
int validate_arithmetic_expression(char *, int start, int end);

int validate_operand(char *);

int validate_logical_operand(char *);
int validate_arithmetic_operand(char);

int validate_operator(char *);

int validate_logical_operator(char *);
int validate_unary_operator(char *);
int validate_arithmetic_operator(char);

int get_expression_end(char *);

int string_length(char *);

int main(int argc, char** argv) {
    if(argc > 2) {
        printf("%s", "Error: Too many arguments.");
        return 0;
    }

    char* str = argv[1];
    int str_len = string_length(str);

    // Iterate the entire statement
    for(int i = 0; i < str_len; i++) {
        // Get bounds for current expression
        int state = 0;
        int start = i;
        int end = get_expression_end(str);

        if(validate_logical_expression(str, start, end)) {
            printf("%s")
        }
    }
}

int string_length(char* str) {
    int length = 0;
    while(*str != '\0') {
        length++;
        str++;
    }

    // Include NULL terminator
    return length + 1;
}

int get_expression_end(char * str) {
    int index = 0;
    while(*str != '\0') {
        index++;
        if(*str == ';') {
            return index;
        }
        str++;
    }

    return index;
}