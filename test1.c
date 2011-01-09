/*
 * test1.c
 *
 *  Created on: Jan 9, 2011
 *      Author: jeremy
 */


#include <stdio.h>
#include <stdlib.h>

double mult (double, double);
double divide (double, double);

int main (void) {
	int i;
	double d;
	i = 5;
	d = 5.4e3;

	printf ("Product \t%10.3f\n", mult(i, d));
	printf ("Quotient \t%10.3f\n", divide(d, i));

	return EXIT_SUCCESS;
}

double mult (double a, double b) {
	return a * b;
}

double divide (double a, double b) {
	return a / b;
}

