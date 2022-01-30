#include "functions.h"
#include "estructuras.h"

//Entrada: Entero que representa la cantidad de elementos que poseera el arreglo de flotantes
//Funcionamiento: Se asigna memoria para un arreglo de flotantes del largo indicado y se asigna el valor 0 en cada posicion del arreglo
//Salida: Arreglo de flotantes con un 0 en cada posicion
float *inicializarArreglo(int largo){
    //Se asigna memoria
    float *arreglo = (float*)malloc(sizeof(float) * largo);
    //Ciclo para recorrer las posiciones del arreglo
    for(int i = 0; i < largo; i++){
        arreglo[i] = 0;
    }

    return arreglo;
}


//Entrada: Arreglo de flotantes que representa el material y un entero con la cantidad de celdas del material
//Funcionamiento: Busca el indice dentro del arreglo de la celda del material que posee mayor cantidad de energia almacenada
//Salida: Flotante con el indice dentro del arreglo de la celda con mayor energia 
int indiceMayorEnergia(float *arregloMaterial, int cantCeldas){
    //Se utiliza el primer valor del arreglo como pivote para comparar
    int indiceMayor = 0;
    float valorMayor = arregloMaterial[0];

    //Se compara con el resto de las energias
    for(int i = 1; i < cantCeldas; i++){
        //Se comprueba si la energia de la celda es mayor a la actual mayor energia
        if(arregloMaterial[i] > valorMayor){
            indiceMayor = i;
            valorMayor = arregloMaterial[i];
        }
    }

    return indiceMayor;
}

//Entrada: Arreglo de caracteres con el nombre del archivo de salida, arreglo de flotantes que representa el material, entero con la cantidad de celdas del material, entero con el indice de la celda con mayor energia almacenada
//Funcionamiento: Crea un archivo de texto con el nombre indicado y escribe la informacion de cada una de las celdas del material, partiendo con la celda de mayor energia
//Salida: Archivo de texto plano con la informacion del material
void generarSalida(char const *nombreArchivo, float *arregloMaterial, int cantCeldas, int mayorIndice){
    FILE *salida;
    //Se abre el archivo en modo escritura
    salida = fopen(nombreArchivo, "w");

    //Se escribe la informacion de la celda con mayor energia al inicio
    fprintf(salida, "%d %f\n", mayorIndice, arregloMaterial[mayorIndice]);

    //Ciclo para escribir el resto de las celdas
    for(int i = 0; i < cantCeldas; i++){
        fprintf(salida, "%d %f\n", i, arregloMaterial[i]);
    }

    //Se cierra el archivo de salida
    fclose(salida);

    return;
}

//Entrada: Arreglo de flotantes con el material, cantidad de celdas que posee el material, entero con el numero de procesos a creados y arreglo con los decriptores de todos los pipes creados
//Funcionamiento: Se leen los mensajes enviados por los pipes de cada subproceso y se actualizan en el arreglo de materiales
//Salida: No se retorna nada, solamente se actualiza la informacion de arregloMaterial sumando la informacion enviada por los subprocesos
void leerInfoHijos(float *arregloMaterial, int cantCeldas, int numeroProcesos, int pipes[][2]){
    //Ciclo para leer los resultados de cada hijo
    for(int i = 0; i < numeroProcesos; i++){
        //Ciclo para leer cada mensaje enviado por el hijo
        for(int j = 0; j < cantCeldas; j++){
            //Variable para almacenar el mensaje leido
            float mensajeLeido;
            read(pipes[i][0], &mensajeLeido, sizeof(float));
            //Se actualiza la energia almacenada en la celda del material
            arregloMaterial[j] = arregloMaterial[j] + mensajeLeido;
        }
        //Una vez leido todos los mensajes del hijo, se cierra el extremo de lectura del pipe
        close(pipes[i][0]);
    }
}