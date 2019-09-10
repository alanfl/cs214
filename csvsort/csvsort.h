//
// Created by alanfl on 9/5/19.
//

#ifndef CS214_CSVSORT_H
#define CS214_CSVSORT_H
#include <stdio.h>

/*
 * Represents a single row for a .csv file.
 *
 * @attribute int record_num = The line number of this row in its parent .csv file.
 * @attribute char values[] = The array of string values for this record.
 */
struct Record {
    int record_num;
    char values[0];
};

/*
 * Usage: Should only be called within read_file() or within scope of CSVFile.
 * Pass in a string pointer in order to generate its associated Record.
 *
 * @param char* line = Pointer to line/row that the Record refers to
 */
struct Record create_record(char* line);

/*
 * Represents a .csv file after read-in to memory,
 * where each row is represented as a Record struct.
 *
 * @attribute char* file_name = Name of the file
 * @attribute int num_of_records = Number of rows this .csv file possesses
 * @attribute struct Record records[] = The array of Record structs associated with this file
 *
 * NOTE: The first element in the "records" array represents the columns
 */
struct CSVFile {
    char* file_name;
    int num_of_records;
    struct Record records[];
};

/* Usage: Pass a file_name to generate a CSVFile struct and its
 * corresponding Record children.
 *
 * @param char* file_name = The name of the .csv file to be read
 * @returns struct CSVFile
 */
struct CSVFile read_file(char* file_name);




#endif //CS214_CSVSORT_H
