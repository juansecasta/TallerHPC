#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/time.h>

#include <string.h>
#include <arpa/inet.h>


/*#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>*/


//#define _POSIX_SOURCE
//#include <sys/wait.h>

void carcarDatos(int *matriz, int numFilas);
void imprimirMatriz(int *matriz, int numFilas);
void reiniciarMatrizResultado(int *matriz, int numFilas);
void multiplicarMatrices(int *a, int *b, int *resultado, int numFilas);
void multiplicacionDeMatricesParalela(int *a, int *b, int *resultado, int numFilas);
void procesosParalelos(int *matrizA, int *matrizB, int *matrizResultado, int numFilas);
void multiplicacionDematricesProcesos(int *a, int *b, int *resultado, int numFilas, int numColumnaResultados);
void multiplicacion(int *a, int *b, int *resultado, int numFilas, int j);
void prueba(int *a, int *b, int *resultado, int numFilas);
double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}


int main(int argc, char *argv[])
{
    int numFilas = atoi(argv[1]);
    int numDeProcesos = numFilas;
    //clock_t tiempoDeInicia, tiempoDeFinalizacion;
    double secs;


    /*int matrizA[numFilas * numFilas];
    int matrizB[numFilas * numFilas];
    int matrizResultado[numFilas * numFilas];*/

    int *matrizA, *matrizB, *matrizResultado;

     matrizA = (int *)malloc((numFilas * numFilas) * sizeof(int));
     matrizB = (int *)malloc((numFilas * numFilas) * sizeof(int));
     matrizResultado = (int *)malloc((numFilas * numFilas) * sizeof(int));


    carcarDatos(matrizA, numFilas);
    carcarDatos(matrizB, numFilas);
    reiniciarMatrizResultado(matrizResultado, numFilas);

    /*cout << "matriz A" << endl;
    imprimirMatriz(matrizA, numFilas);

    cout << endl << "matriz B" << endl;
    imprimirMatriz(matrizB, numFilas);*/

    //double tiempoInicial = omp_get_wtime();
    reiniciarMatrizResultado(matrizResultado, numFilas);

    /*t0=clock();
    multiplicarMatrices(matrizA, matrizB, matrizResultado, numFilas);
    t1 = clock();



    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    //double total = omp_get_wtime() - tiempoInicial;
    cout << time << endl;*/

    /*tiempoDeInicia=clock();
    multiplicacionDematricesProcesos(matrizA, matrizB, matrizResultado, numFilas, numDeProcesos);
    tiempoDeFinalizacion = clock();*/


    //tiempoDeInicia = clock();
    double wall0 = get_wall_time();
    prueba(matrizA, matrizB, matrizResultado, numFilas);

    //tiempoDeFinalizacion = clock();
    double wall1 = get_wall_time();
     //double time = (double)(tiempoDeFinalizacion-tiempoDeInicia)/CLOCKS_PER_SEC;
     //double total = omp_get_wtime() - tiempoInicial;
     secs = wall1 - wall0;
     printf("tiempo de ejecucion %f\n", secs);



    /*cout << endl << "Matriz resultado" << endl;
    imprimirMatriz(matrizResultado, numFilas);*/

    return 0;

}

void prueba(int *a, int *b, int *resultado, int numFilas)
{

  pid_t procesosHijos[numFilas];
  int t, status[numFilas];

  for (t=0; t<numFilas; t++)
  {

      /*** error handling ***/
      if ((procesosHijos[t] = fork()) < 0 ) {
        printf ("No se pudo crear el subproceso\n");
        exit(0);
        }

      /*** this is the child of the fork ***/
      else if (procesosHijos[t] ==0)
      {
        multiplicacion(a,b,resultado, numFilas, t);
        exit(0);
      }
   }


   //Esperamos a que todos los procesos terminen
   for (t=0; t< numFilas; t++)
   {
    waitpid(procesosHijos[t], &status[t], 0);
   }
}

