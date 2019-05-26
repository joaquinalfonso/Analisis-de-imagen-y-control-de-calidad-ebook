# Implantación del sistema

## 1 Objetivo del proyecto

Queremos realizar un sistema que detecte fallos o defectos en tejidos por medio de un sistema informatizado. La ejecucion del mismo se llevara a cabo en entornos industriales textiles, en fabricas de tejidos, en fabricas de ropa, etc... en cualquier industria donde empleen tejidos como materia prima.

El sistema realiza una inspeccion sobre los tejidos, debe ser capaz de decidir si una tela tiene o no defectos y debe actuar sobre el proceso en un tiempo razonablemente bajo.

Uno de los objetivos principales del sistema es la reduccion de costes. Por una parte un proceso de inspeccion automatico permite liberar puestos de trabajo del personal encargado en esta tarea, y por otra parte la deteccion de errores o defectos en las telas en una fase temprana de la produccion evita las fases posteriores de fabricacion, evitando asi desechar o infravalorar el producto final por defectos en las materias primas.

La restriccion temporal es fundamental para que el sistema sea rentable, una clasificacion de tejidos lenta dificulta su implantacion en la empresa.

Este sistema puede ser la base para desarrollar un control de calidad en la fabricación de tejidos y/o prendas textiles.

## 2 Descripción del Sistema

Las tareas que debe desempeñar el sistema son las siguientes:

* Adquisicion de informacion relevante de las telas.
* Procesamiento de los datos adquiridos.
* Manejo de los actuadores que intervengan en el proceso.

    ![](./images/imp_1.png "")

Para describir completamente el sistema que queremos implantar lo dividiremos en sus componentes físicas y lógicas, es decir la parte Hardware y la parte Software. La parte Hardware nos proporcionará la interactuación tanto a la entrada como a la salida del Sistema. La parte Software nos permitirá el tratamiento de la información adquirida así como la toma de decisiones en cada situación.

### 2.1 Subsistema Hardware o Físico

Los componentes básicos de la parte Física o Hardware del sistema serán los siguientes:

![](./images/imp_2.png "")

* **Módulo de Adquisión de Imágenes.**

    El Módulo de Adquisición de Imágenes nos debe proporcionar las imágenes que analizaremos para determinar si contienen algún defecto. Constará de una cámara de vídeo para la adquisición de las mismas, así como una tarjeta capturadora de señal para la digitalización de las imágenes para su posterior tratamiento.

* **Módulo de Procesamiento.**

    El Módulo de Procesamiento es el que se encargará del análisis de las imágenes obtenidas por el subsistema anterior y nos proporcionara la decisión de si la imagen procesada contiene o no errores. Estara formado por un PC de ultima generación (PII o PIII) que monitorice el sistema así como diferentes tarjetas hardware para el prodeco digital de señales, ya que las necesidades de procesamiento son bastante elevadas en nuestra apliación final. Este modulo será el encargado de sostener todo el sistema Lógico o Software, que mas adelante describiremos en detalle.

* **Módulo de Posicionamiento/Actuación.**

    El Módulo de Posicionamiento/Actuación es el encargado de proporcionar las Salidas del sistema, será el encargado de marcar aquellas zonas de las telas que consideremos como defectuosas, tomará los resultados del procesamiento de la imagen y si esta tiene algún defecto, se situará sobre el mismo y lo marcara para su posterior descarte.

    ![](./images/imp_3.png "")

### 2.2 Subsistema Software o Lógico

El Subsistema Software o Lógico también lo podemos dividir en varios módulos que realizarán una función determinada dentro del procesamiento total de la imagen. Como hemos mencionado antes este subsistema ira implantado en el Modulo de procesamiento de la parte Física. El siguiente esquema muestra las diferentes  etapas en el procesamiento de las imágenes:

