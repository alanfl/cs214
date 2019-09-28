//
// Created by alanfl on 9/19/19.
//
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char* data;
    struct Node* next;
} Node;

// Linked list and Node functions
void print_list(Node* head);
Node* add_to_list(Node* head, char* data);

// String functions
int string_compare(const char* first, const char* second);

// Expression and token specific functions
Node* tokenize(const char* str, char delim);
int validate_expression(char* expression_str, Node* error, int expression_index);
int get_token_type(Node* token);
Node* add_token(Node *head, const char *str, int start_index, int end_index);

// Error stuff
char* print_error(char* error_msg, int index);
char* print_token(char* token_data);

// Freeing blocks
void free_list(Node* head);

int main(int argc, char** argv) {
    if(argc == 1) {
        printf("%s\n", "Error: No argument.");
        return 0;
    }

    if(argc > 2) {
        printf("%s\n", "Error: Too many arguments.");
        return 0;
    }

    char* str = argv[1];
    Node* head = tokenize(str, ';');
    Node* list = head;
    Node* error_list = add_to_list(NULL, NULL);
    int arithmetic_count = 0;
    int logical_count = 0;
    int expression_total = 0;

    while(head != NULL) {
        int expression_type = validate_expression(head->data, error_list, expression_total);
        // Expression was validated, and is arithmetic
        if(expression_type == 0) {
            arithmetic_count++;
            expression_total++;
        }
        // Expression was validated, and is logical
        else if (expression_type == 1) {
            logical_count++;
            expression_total++;
        }
        // Expression was validated, but it was empty/unidentifiable
        else if (expression_type == -1) {
            expression_total++;
        }

        // Now expecting a terminator, either ";" or NULL
        head = head->next;
    }

    printf("Found %d expression(s): %d arithmetic, %d logical.\n", expression_total, arithmetic_count, logical_count);
    if(error_list == NULL || error_list->next == NULL) {
        printf("OK\n");
    } else {
        print_list(error_list);
    }

    free_list(list);
    free_list(error_list);
    return 0;
}

/*
 * Usage: Accepts a string and a delimiter character to tokenize that string
 * and returns the head of a linked list containing all the tokens.
 *
 * Note: the space delimiter (" ") is added as a token to the list if there is another space separating it from the next token
 */
Node* tokenize(const char* str, char delim) {
    Node *head = NULL;

    int current_index = 0;
    int start_index = 0;

    if (delim == ' ' && str[current_index] != '\0')
        current_index = 1;

    while(str[current_index] != '\0') {
        if( str[current_index] == delim ) {
            // Create a token for the string from start_index to current_index exclusive
            head = add_token(head, str, start_index, current_index);

            // Move to next character after delimiter
            // Also push the start_index marker up as well
            current_index++;
            start_index = current_index;

            // Note (special case)
            // Since a ' ' immediately after another ' ' is considered valid content,
            // begin looking for delims AFTER that first whitespace
            if (delim == ' ' && str[start_index] != 0) current_index++;
        } else
            current_index++;
    }

    // Append the "last" token
    head = add_token(head, str, start_index, current_index);
    return head;
}

/*
 * Accepts a string and validates whether or not its resultant tokens are
 * a legal expression
 *  *
 * Also accepts a list of errors to append errors to.
 *
 * Returns:
 * -1 = Completely invalid, no identifiable type
 * 0 = Arithmetic type
 * 1 = Logical type
 */
