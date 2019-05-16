#ifndef __ex22_h__
#define __ex22_h__

// Makes THE_SIZE available to other .c files
extern int THE_SIZE;

// gets and sets an internal static variable in ex22.c
int get_age();
void set_age(int age);

// updates a static variable that's inside update_ratio
double update_ratio(double new_ratio);

void print_size();

#endif
