
#include <random>

#include <stdio.h>

#include "gtest/gtest.h"

#include "naivelinalg.h"

TEST(MATRIX_f64,creation_destruction_macros) {
    matrix amat;
    
    bool success_value;
    success_value = matrix_create(&amat, 4, 5);
    ASSERT_EQ(success_value, true) << "Error creating matrix.";
    
    EXPECT_NE(amat.data, nullptr);
    EXPECT_EQ(amat.N, 4) << "Wrong number of rows!";
    EXPECT_EQ(amat.M, 5) << "Wrong number of cols!";
    EXPECT_EQ(MATRIX_SIZE(amat),4*5) << "Created matrix does not have the right size!";
    EXPECT_EQ(MATRIX_STORAGE_BYTES(amat),4*8*sizeof(matrix_data_t)) << "Wrong storage size?";

    success_value = matrix_destroy(&amat);
    ASSERT_EQ(success_value, true) << "Error destroying matrix.";
    
    EXPECT_EQ(amat.data, nullptr) << "Data pointer not nulled out by matrix_destory().";
    EXPECT_EQ(amat.N, 0) << "N not zeroed by matrix_destory().";
    EXPECT_EQ(amat.M, 0) << "M not zeroed by matrix_destory().";
    
    //TODO: Double creation and double free using VALGRIND.
}


TEST(MATRIX_f64,matrix_copy_1) {
    matrix amat, bmat, cmat;
    
    bool success_value;
    success_value = matrix_create(&amat, 4, 4);
    ASSERT_EQ(success_value, true) << "Exception creating matrix.";
    success_value = matrix_create(&bmat, 4, 4);
    ASSERT_EQ(success_value, true) << "Exception creating matrix.";
    success_value = matrix_create(&cmat, 4, 4);
    ASSERT_EQ(success_value, true) << "Exception creating matrix.";
    
    //Ensure that they are distinct matrices.
    ASSERT_NE(amat.data,bmat.data) << "Matrices alias each other!";
    ASSERT_NE(bmat.data,cmat.data) << "Matrices alias each other!";
    
    
    success_value = matrix_set(&cmat, 1.5);
    ASSERT_EQ(success_value, true) << "Exception in matrix_set.";
    
    success_value = matrix_set(&amat, 40.0);
    ASSERT_EQ(success_value, true) << "Exception in matrix_set.";
    //TODO: test some elements in amat
    
    
    ASSERT_EQ(amat.N,bmat.N) << "Matrices to copy not same shape! (Problem in test itself.)";
    ASSERT_EQ(amat.M,bmat.M) << "Matrices to copy not same shape! (Problem in test itself.)";
    
    ASSERT_EQ(bmat.N,cmat.N) << "Matrices to copy not same shape! (Problem in test itself.)";
    ASSERT_EQ(bmat.M,cmat.M) << "Matrices to copy not same shape! (Problem in test itself.)";
    success_value = matrix_copy(&bmat, &cmat);
    ASSERT_EQ(success_value, true) << "Exception in matrix_copy.";
    success_value = matrix_copy(&amat, &bmat);
    ASSERT_EQ(success_value, true) << "Exception in matrix_copy.";
    
    //Confirm that every value is the same.
    for(int i = 0;i<amat.N;i++)
        for(int j = 0;j<amat.M;j++){
            ASSERT_EQ( MATRIX_ELEMENT(amat,i,j),
                        MATRIX_ELEMENT(cmat,i,j)
                    ) << "amat[" << i << "," << j << "] != cmat[" << i << "," << j << "] .";
        }
    
    
    success_value = matrix_destroy(&amat);
    ASSERT_EQ(success_value, true) << "Error destroying matrix A.";
    success_value = matrix_destroy(&bmat);
    ASSERT_EQ(success_value, true) << "Error destroying matrix B.";
    success_value = matrix_destroy(&cmat);
    ASSERT_EQ(success_value, true) << "Error destroying matrix C.";
}