int validate_expression(char* expression_str, Node* error, int expression_index) {

    // This will store the tokenized expression string
    Node* head = NULL;

    // This will store the beginning of the expression linked list for free() purposes
    Node* list = NULL;

    // Track what kind of expression we have
    int expression_type = -1;

    // Need to track when the expression has been completed
    int expression_complete = 0;

    // Also need to track if the error for an unterminated expression has already
    // been added to the buffer
    int expression_error_added = 0;

    // Track the next expected token, for iteration reasons
    int expected_token_type = -3;

    // If this isn't the first expression we're parsing, we need to first check for a leading whitespace
    if (expression_index != 0) {

        // If the leading whitespace is present, throw no error and adjust the string so it is
        // excluded from tokenizing
        if(expression_str != NULL && expression_str[0] == ' ') {
            list = head = tokenize(expression_str+1, ' ');
        }

        // Leading whitespace is not present, so whatever comes next will immediately be unexpected
        else {
            list = head = tokenize(expression_str, ' ');
            expected_token_type = -4;
        }
    } else {
        head = tokenize(expression_str, ' ');
        list = head;
    }

    // UPDATE: The first legal OPERATOR determines the expression type
    // Iterate tokens until out of tokens
    // NOTE: An expression's type is determined by the first legal token that appears in the expression
    while (head != NULL) {
        int token_type = get_token_type(head);

        // If the expression is considered complete, wasn't terminated, and the error was not added
        // Add the error, and track that the error was added.
        if (expression_complete == 1 && expression_error_added != 1 && token_type != -1) {
            add_to_list(error, print_error("Error: Scan error in expression %d: Expression was not ended.", expression_index));
            expression_error_added = 1;
        }

        // == Legal Cases ===
        // 1. Arithmetic operand
        if(token_type == 1) {
            // Not expecting any type of token, this is the first token
            if (expected_token_type == -3 && expression_complete != 1) {
//                printf("Found %s, OK\n", head->data); // todo remove debug
            }
            // This token was NOT expected
            else if (expected_token_type != 1) {
                // Not expected because there was a type mismatch
                if(expression_type == 1) {
                    add_to_list(error, print_error("Error: Parse error in expression %d: Type mismatch.", expression_index));
                    add_to_list(error, print_token(head->data));
                }
                // Not expected for any other reason
                // UPDATE: exclude from cases where some kind of operand was expected, but a type was not set, so we cannot infer the type
                else if (expected_token_type == -4 || expected_token_type == 3 || expected_token_type == 4 || (expression_type != -1 && expected_token_type == 2)){
                    add_to_list(error, print_error("Error: Parse error in expression %d: Unexpected operand", expression_index));
                    add_to_list(error, print_token(head->data));
                }
            }
            // If the expected token was an operand of any sort, then the pattern is finished
            if (expected_token_type == 1 || expected_token_type == 2) {
                expression_complete = 1;

//                printf("Found %s, OK\n", head->data); // todo remove debug

                // We also then expect the next token to be terminating
                expected_token_type = -4;
            }
            // Otherwise next token should be an arithmetic operator
            else {
                expected_token_type = 3;
            }

            head = head->next;
        }

        // 2. Arithmetic operator
        else if (token_type == 3) {
            // Not expecting any type of token so this is the first token and is therefore invalid
            if (expected_token_type == -3 && expression_complete != 1) {
                add_to_list(error, print_error("Error: Parse error in expression %d: Unexpected operator.", expression_index));
                add_to_list(error, print_token(head->data));
            }
            // This token is NOT expected
            else if (expected_token_type != 3) {
                // Not expected, type mismatch
                if(expected_token_type == 2) {
                    add_to_list(error, print_error("Error: Parse error in expression %d: Type mismatch.", expression_index));
                    add_to_list(error, print_token(head->data));
                }
                // Not expected for any other reason
                else {
                    add_to_list(error, print_error("Error: Parse error in expression %d: Unexpected operator.", expression_index));
                    add_to_list(error, print_token(head->data));
                }
            }
            // This token was expected, no issue
            else {
//                printf("Found %s, OK\n", head->data); // todo remove debug
            }

            // This sets the type of expression only if expression has no type
            if (expression_type == -1)
                expression_type = 0;

            // No matter what errors are thrown, this token predicts the next
            expected_token_type = 1;
            head = head->next;
        }

        // 3. Logical operand
        else if (token_type == 2) {
            // Not expecting any kind of token, this is the first and predicts the next
            if(expected_token_type == -3 && expression_complete != 1) {
//                printf("Found %s, OK\n", head->data); // todo remove debug
            }
            // This token is NOT expected
            else if (expected_token_type != 2) {
                // Not expected because Type mismatch
                if(expression_type == 0) {
                    add_to_list(error, print_error("Error: Parse error in expression %d: Type mismatch.", expression_index));
                    add_to_list(error, print_token(head->data));
                }
                // Not expected for any other reason
                // UPDATE: exclude from cases where some kind of operand was expected, but a type was not set, so we cannot infer the type
                else if(expected_token_type == -4 || expected_token_type == 3 || expected_token_type == 4 || (expression_type != -1 && expected_token_type == 1)){
                    add_to_list(error, print_error("Error: Parse error in expression %d: Unexpected operand", expression_index));
                    add_to_list(error, print_token(head->data));
                }
            }

            // If the expected token was an operand of any sort, then the pattern is finished
            if (expected_token_type == 1 || expected_token_type == 2) {
                expression_complete = 1;

//                printf("Found %s, OK\n", head->data); // todo remove debug

                // We also then expect the next token to be terminating
                expected_token_type = -4;
            }

            // Otherwise, next token should be a logical operator
            else
                expected_token_type = 4;

            head = head->next;
        }

        // 4. Logical operator
        else if (token_type == 4) {
            // Not expecting any kind of token, this is the first token and is therefore invalid
            if (expected_token_type == -3 && expression_complete != 1) {
                add_to_list(error, print_error("Error: Parse error in expression %d: Unexpected operator.", expression_index));
                add_to_list(error, print_token(head->data));
            }
            // Not expecting this token
            else if (expected_token_type != 4) {
                // Type mismatch
                if(expected_token_type == 1) {
                    add_to_list(error, print_error("Error: Parse error in expression %d: Type mismatch.", expression_index));
                    add_to_list(error, print_token(head->data));
                }
                // Not expected for any other reason
                else {
                    add_to_list(error, print_error("Error: Parse error in expression %d: Unexpected operator.", expression_index));
                    add_to_list(error, print_token(head->data));
                }
            }
            // This token was expected, throw no issue
            else {
//                printf("Found %s, OK\n", head->data); // todo remove debug
            }

            // Set type only if expression has no type
            if (expression_type == -1)
                expression_type = 1;

            // No matter what errors are thrown, this token predicts the next
            expected_token_type = 2;
            head = head->next;
        }
        // 5. Unary NOT operator
        else if (token_type == 5) {
            // Not expecting any type of token, this is first token and characterizes the expression
            // Next token must also be a logical operand
            if (expected_token_type == -3 && expression_complete != 1) {
//                printf("Found %s, OK\n", head->data); // todo remove debug
            }

            // Missing whitespace, this is immediately unexpected
            else if (expected_token_type == -4) {
                add_to_list(error, print_error("Error: Parse error in expression %d: Unexpected operator.", expression_index));
                add_to_list(error, print_token(head->data));
            }
            // Note: this MUST be the first token in the pattern, so all subsequent encounters are unexpected
            else {
                // Expression has ended, this is unexpected
                if(expression_complete == 1) {
                    add_to_list(error, print_error("Error: Parse error in expression %d: Unexpected operator.", expression_index));
                    add_to_list(error, print_token(head->data));
                }
            }

            // Unary operator immediately sets the expression type ONLY IF no expression type is set
            if (expression_type == -1)
                expression_type = 1;

            // Regardless of any errors, the next token should be a logical operand
            expected_token_type = 2;
            head = head->next;
        }

        // === Illegal Cases ===
        // 6. Unknown identifier
        // Any token we cannot readily identify is immediately returned as an unknown
        // We can determine what the unknown should have been based on context
        // We can then also determine the next identified output
        else if (token_type == -2) {
            // No context, unknown identifier error is added
            if (expected_token_type == -3) {
                add_to_list(error, print_error("Error: Parse error in expression %d: Unknown identifier.", expression_index));
                add_to_list(error, print_token(head->data));
            }

            // Expected some kind of operand, this is an unknown operand
            // We can assume the pattern is now complete because an operand was expected
            else if (expected_token_type == 1 || expected_token_type == 2) {
                add_to_list(error, print_error("Error: Parse error in expression %d: Unknown operand.", expression_index));
                add_to_list(error, print_token(head->data));

                expression_complete = 1;
                expected_token_type = -4;
            }

            // Expected some kind of operator, this is an unknown operator
            // Next token should be an operand of some kind
            else if (expected_token_type == 3 || expected_token_type == 4) {
                add_to_list(error, print_error("Error: Parse error in expression %d: Unknown operator.", expression_index));
                add_to_list(error, print_token(head->data));

                // If expected token was of arithmetic type, then next token should be an arithmetic operand token
                expected_token_type = (expected_token_type == 3) ? 1 : 2;
            }

            head = head->next;
        }

        // 7. NULL terminator ONLY
        // At top level of eval-scope, this ends IMMEDIATELY expression
        // However, if the expression isn't complete, add that to the buffer
        else if (token_type == -1) {
            // Expression not complete
            if(expression_complete == 1) {
//                // Case 1: we can identify what was missing
//                if(expected_token_type == 1 || expected_token_type == 2)
//                    add_to_list(error, print_error("Error: Scan error in expression %d: Missing operand.", expression_index));
//
//                else if (expected_token_type == 3 || expected_token_type == 4)
//                    add_to_list(error, print_error("Error: Parse error in expression %d: Missing operator.", expression_index));
//
//                // This error is added whether or not we can identify what was missing
//                add_to_list(error, print_error("Error: Parse error in expression %d: Incomplete expression.", expression_index));
//            } else {
                add_to_list(error, print_error("Error: Parse error in expression %d: Expression not ended.", expression_index));
            }
            head = head->next;
        }
    }

    // Before returning, check if the expression was completed
    // If the expression terminated before a pattern could be completed, append errors for what was missing
    // as well as the incomplete expression error
    if (expression_complete != 1) {
        // Missing an operand
        if (expected_token_type == 1 || expected_token_type == 2) {
            add_to_list(error, print_error("Error: Parse error in expression %d: Missing operand.", expression_index));
        }

        if (expected_token_type == 3 || expected_token_type == 4) {
            add_to_list(error, print_error("Error: Parse error in expression %d: Missing operator.", expression_index));
        }
        add_to_list(error, print_error("Error: Parse error in expression %d: Incomplete expression.", expression_index));
    }

    free_list(list);
    return expression_type;
}