* **Adquisición / Procesamiento de la Imagen.**

    Esta etapa será la encargada de la adquisición de la imagen y su posterior tratamiento para la extracción de las características que serán relevantes para la posterior clasificación de la misma.

    El método de extracción de características que utilizaremos será el **Coeficiente Morfológico (CM)** que nos proporciona una medida las “formas” que contiene la imagen. El hecho de elegir este algoritmo de procesamiento se debe fundamentalmente a sus caracteristicas de invarianza respecto a rotaciones y traslaciones de las imágenes.

    Este algoritmo consiste en la división de la imagen en diferentes planos de intensidades y aplicarles a cada plano una serie de máscaras de diferentes tamaños (2x2, 3x3, ...) para comprobar el numero de veces que la mascara cabe en cada plano. Con ello conseguimos una discriminación en zonas de las diferentes partes de la imagen.

    Si conseguimos discriminar claramente los planos donde podemos encontrar los errores de los planos que contienen las partes correctas de la imagen, obtendremos una información más precisa  para la etapa de clasificación.

    Nuestro sistema se supone que será parametrizable, porque los valores de los diferentes planos y los tamaños de las mascaras que usemos vendrán determinados por el tipo de piezas que queramos analizar. Para proporcionárselos necesitaremos de un estudio previo realizado por un experto; es decir, que nuestro sistema, tanto en la parte de análisis como en la parte de clasificación seguirá un esquema de aprendizaje SUPERVISADO, necesitando de la ayuda de un “experto” humano para determinar los diferentes parámetros de funcionamiento.

* **Clasificación de los Patrones.**

    Dado que la salida de la fase de Análisis nos proporciona un vector de dimensión n (dependiente del nº de planos) usaremos un método de **clasificación por mínima distancia**. La distancia utilizada será la **distancia Mao** que nos proporciona una medida del ángulo que forman los 2 vectores que pretendemos comparar.

    Para poder clasificar cada una de las imágenes debemos disponer de una Base de Datos de Imágenes previamente analizadas y correctamente clasificadas que será con la que vayamos comparando las imágenes de entrada para determinar a que componente de la Base de Datos se asemeja más.

    En la creación de la base de datos también interviene de nuevo la parte humana, no podemos crear automáticamente una base de datos CORRECTA sobre una tela nueva, por lo tanto necesitamos de una SUPERVISION de un operador para la construcción de la Base de Datos que usaremos para la clasificación de las telas.

* **Posicionamiento del Actuador.**

    Una vez realizada la fase de clasificación ya conocemos si la imagen procesada contiene o no errores y la posición de la imagen que los contiene. Por lo tanto necesitamos un modulo que maneje el actuador y lo lleve a la posición adecuada para marcar la pieza de tela. La posición vendrá determinada de forma relativa por las coordenadas de la imagen. 

    Este modulo debe convertir las coordenadas de la imagen a coordenadas del mundo real. Esto es relativamente sencillo porque las condiciones de trabajo del sistema son conocidas y fijas, es decir la cámara no varia ni su posición, ni sus parámetros de enfoque, por lo tanto conocidas estas podemos convertir de coordenadas de la imagen a coordenadas del actuador.

Debido a la estructuración y a la modularidad con la que esta construido el sistema resulta muy sencillo la inclusión y el tratamiento de nuevos tipo de telas, pero siempre siguiendo las indicaciones antes comentadas tanto para la parametrización del calculo del Coeficiente Morfológico como para la creación de la Base de Datos de Patrones ya que la utilización de un EXPERTO para realizar estas tareas es imprescindible debido a que este sistema se basa en las características de **Aprendizaje Supervisado**.

## 3 Requerimientos del sistema

Según la descripcion planteada anteriormente los requerimientos del sistema seran los siguientes.

### 3.1 Requerimientos Hardware

Adquisición:

* Camara CCD (escala de grises): permite la toma de imágenes de los tejidos.

* Tarjeta Frame-Grabber: para la captura y digitalizacion del las señales analogicas de video proporcionadas por la camara CCD.

Procesamiento:

* Tarjeta de procesamiento digital de señal: Al menos debe incluir dos procesadores DSP que trabajen en paralelo para realizar el procesamiento a alta velocidad de las imágenes capturadas. Un DSP se encarga de calcular el coeficiente morfologico de la imagen y el otro DSP realiza la clasificacion de los patrones obtenidos.

* PC para monitorizacion del proceso: Debe coordinar las acciones conjuntas del sistema. Controla la adquisicion y procesamiento de las imágenes así como los actuadores del sistema.

Actuación:

* Un posicionador: Mecanismo para relizar el desplazamiento a la zona de la tela que contiene defecto y realizar el marcado de la misma para su posterior eliminacion despues del corte.

    Motores y rodillos para el desplazamiento de las telas: Deben tener interface de conexión con el PC que permitan su control a distancia.

### 3.2 Requerimientos Software

Atendiendo a la descirpción hecha del susbsistema lógico necesitaremos los siguientes componentes software:

* Librerías de captura de imagen incluidas con la tarjeta de captura de video.

