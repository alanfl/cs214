//
// Created by alanfl on 9/19/19.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char* data;
    struct Node* next;
} Node;

void print_list(Node* head);

int validate_expression(Node* head, int expression_num);

int get_token_type(Node* token);

int string_length(char *);
void string_copy(const char* source, char* dest, int length);

Node* tokenize(const char* str, char delim, int end);

int main(int argc, char** argv) {
    if(argc == 1) {
        printf("%s\n", "Error: No argument.");
    }

    if(argc > 2) {
        printf("%s\n", "Error: Too many arguments.");
        return 0;
    }

    char* str = argv[1];
    Node* head = tokenize(str, ';', string_length(str));

    while(head != NULL) {
        Node* expression_head = tokenize(head->data, ' ', string_length(head->data));
        print_list(expression_head);
        printf("\n");

        validate_expression(expression_head);
        head = head->next;
    }
    print_list(head);

    return 0;
}

Node* tokenize(const char* str, char delim, int end) {
    Node *head = NULL;
    Node *prev = NULL;

    int start_index = 0;
    for(int i = 0; i < end; i++) {
        if(str[i] == delim || str[i] == '\0') {
            if(i != start_index) {
                Node* new_node = malloc(sizeof(Node));
                new_node->data = malloc( (i-start_index) * sizeof(char) + 1);
                new_node->next = NULL;

                string_copy(&str[start_index], new_node->data, i-start_index);

                // Check if list is empty, if not step over
                // Otherwise assign head
                if(prev != NULL)
                    prev->next = new_node;
                else
                    head = new_node;
                prev = new_node;
            }

            if(str[i] != '\0') {
                Node* delimiter_node = malloc(sizeof(Node));
                delimiter_node->data = malloc(sizeof(char) + 1);
                delimiter_node->next = NULL;

                delimiter_node->data[0] = delim;
                delimiter_node->data[1] = '\0';

                if(prev != NULL)
                    prev->next = delimiter_node;
                else
                    head = delimiter_node;
                prev = delimiter_node;
            }

            start_index = i + 1;
        }
    }

    return head;
}

/*
 * Usage: accepts the head of a tokenized expression linked list and
 * validates whether or not the expression is legal.
 */
int validate_expression(Node* head, int expression_num) {
    if (expression_num != 0) {
        if(string_compare(head->data, " ") != 1) {
            printf("Error: Incomplete expression")
        }
    }

    while(head != NULL) {

        head = head->next;
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

void print_list(Node* head) {
    while( head != NULL) {
        printf("'%s',", head->data);
        head = head->next;
    }
}

void string_copy(const char* source, char* destination, int length) {
    for(int i = 0; i<length; i++) {
        destination[i] = source[i];
    }

    destination[length] = '\0';
}

/* Usage: accepts two strings and compares each character iteratively
 * Returns 1 if the strings are equal, 0 otherwise.
 */
int string_compare(char* first, char* second) {
    int first_length = string_length(first);
    int second_length = string_length(second);

    if(first_length != second_length) {
        return 0;
    }

    for(int i = 0; i < first_length; i++) {
        if(first[i] != second[i]) {
            return 0;
        }
    }

    return 1;
}

/*
 * Token types:
 * -1 = Illegal, unknown tokens
 * 0 = Empty token
 * 1 = Arithmetic operand token
 * 2 = Logical operand token
 * 3 = Arithmetic operator token
 * 4 = Logical operator token
 * 5 = Logical, unary, operator token
 */
int get_token_type(Node* token) {

    // 0 = Empty tokens (though not necessarily illegal)
    if(token == NULL) return 0;
    if(token->data[0] == '\0') return 0;

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

}