TEST(MATRIX_f64,matrix_set_1) {

    const matrix_data_t copy_val = 1.5;
    const int N = 4;
    const int M = 5;
    matrix amat;
    bool success_value;
    
    success_value = matrix_create(&amat, N, M);
    ASSERT_EQ(success_value, true) << "Exception creating matrix.";
    
    success_value = matrix_set(&amat, copy_val);
    ASSERT_EQ(success_value, true) << "Exception in matrix_set.";
    
    //Confirm that every value is the same.
    for(int i = 0;i<amat.N;i++)
        for(int j = 0;j<amat.M;j++){
            EXPECT_EQ( MATRIX_ELEMENT(amat,i,j),
                        copy_val
                    ) << "amat[" << i << "," << j << "] != " << copy_val << " .";
        }
    
    
    success_value = matrix_destroy(&amat);
    ASSERT_EQ(success_value, true) << "Exception destroying matrix A.";
}

TEST(MATRIX_f64,matrix_set_diag_1) {

    const matrix_data_t diag_val = 1.5;
    const matrix_data_t off_diag_val = 0.0;
    const int N = 4;
    const int M = 5;
    matrix amat;
    bool success_value;
    
    success_value = matrix_create(&amat, N, M);
    ASSERT_EQ(success_value, true) << "Exception creating matrix.";
    
    success_value = matrix_set(&amat, off_diag_val);
    ASSERT_EQ(success_value, true) << "Exception in matrix_set.";
    
    success_value = matrix_set_diag(&amat, diag_val);
    ASSERT_EQ(success_value, true) << "Exception in matrix_set_diag.";
    
    //Confirm that every value is the same.
    for(int i = 0;i<amat.N;i++)
        for(int j = 0;j<amat.M;j++){
            
            if(i == j){//Main diagonal
                EXPECT_EQ( MATRIX_ELEMENT(amat,i,j), diag_val ) << "Diagonal value wrong.";
            } else {
                EXPECT_EQ( MATRIX_ELEMENT(amat,i,j), off_diag_val ) << "Off-Diagonal value wrong.";
            }
        }
    
    
    success_value = matrix_destroy(&amat);
    ASSERT_EQ(success_value, true) << "Exception destroying matrix A.";
}

TEST(MATRIX_f64,matrix_eye_1) {

    const matrix_data_t diag_val = 1.0;
    const matrix_data_t off_diag_val = 0.0;
    const int N = 4;
    const int M = 5;
    matrix amat;
    bool success_value;
    
    success_value = matrix_create(&amat, N, M);
    ASSERT_EQ(success_value, true) << "Exception creating matrix.";
    
    success_value = matrix_set(&amat, diag_val + 0.1);//Set to unwanted value
    ASSERT_EQ(success_value, true) << "Exception in matrix_set.";
    
    success_value = matrix_eye(&amat);
    ASSERT_EQ(success_value, true) << "Exception in matrix_eye.";
    
    //Confirm that every value is the same.
    for(int i = 0;i<amat.N;i++)
        for(int j = 0;j<amat.M;j++){
            
            if(i == j){//Main diagonal
                EXPECT_EQ( MATRIX_ELEMENT(amat,i,j), diag_val ) << "Diagonal value wrong.";
            } else {
                EXPECT_EQ( MATRIX_ELEMENT(amat,i,j), off_diag_val ) << "Off-Diagonal value wrong.";
            }
        }
    
    
    success_value = matrix_destroy(&amat);
    ASSERT_EQ(success_value, true) << "Exception destroying matrix A.";
}