void procesosParalelos(int *matrizA, int *matrizB, int *matrizResultado, int numFilas)
{
    int numDeProcesos = numFilas;
    int status[numDeProcesos];
    pid_t procesosHijos[numDeProcesos];


    //while()
    for (int t=0; t<numDeProcesos; t++)
    {
        if ((procesosHijos[t] = fork()) < 0 )
        {
            printf("No Es posible crear un subproceso");
            exit(0);
        }


        else if (procesosHijos[t] == 0)
        {
                multiplicacionDematricesProcesos(matrizA, matrizB, matrizResultado, numFilas, t);
                exit(0);
        }
     }

     //Esperamos a que todos los procesos terminen
     for (int t=0; t< numDeProcesos; t++)
     {
        waitpid(procesosHijos[t], &status[t], 0);
     }
}


void carcarDatos(int *matriz, int numFilas)
{
  for(int i = 0; i < numFilas * numFilas; i++)
  {
    matriz[i] = rand() % 99 + 0;
  }
}

void reiniciarMatrizResultado(int *matriz, int numFilas)
{
    for(int i = 0; i < numFilas * numFilas; i++)
    {
        matriz[i] = 0;
    }
}

void imprimirMatriz(int *matriz, int numFilas)
{
  for(int i = 0; i < numFilas; i++)
  {
    for(int j = 0; j < numFilas; j++)
    {
      int indice = numFilas * i + j;
      printf("%d ", matriz[indice]);
    }
    printf("\n");
  }
}

void multiplicarMatrices(int *a, int *b, int *resultado, int numFilas)
{
    int indiceRespuesta;
    int indiceMatrizA;
    int indiceMatrizB;

    for(int i = 0; i < numFilas; i++)
    {
        for(int j = 0; j < numFilas; j++)
        {
            for(int k = 0; k < numFilas; k++)
            {
                indiceRespuesta = numFilas * i + j;
                indiceMatrizA = numFilas * i + k;
                indiceMatrizB = numFilas * k + j;
                resultado[indiceRespuesta] += a[indiceMatrizA]* b[indiceMatrizB];
            }
        }

    }
}

void multiplicacionDematricesProcesos(int *a, int *b, int *resultado, int numFilas, int numDeProcesos)
{
    int indiceRespuesta;
    int indiceMatrizA;
    int indiceMatrizB;
    int i = 0;


    int status[numDeProcesos];

    int *stats;
    stats = (int *)malloc((numFilas * numFilas) * sizeof(int));
    pid_t procesosHijos[numDeProcesos];

    while(i < numFilas)
    {
        if ((procesosHijos[i] = fork()) < 0 )
        {
            printf("No Es posible crear un subproceso");
            //exit(0);
        }

        else if (procesosHijos[i] == 0)
        {
            for(int j = 0; j < numFilas; j++)
            {
                for(int k = 0; k < numFilas; k++)
                {
                    indiceRespuesta = numFilas * i + j;
                    indiceMatrizA = numFilas * i + k;
                    indiceMatrizB = numFilas * k + j;
                    resultado[indiceRespuesta] += a[indiceMatrizA]* b[indiceMatrizB];
                }
            }

            i++;
            //exit(0);
        }
    }

    for (int t=0; t< numDeProcesos; t++)
    {
        waitpid(procesosHijos[t], &status[t], 0);
    }
}


void multiplicacion(int *a, int *b, int *resultado, int numFilas, int j)
{
  int indiceRespuesta;
  int indiceMatrizA;
  int indiceMatrizB;

	for (int i=0; i<numFilas; i++)
  {
		//mt3[i][j]=0;
		for(int k=0; k<numFilas; k++)
    {
      indiceRespuesta = numFilas * i + j;
      indiceMatrizA = numFilas * i + k;
      indiceMatrizB = numFilas * k + j;
      resultado[indiceRespuesta] += a[indiceMatrizA]* b[indiceMatrizB];
		}
		//printf("Proceso %i mt3[%i][%i]=%i\n", j, i,j, mt3[i][j]);
	}
}
