#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <malloc.h>

#include "pcic44.c"

#include "bmp.c"

void ObtenerParametrosCM(void);
void ObtenerTamanyoImagenBMP(void);
void ObtenerPatronesEntrenamiento(void);


FILE *fbmp;
FILE *fparam;
FILE *fentr;

unsigned long TamVX, TamVY;
unsigned long NumPlanos;
BMPHEADER cabeceraBMP;
unsigned long anchoBMP, altoBMP;
unsigned char V[MAX_VENTANA][MAX_VENTANA];
unsigned long buffer[1024];




void ObtenerParametrosCM()
{
    int i;
    unsigned long Ventana[2];
    unsigned long LimitePlanos[MAX_PLANOS];
    unsigned long Piezas[MAX_PIEZAS];


    fscanf(fparam,"%ld %ld %ld", &TamVX, &TamVY, &NumPlanos);
    Ventana[0] = TamVX;
    Ventana[1] = TamVY;

    printf("Tama¤o de Ventana %ld,%ld\n", Ventana[0], Ventana[1]);
    printf("Planos: %ld\n", NumPlanos);
	printf("Limites:\n");
    for ( i=0 ; i<NumPlanos+1 ; i++ ) {
		fscanf(fparam,"%ld",&LimitePlanos[i]);
		printf("%ld ",LimitePlanos[i]);
    }

	printf("\nPiezas\n");
    for( i=0 ; i<5 ; i++ ) {
    	fscanf(fparam,"%ld",&Piezas[i]);
        printf("%ld ",Piezas[i]);
    }
	printf("\n");

    printf("Escribiendo parametros de CM en la Mem Compartida.\n");

    EscribirBloqueEnteros( BASE, 2, Ventana );
    EscribirBloqueEnteros( BASE + 2, 1, &NumPlanos );
    EscribirBloqueEnteros( BASE + 3, (NumPlanos + 1), LimitePlanos );
    EscribirBloqueEnteros( (BASE + NumPlanos + 4), 5, Piezas );

    printf("Darle paso al procesador 1.\n");
    PonerValorSemaforo( 0, VERDE );

    printf("Esperando lectura PARAMETROS CM procesador 1.\n");
    while( ObtenerValorSemaforo( 0 ) == VERDE );

    fclose(fparam);

}


void ObtenerTamanyoImagenBMP()
{

    ReadBmpHeader(&cabeceraBMP, fbmp);

	/* Ir al comienzo de los datos */
	fseek(fbmp, cabeceraBMP.Header.Offset, SEEK_SET);

	/* Obtener los tama¤os de la imagen */
    anchoBMP = (unsigned long) cabeceraBMP.WinInfo.Width;
    altoBMP  = (unsigned long) cabeceraBMP.WinInfo.Height;

    printf("Ancho: %ld - Alto: %ld\n", anchoBMP, altoBMP);
 
    EscribirBloqueEnteros( BASE, 1, &anchoBMP);
    EscribirBloqueEnteros( (BASE + 1), 1, &altoBMP );

    PonerValorSemaforo( 0, VERDE );
    while( ObtenerValorSemaforo( 0 ) == VERDE );

}

void ObtenerVentana(int Limites[])
{
    unsigned long dato;
    int indice, x, y;

    LeerVentana(cabeceraBMP, fbmp, Limites, V);
    for( y=0 ; y<TamVY ; y++ ) {
        for( x=0 ; x<TamVX ; x+=4 ) {
            dato =  (unsigned long) V[y][x]   << 24;
            dato |= (unsigned long) V[y][x+1] << 16;
            dato |= (unsigned long) V[y][x+2] << 8;
            dato |= (unsigned long) V[y][x+3];
            indice = (int)TamVX*y/4 + x/4;
            buffer[indice] = dato;
        }
    }

    EscribirBloqueEnteros( BASE, (TamVX*TamVY/4), buffer);

    return;
}