TEST(MATRIX_f64,matrix_mult_scalar_1) {
    matrix amat, bmat;
    
    bool success_value;
    success_value = matrix_create(&amat, 4, 4);
    ASSERT_EQ(success_value, true) << "Exception creating matrix.";
    success_value = matrix_create(&bmat, 4, 4);
    ASSERT_EQ(success_value, true) << "Exception creating matrix.";
    
    //Ensure that they are distinct matrices.
    ASSERT_NE(amat.data,bmat.data) << "Matrices alias each other!";
    
    
    success_value = matrix_set(&bmat, 2.0);
    ASSERT_EQ(success_value, true) << "Exception in matrix_set.";
    
    success_value = matrix_set(&amat, 40.0);
    ASSERT_EQ(success_value, true) << "Exception in matrix_set.";
    //TODO: test some elements in amat
    
    
    ASSERT_EQ(amat.N,bmat.N) << "Matrices to copy not same shape! (Problem in test itself.)";
    ASSERT_EQ(amat.M,bmat.M) << "Matrices to copy not same shape! (Problem in test itself.)";
    
    
    success_value = matrix_mult_scalar(&amat, &bmat, 7.0);
    ASSERT_TRUE(success_value) << "Exception in matrix_mult_scalar (different matrices).";
    
    //Confirm every value
    for(int i = 0;i<amat.N;i++)
        for(int j = 0;j<amat.M;j++){
            ASSERT_EQ( MATRIX_ELEMENT(amat,i,j), 14.0 ) << "Incorrect value after scalar mult (out of place).";
        }
    
    success_value = matrix_mult_scalar(&amat, &amat, 2.0);
    ASSERT_TRUE(success_value) << "Exception in matrix_mult_scalar (in-place).";
    
    //Confirm every value (in-place)
    for(int i = 0;i<amat.N;i++)
        for(int j = 0;j<amat.M;j++){
            ASSERT_EQ( MATRIX_ELEMENT(amat,i,j), 28.0 ) << "Incorrect value after scalar mult (in-place).";
        }
    
    success_value = matrix_destroy(&amat);
    ASSERT_EQ(success_value, true) << "Error destroying matrix A.";
    success_value = matrix_destroy(&bmat);
    ASSERT_EQ(success_value, true) << "Error destroying matrix B.";
}

TEST(MATRIX_f64,matrix_add_scalar_1) {
    matrix amat, bmat;
    
    bool success_value;
    success_value = matrix_create(&amat, 4, 4);
    ASSERT_EQ(success_value, true) << "Exception creating matrix.";
    success_value = matrix_create(&bmat, 4, 4);
    ASSERT_EQ(success_value, true) << "Exception creating matrix.";
    
    //Ensure that they are distinct matrices.
    ASSERT_NE(amat.data,bmat.data) << "Matrices alias each other!";
    
    
    success_value = matrix_set(&bmat, 2.0);
    ASSERT_EQ(success_value, true) << "Exception in matrix_set.";
    
    success_value = matrix_set(&amat, 40.0);
    ASSERT_EQ(success_value, true) << "Exception in matrix_set.";
    //TODO: test some elements in amat
    
    
    ASSERT_EQ(amat.N,bmat.N) << "Matrices to copy not same shape! (Problem in test itself.)";
    ASSERT_EQ(amat.M,bmat.M) << "Matrices to copy not same shape! (Problem in test itself.)";
    
    
    success_value = matrix_add_scalar(&amat, &bmat, 7.0);
    ASSERT_TRUE(success_value) << "Exception in matrix_mult_scalar (different matrices).";
    
    //Confirm every value
    for(int i = 0;i<amat.N;i++)
        for(int j = 0;j<amat.M;j++){
            ASSERT_EQ( MATRIX_ELEMENT(amat,i,j), 9.0 ) << "Incorrect value after scalar mult (out of place).";
        }
    
    success_value = matrix_add_scalar(&amat, &amat, 2.0);
    ASSERT_TRUE(success_value) << "Exception in matrix_mult_scalar (in-place).";
    
    //Confirm every value (in-place)
    for(int i = 0;i<amat.N;i++)
        for(int j = 0;j<amat.M;j++){
            ASSERT_EQ( MATRIX_ELEMENT(amat,i,j), 11.0 ) << "Incorrect value after scalar mult (in-place).";
        }
    
    success_value = matrix_destroy(&amat);
    ASSERT_EQ(success_value, true) << "Error destroying matrix A.";
    success_value = matrix_destroy(&bmat);
    ASSERT_EQ(success_value, true) << "Error destroying matrix B.";
}


