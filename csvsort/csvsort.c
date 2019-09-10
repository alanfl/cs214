//
// Created by alanfl on 9/5/19.
//
#include "csvsort.h"
#include "mergesort.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char** argv) {

    struct CSVFile* csv_file = read_file(argv[1]);

    free(csv_file);

    return 0;
}

struct Record* create_record(int num_columns, char* line, int line_num) {
    struct Record* record = malloc(sizeof(struct Record));
    record->record_num = line_num;
    record->values = malloc(num_columns * sizeof(char *));

    int col = 0;
    for(char* p = strtok(line, ","); p != NULL; p = strtok(NULL, ",")) {
        record->values[col] = p;
        col++;

        if(col > num_columns) {
            return NULL; // Immediately throw error if line elements exceeds the number of columns
        }
    }
}


struct CSVFile* read_file(char* file_name) {

    // Check that file is of .csv type
    char *dot = strrchr(file_name, '.');
    if (dot && !strcmp(dot, ".csv")) {
        printf("%s", "Error: file must have extension '.csv'");
        return NULL;
    }

    FILE *csv_file = fopen(file_name, "r");

    if (!csv_file) {
        printf("%s", "Error: file does not exist.");
    }

    // Get number of lines in file
    int ch;
    int lines = 0;
    while (EOF != (ch = getc(csv_file))) {
        if (ch == '\n') {
            ++lines;
        }
    }

    rewind(csv_file);

    // Fetch the first line of the file to obtain the number of columns
    char line[256];
    fscanf(csv_file, line, &line);

    char* columns = strtok(line, ",");

    // Instantiate and malloc CSVFile
    struct CSVFile* csv = malloc(sizeof(struct CSVFile));
    csv->file_name = file_name;
    csv->num_of_records = lines;
    csv->records = malloc(lines * sizeof(struct Record));
}