* Librerías de control y programación de la tarjeta de procesamiento digital de señal (DSP).

* Librerías de manipulación de los actuadores para posicionamiento final.

* Aplicación personalizada (ad hoc) para el control de calidad sobre material textil.

* Sistema Operativo Windows 98 necesario para el soporte del sistema.

## 4 Entorno de Implantación

Para describir el entorno donde debería implantarse el sistema para obtener el máximo rendimiento, vamos a dividir la situación del sistema en 2 partes:

* Localización física

* Distribución del sistema

### 4.1 Localización Física

Lógicamente el sistema va a estar situado en una determinada habitación o sala perteneciente a una empresa de la industria textil.

Para asegurarnos de que el entorno donde va a estar situado el sistema no influya (o por lo menos su influencia sea mínima) en él, lo ideal sería que estuviese localizado en una sala cerrada en la que no se produzcan variaciones de luz. De lo que se trata es de conseguir un ambiente fijo que permita al sistema trabajar casi siempre bajo las mismas condiciones de forma que su rendimiento sea óptimo. Es por ello por lo que la iluminación de la sala en la que se situe el sistema debe ser siempre la misma.

En la misma línea, sería también conveniente asegurar que no se van a producir grandes cambios en el entorno puesto que el sistema habrá sido adiestrado bajo unas determinadas circunstancias. El entorno deberá reproducir de la forma más fiel posible dichas circunstancias.

### 4.2 Distribución del sistema

En lo que a la posición de los distintos elementos que conforman el sistema se refiere, lo mejor es realizar un dibujo ilustrativo y sobre él, dar las explicaciones oportunas:

![](./images/imp_4.png "")

Vista superior del sistema

![](./images/imp_5.png "")

Vista lateral del sistema

## 5 Interface del Operador

A la hora de desarrollar el interface del operador, podemos dividirlo en 2 partes claramente diferenciadas, a saber:

* Tarea de Supervisión
* Tarea de Mantenimiento

### 5.1 Supervisión

Dentro de la tarea de supervisión, el operador del sistema será el encargado de poner en funcionamiento el entorno para empezar a detectar defectos así como de detenerlo una vez se haya concluido la jornada laboral (si procede. El sistema puede programarse para detenerse a un hora todos los días).

Del mismo modo, y como su propio nombre indica, el operador debe supervisar las distintas clasificaciones (defectuoso o no defectuoso) que realiza el programa para asegurarse de que el sistema funciona correctamente y no esta clasificando de un modo incorrecto. No se trata de ir comprobando una por una cada una de las telas que aparecen en la pantalla del PC puesto que, en este caso, no supondría ninguna ventaja para la empresa el hecho de automatizar la detección de defectos. El operador solamente debe asegurarse que la tasa de error del entorno esta dentro del margen previamente obtenido.

Esta tasa de error se ha obtenido como resultado de desarrollar una gran variedad y cantidad de experimentos variando no sólo los tipos de tela a analizar sino también los distintos parámetros que conforman tanto el algoritmo de cálculo del CM (nº de planos y nº de piezas) como el clasificador que se utiliza para diferenciar las telas defectuosas de las demás. 

En definitiva, el encargado de realizar esta tarea puede ser cualquier trabajador de la empresa ya que no se requiere ningún tipo de conocimiento técnico especifico al respecto, mas aún cuando la sencillez del interface que se plantea a continuación queda patente. En lo que a los menús de pantalla se refiere, las opciones que tendría son:

1. Seleccionar Tipo de Tela
2. Iniciar Sistema
3. Finalizar Sistema
4. Detener Sistema
5. Ver Estadísticas

Para una mayor claridad y comprensión, vamos a comentar las distintas opciones:

#### **Pantalla Principal:**

Se compone de un menú con 5 opciones en la parte izquierda de la pantalla y, en la parte derecha, la última imagen capturada dividida en ventanas con la clasificación correspondiente de cada una.

![](./images/imp_6.png "")

Las ventanas tachadas con X indican que esas zonas de la imagen (de la tela) han sido clasificadas como defectuosas. El resto se consideran ventanas sin defectos.

#### **Menús:**

* **Seleccionar Tipo Tela:** Te permite seleccionar el tipo de tela que va a ser analizado por el sistema. Lógicamente, para poder poner en funcionamiento el sistema con la opción “Iniciar Sistema”, es necesario que se halla seleccionado alguna tela. Por defecto el sistema podría seleccionar el último tipo de tela analizado.