TEST(MATRIX_f64,matrix_hadamard_mult_1) {
    matrix amat, bmat, cmat;
    
    const matrix_data_t a_init_val = 40.0, b_init_val=2.0, c_init_val = 1.5;
    
    bool success_value;
    success_value = matrix_create(&amat, 4, 4);
    ASSERT_EQ(success_value, true) << "Exception creating matrix.";
    success_value = matrix_create(&bmat, 4, 4);
    ASSERT_EQ(success_value, true) << "Exception creating matrix.";
    success_value = matrix_create(&cmat, 4, 4);
    ASSERT_EQ(success_value, true) << "Exception creating matrix.";
    
    //Ensure that they are distinct matrices.
    ASSERT_NE(amat.data,bmat.data) << "Matrices alias each other!";
    ASSERT_NE(bmat.data,cmat.data) << "Matrices alias each other!";
    
    //Initialize values
    success_value = matrix_set(&amat, a_init_val);
    ASSERT_EQ(success_value, true) << "Exception in matrix_set.";
    
    success_value = matrix_set(&bmat, b_init_val);
    ASSERT_EQ(success_value, true) << "Exception in matrix_set.";
    
    success_value = matrix_set(&cmat, c_init_val);
    ASSERT_EQ(success_value, true) << "Exception in matrix_set.";
    
    
    //TODO: test some elements in amat
    
    ASSERT_EQ(amat.N,bmat.N) << "Matrices to copy not same shape! (Problem in test itself.)";
    ASSERT_EQ(amat.M,bmat.M) << "Matrices to copy not same shape! (Problem in test itself.)";
    
    ASSERT_EQ(bmat.N,cmat.N) << "Matrices to copy not same shape! (Problem in test itself.)";
    ASSERT_EQ(bmat.M,cmat.M) << "Matrices to copy not same shape! (Problem in test itself.)";
    
    /********************************************/
    //A = B.C
    success_value = matrix_hadamard_mult(&amat, &bmat, &cmat);
    ASSERT_EQ(success_value, true) << "Exception in matrix_hadamard_mult (out of place).";
    
    //Confirm that every value is the same.
    for(int i = 0;i<amat.N;i++)
        for(int j = 0;j<amat.M;j++){
            ASSERT_EQ( MATRIX_ELEMENT(amat,i,j),
                    b_init_val*c_init_val
                ) << "Incorrect output of out-of-place matrix_hadamard_mult(A,B,C).";
        }
    
    /********************************************/
    //A = A.B
    success_value = matrix_hadamard_mult(&amat, &amat, &bmat);
    ASSERT_EQ(success_value, true) << "Exception in matrix_hadamard_mult (half-in-place).";
    
    //Confirm that every value is the same.
    for(int i = 0;i<amat.N;i++)
        for(int j = 0;j<amat.M;j++){
            ASSERT_EQ( MATRIX_ELEMENT(amat,i,j),
                    b_init_val*c_init_val*b_init_val
                ) << "Incorrect output of out-of-place matrix_hadamard_mult(A,A,B).";
        }
    
    /********************************************/
    //A = A.A
    success_value = matrix_hadamard_mult(&amat, &amat, &amat);
    ASSERT_EQ(success_value, true) << "Exception in matrix_hadamard_mult (in-place self).";
    
    //Confirm that every value is the same.
    for(int i = 0;i<amat.N;i++)
        for(int j = 0;j<amat.M;j++){
            ASSERT_EQ( MATRIX_ELEMENT(amat,i,j),
                    b_init_val*c_init_val*b_init_val*b_init_val*c_init_val*b_init_val
                ) << "Incorrect output of out-of-place matrix_hadamard_mult(A,A,B).";
        }
    
    /********************************************/
    
    success_value = matrix_destroy(&amat);
    ASSERT_EQ(success_value, true) << "Error destroying matrix A.";
    success_value = matrix_destroy(&bmat);
    ASSERT_EQ(success_value, true) << "Error destroying matrix B.";
    success_value = matrix_destroy(&cmat);
    ASSERT_EQ(success_value, true) << "Error destroying matrix C.";
}

