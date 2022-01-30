#include "functions.h"
#include "estructuras.h"

//Entrada: Recibe un "string" que representa el nombre del archivo de entrada y la direccion de memoria de una entero 
//Funcionamiento: Se lee el archivo de entrada, se almacena la informacion de los impactos en un arreglo y se asigna la cantidad de particulas mediante la direccion de memoria
//Salida: Arreglo de Particula que contiene la informacion de los impactos
Particula *leerEntrada(char const *nombreArchivo, int *cantidadParticulas){
    FILE *pf;
    pf = fopen(nombreArchivo, "r");

    //Se comprueba que exista el archivo de entrada indicado
    if(pf == NULL){
        Particula *listaParticulas = (Particula*)malloc(sizeof(Particula) * 0);
        *cantidadParticulas = -1;
        return listaParticulas;
    }

    //Se copia la cantidad de particulas 
    fscanf(pf, "%d", cantidadParticulas);
    //Se crea el arreglo para almacenar las particulas
    Particula *listaParticulas = (Particula*)malloc(sizeof(Particula) * (*cantidadParticulas));

    //Ciclo para leer y almacenar las particulas del archivo
    for(int i = 0; i < *cantidadParticulas; i++){
        Particula nueva;
        fscanf(pf, "%d", &nueva.posicion);
        fscanf(pf, "%f", &nueva.energia);

        //Se agrega la nueva particula al arreglo
        listaParticulas[i] = nueva;
    }

    //Se cierra el archivo
    fclose(pf);

    return listaParticulas;
}

//Entrada: Arreglo de flotantes que representa el material, entero con la catidad de celdas del material, la Particula que impacta a analizar, flotante con el umbral minimo de energia
//Funcionamiento: Se calcula la energia resultante para cada celda del material producida por el impacto de la particula indicada y se registra en caso de superar el umbral minimo
//Salida: No se retorna nada, solo se actualizan los valores del arreglo
void energiaResultante(float *arregloMaterial, int cantCeldas, Particula particula, float minEnergia){
    //Ciclo para conseguir la energia resultante en cada celda del material
    for(int i = 0; i < cantCeldas; i++){
        //Se calcula la energia resultante en la celda
        float nuevaEnergia = arregloMaterial[i] + ((1000 * particula.energia) / (cantCeldas * sqrt(abs(particula.posicion - i) + 1)));

        //Se comprueba que la energia resultante sea mayor al umbral
        if(nuevaEnergia >= minEnergia){
            arregloMaterial[i] = nuevaEnergia;
        }
    }

    return;
}


//Entrada: Arreglo de flotantes que representa el material, entero con la catidad de celdas del material, arreglo de Particulas que impactan el material, entero con la cantidad de particulas
//Funcionamiento: Se calcula el umbral minimo de energia a registrar y luego se simula el impacto de las particulas sobre el material
//Salida: No se retorna nada, solo se actualizan los valores del arreglo
void simularImpacto(float *arregloMaterial, int cantCeldas, Particula *listaParticulas, int particulaInicioSimulacion, int particulaFinSimulacion){
    //Se calcula el umbral minimo de energia
    float minEnergia = 0.001/cantCeldas;

    //Ciclo para simular el impacto desde la primera particula que corresponde hasta la ultima
    for(int i = particulaInicioSimulacion; i < particulaFinSimulacion; i++){
        energiaResultante(arregloMaterial, cantCeldas, listaParticulas[i], minEnergia);
    }

    return;
}

//Entrada: Arreglo con los descriptores de los pipes, indice del proceso actual, cantidad de procesos creados
//Funcionamiento: Se cierran todos los extremos de los pipes que no sean el extremo de escritura del pipe correspondiente al proceso
//Salida: No se retorna nada, solo se cierran los pipes
void cerrarPipes(int pipes[][2], int indiceProceso, int numeroProcesos){
    //Ciclo para cerrar los pipes que no se utilizaran
    for(int k = 0; k < numeroProcesos; k++){
        //Se cierran todos los extremos de lectura
        close(pipes[k][0]);
        //Se cierran todos los extremos de escritura que no sean del pipe correspondiente al proceso actual
        if(k != indiceProceso){
            close(pipes[k][1]);
        }
    }
    return;
}

//Entrada: Nombre del archivo de entrada, entero con la cantidad de lineas que tiene el archivo de entrada, arreglo de flotantes con el material inicial, entero con la cantidad de celdas del material
        //entero con la cantidad de procesos a crear, arreglo de enteros con los descriptores de los pipes, entero que representa el indice del proceso actual
//Funcionamiento: Crea una lista de particulas con las particulas que le corresponde simular y simula su impacto sobre el material, para finalmente escrir el resuiltado en el pipe
//Salida: Retorna un entero para indicar errores, ademas escribe los valores de cada celda del material con la energia resultante en el pipe correspondiente
int simulacionSubproceso(char *valorEntrada, int cantLineasEntrada, float *arregloMaterial, int cantCeldas, int numeroProcesos, int pipes[][2], int indiceProceso){
    
    //Se cierran los pipes que no se utilizaran
    cerrarPipes(pipes, indiceProceso, numeroProcesos);

    //Variable para almacenar la cantidad de particulas
    int cantParticulas;
    //Se lee el archivo de entrada y se almacena la informacion de las particulas en un arreglo
    Particula *listaParticulas = leerEntrada(valorEntrada, &cantParticulas);

    if(cantParticulas == -1){
        printf("El archivo de entrada no existe.\n");
        return 1;
    }

    //Se calcula la cantidad de particulas por proceso
    int particulasPorProceso = cantParticulas / numeroProcesos;
    int particulasUltimoProceso = particulasPorProceso + (cantParticulas % numeroProcesos);
    int particulasSimular = 0;
    
    //Se establece la cantidad de particulas que simulara este proceso
    //Caso de division inexacta
    if(indiceProceso == numeroProcesos - 1){
        particulasSimular = particulasUltimoProceso;
    }   
    //Caso general
    else{
        particulasSimular = particulasPorProceso;
    }

    //Se simula el impacto de las particulas correspondientes contra el material
    //Caso para el primer proceso, este comenzara desde el inicio del arreglo de particulas hasta el numero de particulas a simular
    if(indiceProceso == 0){
        simularImpacto(arregloMaterial, cantCeldas, listaParticulas, indiceProceso, particulasSimular);
    }
    //Caso general, este comenzara en la particula siguente al proceso anterior hasta la que le corresponde segun la cantida de particulas a simular
    else{
        simularImpacto(arregloMaterial, cantCeldas, listaParticulas, (indiceProceso*particulasPorProceso), ((indiceProceso*particulasPorProceso)+particulasSimular));
    }

    //Ciclo para escribir en el pipe los valores de las celdas del material resultante del impacto
    for(int j = 0; j < cantCeldas; j++){
        float mensaje = arregloMaterial[j];
        write(pipes[indiceProceso][1], &mensaje, sizeof(float));
    }

    //Se cierra el extremo de escritura del pipe
    close(pipes[indiceProceso][1]);

    //Se libera la memoria utilizada por los arreglos
    free(listaParticulas);
    free(arregloMaterial);

    return 0;
}