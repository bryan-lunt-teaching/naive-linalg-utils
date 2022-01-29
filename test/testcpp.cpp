#include <stdio.h>

#include "naivelinalg.h"

int main()
{
   // printf() displays the string inside quotation
   printf("Hello, World!\n");


	matrix my_test_mat;

	matrix_create(&my_test_mat, 4, 4);
	matrix_set(&my_test_mat,0.0);

	MATRIX_ELEMENT(my_test_mat,2,3) = 21.5;

	matrix_fprint(stdout, &my_test_mat);

	matrix_destroy(&my_test_mat);
   return 0;
}
