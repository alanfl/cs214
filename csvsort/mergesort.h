//
// Created by test on 9/5/19.
//

#ifndef CSVSORT_MERGESORT_H
#define CSVSORT_MERGESORT_H

/*
 * Note: Using bottom-up mergesort.
 *
 * Usage: Pass in arrays and target_array will return sorted.
 */
void mergesort(target_array[], work_array[], int n);

void mergesort_split(target_array[], left, right, end, work_array[]);

#endif //CSVSORT_MERGESORT_H
