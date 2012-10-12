void mdivide(float **a,int val,int m1,int n1){
    if (val!=0) for(int i=0;i<m1;i++) for(int j=0;j<n1;j++) a[i][j]=a[i][j]/val;
}

void zero_m(float **a,int m1,int n1){
    for(int i=0;i<m1;i++) for(int j=0;j<n1;j++) a[i][j]=0;
}
void madd(float **a,float **b,float **c,int m1,int n1){


            for(int i=0;i<m1;i++){
                for(int j=0;j<n1;j++){
                        c[i][j]=a[i][j]+b[i][j];
                }
            }


}
void msub(float **a,float **b,int m1,int n1){
    for(int i=0;i<m1;i++){
        for(int j=0;j<n1;j++){
                a[i][j]=a[i][j]-b[i][j];
        }
    }
}

void mmultiply(float **a,float **b,float **c, int m1,int n1,int o, int p){
 //m1 number of rows of A
 //n1 number of coloumns of A
 //o number of rows of B
 //p number of coloumns of B
        //float **c = new float *[m1];
        //for (int i = 0; i<m1; i++) c[i] = new float [p];
        if (n1==o){
            for(int i=0;i<m1;i++){
                for(int j=0;j<p;j++){
                    c[i][j]=0;
                    for( int k=0;k<n1;k++){
                        c[i][j]=c[i][j]+a[i][k]*b[k][j];
                    }
                }
            }
        }
        //else printf("Multiplication is not possible");
        //return c;
 }


void transp(float **m,float **transpose,int rows,int cols){
        //òðàíñïîíèðóåì Ì è çàïèñûâàåì â transpose
        // Allocate memory for transpose, and check if
        // the memory was allocated with success.
        //float **transpose = new float*[cols];
        //for (int i = 0; i < cols; i++) transpose[i] = new float[rows];
        // Build the transpose:
        for (int i = 0; i < rows; i++)
                for (int j = 0; j < cols; j++)
                        transpose[j][i] = m[i][j];
        // Free memory
        /*for (int i = 0; i < cols; i++)
        {
                delete transpose[i];
                transpose[i] = NULL;
        }
        delete[] transpose;//*/
}

void LU(float** A,float** A1,float** L, float** U,int m1){
     //ââîäåìî òèì÷àñîâèé ìàññèâ À1, ùîá íå çì³íþâàòè À;
       int i,j,r;
       int size=m1;
       for (i=0; i<size; i++) for (j=0; j<m1; j++) A1[i][j]=A[i][j];
       //Îáíóëþºìî Ó òà Ë
       for (i=0; i<size; i++) for (j=0; j<m1; j++) U[i][j]=0;
       for (i=0; i<size; i++) for (j=0; j<m1; j++) L[i][j]=0;

        float d,e;
        for (r=0;r<m1;r++){
             d=1/A1[r][r];
             for (i=(r+1);i<m1;i++){
                 e=d*A1[i][r];
                 A1[i][r]=e;
                 for (j=(r+1);j<m1;j++){
                        A1[i][j]=A1[i][j]-e*A1[r][j];
                 }

             }
        }
        for (i=0; i<size; i++) for(j=i;j<m1;j++) U[i][j]=A1[i][j];
        for (i=0; i<size; i++) for(j=0;j<i;j++) L[i][j]=A1[i][j];
        for (i=0; i<size; i++) L[i][i]=1;
}
void invrsUp(float** U,int n1,float** U_1) {
float s;
for(int i=0;i<n1;i++){
 for(int j=0;j<n1;j++){
    U_1[i][j]=0;
 }
}
   for (int j=n1-1;j>=0;j--){
     for (int i=j;i>=0;i--){
        if (i==j) U_1[i][i]=1/U[i][i];
        else{
           s=0;
           for (int k=j;k>i;k--) s+=U[i][k]*U_1[k][j];
           U_1[i][j]=-s/U[i][i];
        }
       }
   }
}
void invrsLw(float** L,int n1,float** L_1) {
float s;
for(int i=0;i<n1;i++){
 for(int j=0;j<n1;j++){
     L_1[i][j]=0;
 }
}
   for (int j=0;j<n1;j++){
     for (int i=j;i<n1;i++){
        if (i==j) L_1[i][i]=1/L[i][i];
        else{
           s=0;
           s=0;
           for (int k=j;k<i;k++) s+=L[i][k]*L_1[k][j];
           L_1[i][j]=-s/L[i][i];
        }
        }

   }
}
void minv(float ** in,float ** out,int m1){
    /*inv(A) = inv((U)*inv(L)*I*/

    float **eye= new float*[m1];
    for(int i=0;i<m1;i++) eye[i] = new float[m1];
    for(int i=0;i<m1;i++){
        for(int j=0;j<m1;j++){
            if (i!=j)eye[i][j]=0;
            else eye[i][j]=1;
        }
    }
    float **temp= new float*[m1];
    for(int i=0;i<m1;i++) temp[i] = new float[m1];
    float **L= new float*[m1];
    for(int i=0;i<m1;i++) L[i] = new float[m1];
    float **U= new float*[m1];
    for(int i=0;i<m1;i++) U[i] = new float[m1];
    float **inv_U= new float*[m1];
    for(int i=0;i<m1;i++) inv_U[i] = new float[m1];
    float **inv_L= new float*[m1];
    for(int i=0;i<m1;i++) inv_L[i] = new float[m1];
    float **inv_res= new float*[m1];
    for(int i=0;i<m1;i++) inv_res[i] = new float[m1];


    LU(in,temp,L, U,m1);
    invrsUp(U,m1,inv_U);
    invrsLw(L,m1,inv_L);
    mmultiply(inv_U,inv_L,inv_res,m1,m1,m1,m1);
    mmultiply(inv_res,eye,out,m1,m1,m1,m1);

    for (int i = 0; i <m1; i++){
        delete[] eye[i];
        delete[] temp[i];
        delete[] L[i];
        delete[] U[i];
        delete[] inv_U[i];
        delete[] inv_L[i];
        delete[] inv_res[i];
    }
    delete[] eye;
    delete[] temp;
    delete[] L;
    delete[] U;
    delete[] inv_U;
    delete[] inv_L;
    delete[] inv_res;
}

long double trace(float **a,int m1){
    long double sum=0;
    for(int i=0;i<m1;i++) sum+=a[i][i];
    return sum;
}
long double trace_sum(float **a,int m1,int n1){
    long double sum=0;
    for(int i=0;i<m1;i++) for(int j=0;j<n1;j++) sum+=a[i][j]*a[i][j];
    return sum;
}

long double trace_lin(float** a,float ** b,int m1,int n1){
    long double c=0;
    float **temp_mult= new float*[m1];
    for(int i=0;i<m1;i++) temp_mult[i] = new float[m1];
    float **temp_transp= new float*[n1];
    for(int i=0;i<n1;i++) temp_transp[i] = new float[m1];

    transp(b,temp_transp,m1,n1);
    mmultiply(a,temp_transp,temp_mult,m1,n1,n1,m1);
    c=trace(temp_mult,m1);

    for(int i=0;i<n1;i++) delete temp_transp[i];
    delete[] temp_transp;
    for(int i=0;i<m1;i++) delete temp_mult[i];
    delete[] temp_mult;
    return c;
}