/*
 * Token types:
 * -2 = Unknown token
 * -1 = Empty token
 * 1 = Arithmetic operand token
 * 2 = Logical operand token
 * 3 = Arithmetic operator token
 * 4 = Logical operator token
 * 5 = Logical, unary, operator token
 * 6 = NULL terminator token
 */
int get_token_type(Node* token) {

    // -1 = Empty tokens
    if(token == NULL) return -1;
    if(token->data[0] == '\0') return -1;


    // 1 = Legal arithmetic tokens
    if(string_compare("0", token->data)) return 1;
    if(string_compare("1", token->data)) return 1;
    if(string_compare("2", token->data)) return 1;
    if(string_compare("3", token->data)) return 1;
    if(string_compare("4", token->data)) return 1;
    if(string_compare("5", token->data)) return 1;
    if(string_compare("6", token->data)) return 1;
    if(string_compare("7", token->data)) return 1;
    if(string_compare("8", token->data)) return 1;
    if(string_compare("9", token->data)) return 1;

    // 2 = Logical operand tokens
    if(string_compare("true", token->data)) return 2;
    if(string_compare("false", token->data)) return 2;

    // 3 = Arithmetic operator tokens
    if(string_compare("+", token->data)) return 3;
    if(string_compare("-", token->data)) return 3;
    if(string_compare("*", token->data)) return 3;
    if(string_compare("/", token->data)) return 3;

    // 4 = Logical operator tokens
    if(string_compare("AND", token->data)) return 4;
    if(string_compare("OR", token->data)) return 4;

    // 5 = Logical, unary, operator token
    if(string_compare("NOT", token->data)) return 5;

    // Not any of the above, must be unknown.
    // -2 = Unknown identifier
    return -2;
}

