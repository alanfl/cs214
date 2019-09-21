//
// Created by alanfl on 9/5/19.
//
#include "csvsort.h"
//#include "mergesort.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char** argv) {

    struct CSVFile* csv = read_file(argv[1]);

    for(int i = 0; i < csv->num_of_records; i++) {
        struct Record record = *csv->records[i];
        for(int j = 0; j < csv->columns; j++) {
            printf("%s", record.values[j]);
        }
    }

    for(int i = 0; i < csv->num_of_records; i++) {
        struct Record record = *csv->records[i];
        for(int j = 0; j< csv->columns; j++) {
            if (j == 2) {
                printf("%s", record.values[j]);
            }
        }
    }

    if(csv) {
        printf("%s was converted to CSVFile.", argv[1]);
        free_csv(csv);
    }

    return 0;
}

struct Record* create_record(int num_columns, char* line, int line_num) {
    struct Record* record = (struct Record *) malloc(sizeof(struct Record));
    record->record_num = line_num;
    record->values = (char **) malloc(sizeof(char *) * num_columns);

    int col = 0;
    char* p = strtok(line, ",\n\r");
    while(p != NULL) {
        record->values[col] = malloc(sizeof(p));
        strcpy(record->values[col], p);
        col++;
        if(col > num_columns) {
            return NULL; // Immediately throw error if line elements exceeds the number of columns
        }

        p = strtok(NULL, ",");
    }

    return record;
}

struct CSVFile* read_file(char* file_name) {

    // Check that file_name is non-null
    if(!file_name) {
        printf("%s", "Error: no file name supplied.\n");
        return NULL;
    }

    // Check that file is of .csv type
    char *dot = strchr(file_name, '.');
    if (dot && strcmp(dot, ".csv") != 0) {
        printf("%s", "Error: file must have extension '.csv'\n");
        return NULL;
    }

    FILE *csv_file = fopen(file_name, "r");

    if (!csv_file) {
        printf("%s", "Error: file does not exist.\n");
        return NULL;
    }

    // Get number of lines in file
    int ch;
    int lines = 0;
    while (EOF != (ch = getc(csv_file))) {
        if (ch == '\n') {
            ++lines;
        }
    }

    // Instantiate and malloc CSVFile
    struct CSVFile* csv = malloc(sizeof(struct CSVFile));
    csv->file_name = file_name;
    csv->num_of_records = lines;
    csv->records = (struct Record **) malloc(lines * sizeof(struct Record*));

    rewind(csv_file);

    // Fetch the first line of the file to obtain the number of columns
    char line[256];
    fgets(line, 256, csv_file);

    // strtok alters line, must preserve for first record
    int num_columns = 0;
    // Iterate through
    char * p = strtok(line, ",");
    while(p != NULL) {
        num_columns++;
        p = strtok(NULL, ",");
    }

    csv->columns = num_columns;

    // Populate Record array

    rewind(csv_file);

    int line_num = 0;
    while(fgets(line, 256, csv_file) != NULL) {
        csv->records[line_num] = (struct Record *) malloc(sizeof(struct Record));
        csv->records[line_num] = create_record(num_columns, line, line_num);
        line_num++;
    }

    return csv;
}

void free_csv(struct CSVFile* csv) {
    for (int i = 0; i < csv->num_of_records; i++) {
        free(csv->records[i]->values);
        free(csv->records[i]);
    }

    free(csv);
}


