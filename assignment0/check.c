//
// Created by alanfl on 9/17/19.
//

// !!! CLARIFICATION NEEDED !!!
/*
 * 0. Are single operand expressions legal?
 *      E.g. "1" or "true"
 * 1. How many operators are permitted in a legal expression?
 *      E.g. "1 + 2 + 3" or "true AND false AND true"
 *      Answer: only ONE operator per expression
 * 2. Are empty expressions still legal?
 * 3. Is a NOT following an operator legal?
 *      E.g. "true AND NOT false"
 *      Answer: No, logical NOT is unary-exclusive and cannot be nested
 */

//=== REQUIREMENTS ===

/*
 * 0.  Identification
 * 0.a Find all tokens
 * 0.b Find all expressions
 * 0.c Find all operators
 * 0.d Find all operands
 */

/*
 * 1.  Computation
 * 1.a Validate input operand
 * 1.b Validate input operator
 * 1.c Extract next expression from input line
 * 1.d Extract next token from input expression
 */

/*
 * 2.      Evaluation
 * 2.a     Fetch input
 * 2.b     Feed input to 1.c
 * 2.b.0   Feed expression to 1.d
 * 2.b.(1) While tokens are still available
 * 2.b.1   Feed token to 1.a
 *             Should be first operand or NOT unary
 *             Otherwise, output error
 * 2.b.2   Feed next token to 1.b
 *             Should be operator or operand for NOT unary
 *             Otherwise, output error
 * 2.b.3   Feed next token to 1.a
 *             If not NOT unary, should be operand
 *             Otherwise, output error
 * 2.b.4   Feed next token to 1.a
 *             Last token must be operand and is only read if previous token was NOT
 */

//=== NOTES ===

/*
 * Compile Notes
 * 1. Compile only with gcc
 * 2. Do not use any other flags other than -o
 */

/*
 * Other Notes
 * 0. A parse error should not terminate the program
 * 1. Errors should always be printed to the console
 * 2. Handle all errors, do not terminate the program until all input has been read
 * 3. Do not use sort(), strtok(), or anything from the ctype.h or string.h libraries.
 */

#include <stdio.h>
#include <stdlib.h>

enum expression_type{arithmetic, logical};

// -1. Data Structures
typedef struct Expression {
    int type;
    int num_tokens;
    char* string;
    char** tokens;
} Expression;

// 0. Identification

/*
 * 0.a Find all tokens
 *
 * Takes an input string and a char array of delimiters and replaces all instances of
 * any characters that are also present in the delimiter array with a NULL terminator.
 */
char* tokenize(char* str, char delim);

/*
 * 0.b Find all expressions
 * Takes an input string, and calls tokenize with a ';' delimiter.
 * Then populates an Expression array and returns a pointer to it.
 */
Expression* split_to_expression(char* str);

/*
 * 0.b Utility
 * Counts the number of expressions in a provided string for usage in bounds checking.
 */
int count_expressions(char* str, int length);

int string_length(char* str);

int main(int argc, char** argv) {
    char* str = argv[1];
    int str_len = string_length(str);
    int num_expressions = count_expressions(str, str_len);
    Expression* arr = split_to_expression(str);
    for(int i = 0; i < num_expressions; i++) {
        printf("%s\n", arr[i].string);
    }
}

// 0.a
char* tokenize(char* str, char delim) {
    while(*str != '\0') {
        if (*str == delim) {
            *str = '\0';
        }
        str++;
    }

    return str;
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

Expression* split_to_expression(char* str) {
    int str_len = string_length(str);
    int expressions = 0;
    expressions = count_expressions(str, str_len);
    char* new_str = tokenize(str, ';');


    // Count the number of expressions
//    for(int i = 0; i < str_len; i++) {
//        if(new_str[i] == '\0' && new_str[i-1] != '\0') {
//            expressions++;
//        }
//    }


    // Allocate specific indexes
    Expression* expression_array = malloc(expressions * sizeof(*expression_array));

    // No space was allocated, no expression could be parsed
    if(!expression_array) {
        return NULL;
    }

    // Populate the Expression array
    int trailing_ptr = 0;
    int index = 0;

    printf("%s", new_str);

    for(int i = 0; i < str_len; i++) {
        if(str[i] == '\0' && str[i-1] != '\0') {
            expression_array[index].string = malloc(sizeof(char *));
            expression_array[index].string = &new_str[trailing_ptr];
            index++;
            trailing_ptr = i+1;
        }
    }

    return expression_array;
}

int count_expressions(char* str, int length) {
    int expressions = 0;
    for(int i = 0; i < length; i++) {
        if((str[i] == ';' && str[i-1] != '\0') ||
           (str[i] == '\0' && str[i-1] != '\0')) {
            expressions++;
        }
    }
    return expressions;
}
