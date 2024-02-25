//
// Created by Aleksa on 2/24/2024.
//

// Header guards to prevent multiple inclusions of the header file
#ifndef CS130_PZ_C_STUDENT_H
#define CS130_PZ_C_STUDENT_H

// Define naming conventions for the maximum size of varchar, index, and student count
#define VARCHAR_SIZE 100
#define INDEX_SIZE 10
#define MAX_STUDENT_COUNT 100

// Define the Student struct with attributes for student data
typedef struct {
    int local_id;
    char name[VARCHAR_SIZE];
    char surname[VARCHAR_SIZE];
    char index[INDEX_SIZE];
    int year;
    float average;
} Student;

// Define the size of the Student struct
#define SIZEOF_STUDENT sizeof(Student)

// Declare a global array of Student objects and a variable to track the number of students
Student *students = NULL;
int student_count = 0;

// Free memory allocated for the array and reset student count
void student_free_array() {
    free(students);
    student_count = 0;
}

// Initialize the array of Student objects
void student_init_array() {
    if (students != NULL || student_count != 0)student_free_array();
    students = malloc(SIZEOF_STUDENT * MAX_STUDENT_COUNT);
    student_count = 0;
}

// Add a new student to the array with given attributes
void student_add_new(const char *name, const char *surname, const char *index, int year, float average) {
    students[student_count].local_id = student_count;
    strcpy(students[student_count].name, name);
    strcpy(students[student_count].surname, surname);
    strcpy(students[student_count].index, index);
    students[student_count].year = year;
    students[student_count].average = average;
    student_count++;
}

// Add a student element to the array
void student_add_element(Student *student) {
    student_add_new(student->name, student->surname, student->index, student->year, student->average);
}

// Remove a student at a specific index from the array
void student_remove_at(int index) {
    if (index < 0 || index >= student_count) return;
    for (; index < student_count - 1; index++) students[index] = students[index + 1];
    student_count--;
}

// Remove a student with a specific local ID from the array
void student_remove(int local_id) {
    int i;
    for (i = 0; i < student_count; i++)if (students[i].local_id == local_id)break;
    if (i < student_count)student_remove_at(i);
}

// Get a pointer to the student at a specific index
Student *student_get_at(int index) {
    if (index < 0 || index >= student_count) return NULL;
    return &students[index];
}

// Get a pointer to the student with a specific local ID
Student *student_get(int local_id) {
    int i;
    for (i = 0; i < student_count; i++)if (students[i].local_id == local_id)break;
    if (i < student_count)return &students[i];
    return NULL;
}

// Define function pointer type for comparing students
typedef int (*compare_func)(const void *, const void *);

// Sort the array of students using a comparison function
void student_sort(compare_func compare) {
    qsort(students, student_count, SIZEOF_STUDENT, compare);
}

// Define function pointer type for searching students
typedef int (*search_func)(const void *, const void *);

// Search for a student in the array using a search function
int student_search(search_func search, const void *key) {
    int i;
    for (i = 0; i < student_count; i++)if (search(&students[i], key))break;
    if (i < student_count)return i;
    return -1;
}

// Define function pointer type for handling student selection changes
typedef void (*student_selection_change_func)(int previous_index, int new_index);

student_selection_change_func student_selection_change = NULL;

int student_selected_index = -1;

// Retrieve the selected student from the array
Student *student_get_selected() {
    if (student_selected_index < 0 || student_selected_index >= student_count)return NULL;
    return &students[student_selected_index];
}

// Callback function to handle changes in selected student index
void student_selection_change_callback(int new_index) {
    const int current_index = student_selected_index;
    student_selected_index = new_index;
    if (student_selection_change != NULL)student_selection_change(current_index, student_selected_index);
}

#endif //CS130_PZ_C_STUDENT_H