TEST(MATRIX_f64,matrix_mult_naive_1) {
    matrix amat, bmat, cmat;
    
    const int N = 2, M = 2, K = 3;
    
    //A = B*C
    /*
    Expressed as python:
    A = array([[ 58,  64],
               [139, 154]])
    
    B = array([[1, 2, 3],
               [4, 5, 6]])
    
    C = array([[ 7,  8],
               [ 9, 10],
               [11, 12]])
    */
    
    bool success_value;
    success_value = matrix_create(&amat, N, M);
    ASSERT_EQ(success_value, true) << "Exception creating matrix.";
    success_value = matrix_create(&bmat, N, K);
    ASSERT_EQ(success_value, true) << "Exception creating matrix.";
    success_value = matrix_create(&cmat, K, M);
    ASSERT_EQ(success_value, true) << "Exception creating matrix.";
    
    //Ensure that they are distinct matrices.
    ASSERT_NE(amat.data,bmat.data) << "Matrices alias each other!";
    ASSERT_NE(bmat.data,cmat.data) << "Matrices alias each other!";
    
    //Initialize values of B
    for(int i = 0;i<bmat.N;i++){
        for(int j = 0;j<bmat.M;j++){
            MATRIX_ELEMENT(bmat,i,j) = 1 + j + i*bmat.M;
        }
    }
    
    //Initialize values of C
    for(int i = 0;i<cmat.N;i++){
        for(int j = 0;j<cmat.M;j++){
            MATRIX_ELEMENT(cmat,i,j) = 7 + j + i*cmat.M;
        }
    }
    
    //DEBUG of the tests themselves.
    //matrix_fprint(stdout, &bmat);
    //matrix_fprint(stdout, &cmat);
    
    /********************************************/
    //A = B*C
    success_value = matrix_mult_naive(&amat, &bmat, &cmat);
    ASSERT_EQ(success_value, true) << "Exception in matrix_mult_naive(A,B,C) (out of place).";
    
    //Check values
    /*
    A = array([[ 58,  64],
               [139, 154]])
    */
    ASSERT_EQ(MATRIX_ELEMENT(amat,0,0),58)  << "Wrong value in matrix after multiply.";
    ASSERT_EQ(MATRIX_ELEMENT(amat,0,1),64)  << "Wrong value in matrix after multiply.";
    ASSERT_EQ(MATRIX_ELEMENT(amat,1,0),139) << "Wrong value in matrix after multiply.";
    ASSERT_EQ(MATRIX_ELEMENT(amat,1,1),154) << "Wrong value in matrix after multiply.";
    
    /********** EXCEPTIONS **********/
    success_value = matrix_mult_naive(&amat, &cmat, &bmat);//C*B is fine, but A is the wrong shape.
    ASSERT_FALSE(success_value) << "Should have refused to multiply into destination of wrong shape.";
    
    success_value = matrix_destroy(&amat);
    ASSERT_EQ(success_value, true) << "Error destroying matrix A.";
    success_value = matrix_destroy(&bmat);
    ASSERT_EQ(success_value, true) << "Error destroying matrix B.";
    success_value = matrix_destroy(&cmat);
    ASSERT_EQ(success_value, true) << "Error destroying matrix C.";
}