/* Usage: accepts two strings and compares each character iteratively
 * Returns 1 if the strings are equal, 0 otherwise.
 */
int string_compare(const char* first, const char* second) {
    // Cannot compare if either strings are NULL
    // Immediately exit with unequal code
    if(first == NULL || second == NULL) {
        return 0;
    }

    int index = 0;

    while(first[index] != '\0' && second[index] != '\0' && first[index] == second[index]) {
        index++;
    }

    if(first[index] == '\0' && second[index] == '\0')
        return 1;
    else
        return 0;
}

// Prints a formatted error message:
// Error: <error_type> in expression <num>: <error>
char* print_error(char* error_msg, int index) {
    char *error;
    asprintf(&error, error_msg, index);
    return error;
}

// Prints a formatted token message:
char* print_token(char *token_data) {
    char *token_msg;
    asprintf(&token_msg, "\t'%s'", token_data);
    return token_msg;
}

Node* add_to_list(Node* head, char* data) {
    Node* prev = head;

    Node* new_node = (Node*) malloc(sizeof(Node));
    new_node->next = NULL;
    new_node->data = data;

    // Insert the node
    if (prev == NULL) {
        // Head is empty, new node is the head
        head = new_node;
    } else {
        // Insert node at the end of the list
        while(prev->next != NULL) {
            prev = prev->next;
        }
        prev->next = new_node;
    }

    return head;
}

// Creates a token from some input string and appends it to the passed list of tokens
Node* add_token(Node *head, const char *str, int start_index, int end_index) {

    // Allocate the length of the selection that is being copied in, plus one
    // for the null terminator
    char *new_token = (char *) calloc(sizeof(char), end_index - start_index + 1);

    int index = 0;
    int i = start_index;
    for(; i < end_index; i++) {
        new_token[index++] = str[i];
    }

    head = add_to_list(head, new_token);

    return head;
}

void print_list(Node* head) {
    head = head->next;

    while( head != NULL) {
        printf("%s\n", head->data);
        head = head->next;
    }
}

// Frees the list associated with a head
void free_list(Node* head) {
    while (head != NULL) {
        Node* temp = head->next;
        if(head->data != NULL)
            free(head->data);

        free(head);

        head = temp;
    }
}