* **Iniciar Sistema:** Pone en funcionamiento el sistema de detección de defectos: activa la cinta transportadora, prepara la cámara para la captura de imágenes, prepara el brazo robot y lo coloca en una posición inicial, etc.

* **Finalizar Sistema:** Cierra todos los dispositivos que estén activos en el proceso del sistema y apaga el sistema: principalmente detiene la cinta transportadora y apaga el PC que controla el resto de dispositivos.

* **Detener Sistema:** Permite detener (realizar una pausa) el sistema sin llegar a apagarlo. Esta opción será utilizada cuando se produzca alguna circunstancia anormal, esto es, que el sistema falle mucho más de lo esperado, que siempre clasifique como correcta aunque hayan telas defectuosas, etc. Esto supone detener la cinta transportadora.

* **Ver Estadísticas:** Te permite visualizar una serie de estadísticas y gráficos de funcionamiento del sistema. En ellas se incluyen algunos aspectos como tasa de error, tiempos empleados, etc. Esta opción presupone que será necesario mantener una base de datos donde se almacenen los diferentes aspectos.

### 5.2 Mantenimiento

A diferencia de la tarea anterior, el encargado de llevarla a cabo debe ser un técnico experto puesto que las actividades de esta tarea son de vital importancia para el correcto funcionamiento del sistema.

Las actividades que la componen podrían ser:

* **Nuevo tipo de tela a reconocer:**

    En este caso, el experto debe construir una base de datos con imágenes defectuosas y correctas del tipo de tela en cuestión que se utilizará para realizar la clasificación. El proceso de creación de una base de conocimiento es el siguiente:

    Se obtienen alrededor de 10 imágenes de las cuales una será totalmente buena (trozo de tela sin defectos) y las 9 restantes tendrán algún tipo de defecto (agujero, rasgado, machado, roto, etc.). cada imagen es dividida en ventanas de tamaño fijo (generalmente la imagen se divide en 64 ventanas cada una con un tamaño de 64x64 pixeles). De cada una de las ventanas de cada imagen se obtiene su Coeficiente Morfológico (CM). Una vez que tenemos todos los CM de las distintas imágenes, la base de datos estará formada por:

    * Los 64 CM de la imagen sin defectos

        * Del resto de imágenes (las defectuosas) se cogen los CM de aquellas ventanas donde hayan defectos. Obviamente, los defectos no están localizados en toda la imagen sino en pequeñas áreas de la misma. Por eso si cogiésemos todas las imágenes defectuosas al completo, estaríamos creando una base de datos con CM repetidos muchas veces. Y en realidad lo que interesa es tener una base de datos con una gran variedad de defectos.

* **Nuevo tipo de defecto de una tela:**

    Este tarea es muy sencilla de realizar (siempre por el experto) ya que simplemente hay que añadir a la B.D el CM de la ventana que contiene el nuevo defecto.

También hay que señalar que estas opciones van a suponer cambios internos del sistema. Por un lado, el hecho de incorporar una nueva tela a analizar supondrá que en la opción “Seleccionar Tela” del menú se agregue como nueva posibilidad la tela. Por otro lado, tanto cuando se introduce una nueva tela como cuando se introduce un nuevo defecto, va a ser necesario recalcular la tasa de error del sistema.

## 6 Presupuesto

### 6.1 COMPONENTES HARDWARE

|Componente|Precio|
|:---|---:|
|Camara B/W CCD HTC-381|28.995 pts|
|Tarjeta capturadora de señal de video – framegrabber LSI PLC modelo MDC40IM | 695.000 pts|
|Tarjeta de procesamiento digital de señal (DSP) con dos procesadores Texas Instruments C44 modelo PCI/C44S	| 1.350.000 pts |
| Pentium III 450 MHz, 128MB RAM, 8,6GB HD, Monitor Sony 17’ Ratón, Teclado | 195,000 pts| 
| Posicionador | ? |
| Total Componentes Hardware | 2.268.995 pts |

### 6.2 COMPONENTES SOFTWARE

|Componente|Precio|
|:---|---:|
|Windows 98|14.995 pts|
|Aplicación personalizada para control de calidad de material textil|500.000 pts|
|Mantenimiento (1 año)|150.000 pts|
| Total Componentes Software + 1 año mantenimiento | 664.995 pts |

Librerías específicas de cada tarjeta incluidas	