TEST(MATRIX_f64,matrix_transpose_1) {
    matrix amat, bmat, cmat;
    
    const int N = 3, M = 2;
    
    //A = B*C
    /*
    Expressed as python:
    A = array([[1, 4],
               [2, 5],
               [3, 6]])
    
    B = array([[1, 2, 3],
               [4, 5, 6]])
    
    C = array([[1, 2, 3],
               [0, 0, 0],
               [0, 0, 0]])
    */
    
    bool success_value;
    success_value = matrix_create(&amat, N, M);
    ASSERT_EQ(success_value, true) << "Exception creating matrix.";
    success_value = matrix_create(&bmat, M, N);
    ASSERT_EQ(success_value, true) << "Exception creating matrix.";
    success_value = matrix_create(&cmat, N, N);
    ASSERT_EQ(success_value, true) << "Exception creating matrix.";
    
    //Ensure that they are distinct matrices.
    ASSERT_NE(amat.data,bmat.data) << "Matrices alias each other!";
    ASSERT_NE(bmat.data,cmat.data) << "Matrices alias each other!";
    
    //Initialize values of B
    for(int i = 0;i<bmat.N;i++){
        for(int j = 0;j<bmat.M;j++){
            MATRIX_ELEMENT(bmat,i,j) = 1 + j + i*bmat.M;
        }
    }
    
    //Initialize values of C
    for(int i = 0;i<cmat.N;i++){
        for(int j = 0;j<cmat.M;j++){
            if(i == 0){
                MATRIX_ELEMENT(cmat,i,j) = j + 1.0;
            }else{
                MATRIX_ELEMENT(cmat,i,j) = 0.0;
            }
        }
    }
    
    //DEBUG of the tests themselves.
    //matrix_fprint(stdout, &bmat);
    //matrix_fprint(stdout, &cmat);
    
    /********************************************/
    //A = B^T
    success_value = matrix_transpose(&amat, &bmat);
    ASSERT_EQ(success_value, true) << "Exception in matrix_transpose(A,B) (out of place).";
    
    //Check values in A
    for(int i = 0;i<amat.N;i++)
        for(int j = 0;j<amat.M;j++)
            ASSERT_EQ(MATRIX_ELEMENT(amat,i,j), MATRIX_ELEMENT(bmat,j,i)) << "Incorrect value in A after matrix_transpose(A,B) (out of place).";
    
    success_value = matrix_transpose(&cmat, &cmat);
    ASSERT_EQ(success_value, true) << "Exception in matrix_transpose(C,C) (square, in-place).";
    
    //Check values in C
    for(int i = 0;i<cmat.N;i++){
        for(int j = 0;j<cmat.M;j++){
            if(j == 0){
                ASSERT_EQ(MATRIX_ELEMENT(cmat,i,j), i + 1.0) << "Incorrect value in C after in-place transpose.";
            }else{
                ASSERT_EQ(MATRIX_ELEMENT(cmat,i,j),0.0) << "Incorrect value in C after in-place transpose.";
            }
        }
    }
    
    /********** EXCEPTIONS **********/
    //Check refusal to transpose into a bad target
    success_value = matrix_transpose(&cmat, &bmat);
    ASSERT_FALSE(success_value) << "Expected exception when trying to transpose to matrix of wrong shape.";
    
    success_value = matrix_transpose(&bmat, &bmat);
    ASSERT_FALSE(success_value) << "Expected exception when trying to transpose to matrix of wrong shape.";

    
    
    success_value = matrix_destroy(&amat);
    ASSERT_EQ(success_value, true) << "Error destroying matrix A.";
    success_value = matrix_destroy(&bmat);
    ASSERT_EQ(success_value, true) << "Error destroying matrix B.";
    success_value = matrix_destroy(&cmat);
    ASSERT_EQ(success_value, true) << "Error destroying matrix C.";
}
