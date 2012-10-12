#ifndef MATRIX_H
#define MATRIX_H

#include "matrix.cpp"
void mmultiply(float **a,float **b,float **c, int m1,int n1,int o, int p); //c=a*b
void transp(float **m,float **transpose,int rows,int cols); //transpose matrix
void madd(float **a,float **b,float **c,int m1,int n1);
void msub(float **a,float **b,int m1,int n1);
void zero_m(float **a,int m1,int n1);
void mdivide(float **a,int val,int m1,int n1);
void invrsLw(float** L,int n1,float** L_1);
void invrsUp(float** U,int n1,float** U_1);
void LU(float** A,float** A1,float** L, float** U,int m1);
void minv(float ** in,float ** out,int m1);
long double trace(float **a,int m1);
long double trace_sum(float **a,int m1,int n1);//lower
long double trace_lin(float** a,float ** b,int m1,int n1);//upper

#endif // MATRIX_H