void ObtenerPatronesEntrenamiento()
{
    int nP, j, i;
    float patrones[ 50*MAX_PLANOS ];
    float dato, V[4];
    int Dimension;

    /* Extraer datos del Fichero */
    fscanf(fentr,"%d",&nP);
    fscanf(fentr,"%d",&Dimension);

    printf("Patrones de ENTRENAMIENTO.\n");
    printf("Num. Patrones: %d\n",nP);
    printf("Dimension: %d\n",Dimension);

    /*** Escribir los patrones de Entrenamiento en la Mem Compartida */
    printf("Escribiendo patrones Entrenamiento en la Mem Compartida.\n");

    dato = (float) nP;
    EscribirBloqueFlotantes( BASE, 1, &dato);

    dato = (float) Dimension;
    EscribirBloqueFlotantes( (BASE + 1), 1, &dato );

    V[0] = (float)anchoBMP;
    V[1] = (float)altoBMP;
    V[2] = (float)TamVX;
    V[3] = (float)TamVY;
    EscribirBloqueFlotantes( (BASE + 2), 4, V );

    PonerValorSemaforo( 1, VERDE );
    while( ObtenerValorSemaforo( 1 ) == VERDE );


    for( j = 0 ; j < nP ; j++ ) {
        for( i = 0 ; i < ( Dimension + 1 ) ; i++ ) {
            fscanf(fentr,"%f",&dato);
            patrones[ (j%50) * ( Dimension + 1 ) + i ] = dato;
        }
        if( (j%50) == 49 )
        {
                EscribirBloqueFlotantes( BASE, 50 * (Dimension+1), patrones );
                PonerValorSemaforo( 1, VERDE );
                while( ObtenerValorSemaforo( 1 ) == VERDE );
        }
    }
    EscribirBloqueFlotantes( BASE, (nP%50) * (Dimension+1), patrones );
    PonerValorSemaforo( 1, VERDE );
    while( ObtenerValorSemaforo( 1 ) == VERDE );

    fclose(fentr);

    return;
}

void main(int argc, char *argv[])
{
    int x, y, j, numRes;
    float resultados[20];
    float cm[MAX_PLANOS];
    int Lim[4];

    clock_t tIni, tFin;
    float tiempo;


    if( argc != 4 ) {
        fprintf(stderr,"Numero de parametros incorrecto.\n");
        fprintf(stderr,"USO:\t pcicmmao  <imagen.bmp>   <parametros.cm> <patrones.ent>\n");
        exit(ERROR);
    }

    if( ( fbmp = fopen( argv[1], "rb" ) ) == NULL )
	{
	    fprintf(stderr,"No se puede Abrir el Fichero BMP: %s\n",argv[1]);
	    exit(-1);
	}

    if( ( fparam = fopen( argv[2], "r" ) ) == NULL ) {
        fprintf(stderr, "No puedo abrir el fichero de parametros.\n");
        exit(ERROR);
    }

    if( ( fentr = fopen( argv[3], "r" ) ) == NULL )
	{
        fprintf(stderr,"No se puede Abrir el Fichero de Entrenamiento: %s\n",argv[3]);
	    exit(-1);
	}

    /*********** Inicializar DSP's **************/
    InicializarPCIC44( "cm.out", "mao.out" );

    /*********** Extraer datos Fichero parametros CM ************/
    ObtenerParametrosCM();

    /*********** Extraer datos Fichero de BMP *************/
    ObtenerTamanyoImagenBMP();

    /*********** Extraer datos Fichero Patrones de Entenamiento *******/
    ObtenerPatronesEntrenamiento();


    /***************** Inicio del funcionamiento es los DSPs *********************/

    printf("Darles paso a los dos procesadores\n");
    tIni = clock();

    PonerValorSemaforo( 7, ROJO );

    Lim[2] = (int)TamVY;
    Lim[3] = (int)TamVX;
    for( y=0 ; y< altoBMP ; y+= (int)TamVY) {
        Lim[0] = y;
        for( x=0 ; x< anchoBMP ; x += (int)TamVX ) {
            Lim[1] = x;

            ObtenerVentana( Lim );
            PonerValorSemaforo( 0, VERDE );
            while( ObtenerValorSemaforo( 0 ) == VERDE );


            /* Lectura de CM's ­­­Comentar para la medicion de tiempos!!! */
            /*
            LeerBloqueFlotantes( BASE, NumPlanos, cm );
            for(j=0;j<NumPlanos;j++)
                printf("%.10f ",cm[j]);
            printf("\n");
            */

            PonerValorSemaforo( 1, VERDE );
            while( ObtenerValorSemaforo( 1 ) == VERDE );

        }
    }

    tFin = clock();
    tiempo = (float) ((tFin - tIni)/CLOCKS_PER_SEC);
    printf("Tiempo de Procesamiento: %.5f seg.\n", tiempo);

    numRes = (int) ((altoBMP / TamVY) * (anchoBMP / TamVX));
    for( j=0 ; j<numRes ; j++ ) {
        while( ObtenerValorSemaforo( 7 ) == ROJO );
        LeerBloqueFlotantes( BASE + (3 * (j % 50)), 3, resultados);
        printf("%03d Vec: %02d  Dist: %f  Etiq: %d\n", j, (int) resultados[0], resultados[1], (int) resultados[2]);
        if( (j % 50) == 49 )
            PonerValorSemaforo( 7, ROJO );
    }

    printf("Fin Ejecucion.\n");

    CerrarYSalir( 0 );

}
