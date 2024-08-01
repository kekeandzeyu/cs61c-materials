#include <stdio.h>
#include <stdlib.h>

// Structs allow you to hold data items of different types in a single variable
// Struct definitions can be used to declare a struct variable within your program
// Struct definitions are typically done outside of a function
struct Student {
    int id;
    char* name;
};

int main() {
    // declare a variable student of type struct Student
    struct Student student;

    // print out the size of a struct Student
    printf("Size of a struct Student: %lu bytes\n", sizeof(struct Student));

    // set student's id field to 5
    student.id = 5;

    // print out student's id field
    printf("Student's ID: %d\n", student.id);

    return 0;
}
