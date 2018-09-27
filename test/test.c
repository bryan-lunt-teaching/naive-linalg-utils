#include <stdio.h>

#include "naivelinalg.h"

int main()
{
   // printf() displays the string inside quotation
   printf("Hello, World!\n");


	matrix my_test_mat;

	create_matrix(&my_test_mat, 10, 10);


	destroy_matrix(&my_test_mat);
   return 0;
}
