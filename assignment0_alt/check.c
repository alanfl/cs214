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
void string_copy(const char* source, char* dest, int length);

typedef struct Node {
    char* data;
    struct Node* next;
} Node;

void print_list(Node* head);

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

void print_list(Node* head) {
    while( head != NULL) {
        printf("'%s'\n", head->data);
        head = head->next;
    }
}

void string_copy(const char* source, char* destination, int length) {
    for(int i = 0; i<length; i++) {
        destination[i] = source[i];
    }

    destination[length] = '\0';
}