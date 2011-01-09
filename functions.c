/*
 *  functions.c
 *
 *
 *  Created by Dolores Parks on 12/24/10.
 *  Copyright 2010 Appalachian State University. All rights reserved.
 *
 *  Material for this sample is taken from the book by James Gardner
 *  entitled, "From C to C: an introduction to standard ANSI C".
 *
 *  Every C program has a function named main.  This is where execution begins.
 *  Normally, program execution ends when main ends although there are ways
 *  to prematurely end a program.
 *
 *  Main returns an int to the OS and can receive arguments from the OS
 *  (typically typed by the user at the command line).  Main returns 0 if
 *  the program ended properly.  There is a special constant for this called
 *  EXIT_SUCCESS.
 *
 *  EXIT_SUCCESS is defined in the stdlib.h header file which is available
 *  by saying '#include <stdlib.h>'.  #include directives can be used either
 *  for library files or for your own header files.  For library files you
 *  put angle brackets around the file name; for your own you use double quotes.
 *  Note that the # character must be at the left margin.
 *
 *  printf is a library function (stdio.h) that prints to standard output.  Where
 *  that is depends on the environment the program is running in, but it is
 *  typically a terminal or console of some sort.  The printf takes a string
 *  called a 'format string' as its first argument.  The format string contains
 *  the characters to be printed along with 'placeholders' for different types
 *  of values coming from expressions, variables, function calls, etc.  Here are
 *  some of the placeholders that can be used, depending on the data type of
 *  the value you want to print:
 *    %c  character
 *    %d  integer
 *    %e  double in scientific notation
 *    %f  double in form [-]mmmm.nnnnnn (precision defaults to 6)
 *    %g  double (compiler decides on %e or %f)
 *    %o  octal
 *    %u  unsigned integer
 *    %x  hexadecimal
 *    %ld long integer
 *
 */

#include <stdio.h>
#include <stdlib.h>

// Function prototypes are declarations of all the functions in the program
// other than main.  The compiler needs this information to check for errors
// in how functions are called.
int plus(int x, int y);

int main(void) {
  int a = 7, b = 3;
  int c = plus(a,b);
  printf("%d + %d = %d\n", a, b, c);

  //nesting function calls
  printf("%d + %d = %d\n", a, b, plus(a, b));

  c = plus(plus(a,b),plus(a,b));
  printf("%d + %d + %d + %d = %d\n", a, b, a, b, c);

  //playing with printf
  double dub = 18.0 / 7; // beware of integer division
  unsigned int ui = 45;

  printf(" %d \n %e \n %f \n %g \n %o \n %x \n", c, dub, dub, dub, ui, ui);

  return EXIT_SUCCESS;
}

int plus(int x, int y) {
  return x + y;
}

/*
 * First make many individual changes to this program to see the results
 * of your changes.
 *
 * Then write a program from scratch that
 *   1.  declares an int and a double,
 *   2.  passes these values to a function that multiplies them and
 *       returns their product,
 *   3.  passes them to a function that divides them and returns their
 *       quotient, and
 *   4.  prints out their product and quotient.
 *
 */
