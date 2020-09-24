#include <iostream>
#include <fstream>
#include <ctime>
 
using namespace std;
 
//void multmat(int **A, int **B, int **C);

void impresion(int **A){
	for(int i=0; i<2000; i++){
          for(int j=0; j<2000; j++){
                  cout <<  A[i][j] << " ";
          }
    cout << endl;
      }
      
    cout << endl;
}

void multmat(int **A, int **B, int **C){
    int i, j, k;
         for(i=0; i<2000; i++){
                 for(j=0; j<2000; j++){
                C[i][j]=0;         
                         for(k=0; k<2000; k++){
                                   C[i][j]= C[i][j]+A[i][k]*B[k][j];
                         }
                 }
       }
 }
 
void lectura(int **A){
	ifstream label1 ("datos.txt"); // Abre el archivo de entrada de datos
	for(int i=0; i<2000; i++){
              for(int j=0; j<2000; j++){
                     label1 >> A[i][j];
               }
    }
    label1.close(); // Cierra el archivo de datos
} 
 
int main() {
 clock_t begin = clock();
   //ifstream label1 ("datos.txt"); // Abre el archivo de entrada de datos
 
   // Definición de variables y asignación dinámica de memoria   
   int i, j, k; 
   int **A, **B, **C; 
   A = new int* [2000], B = new int* [2000], C = new int* [2000]; 
   for(j=0; j<2000; j++){ 
      A[j] = new int [2000], B[j] = new int [2000], C[j] = new int [2000];
    }   
 
   // Lectura de las matrices (label1 apunta a datos.in en el subdirectorio datos del home    de usuario)
    lectura (A);    
	lectura(B);
 
   
  

  


   // Multiplicación de matrices A y B
    multmat(A, B, C);


   // Impresión de las matrices
   // impresion(A);
    //impresion(B);
    //impresion(C);
    


 
   delete [] A, B, C; //Desasigna la memoria
 

   clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  printf("-------------------------------------------------------------------" );
  printf("%.16g milisegundos\n", elapsed_secs );

   return 0;

}


 

