# Proyecto Sistemas Informáticos. Universidad de Alicante Curso 1998-1999

## Autores

* ### José Francisco Galiana Molina
* ### José Manuel Rocamora Bernabéu
* ### Juan José Lorenzo Quintanilla
* ### Joaquín Alfonso González

# Análisis de imagen y control de calidad

## 1 Introducción

* [1.1 Necesidad del Control de Calidad y Automatización en la Industria](01%20Introducción/1.1%20Necesidad%20del%20Control%20de%20Calidad%20y%20Automatización%20en%20la%20Industria.md)
  * [1.1.1 El concepto de CALIDAD](01%20Introducción/1.1%20Necesidad%20del%20Control%20de%20Calidad%20y%20Automatización%20en%20la%20Industria.md#111-el-concepto-de-calidad)
  * [1.1.2 El tratamiento de la Calidad en las Empresas](01%20Introducción/1.1%20Necesidad%20del%20Control%20de%20Calidad%20y%20Automatización%20en%20la%20Industria.md#112-el-tratamiento-de-la-calidad-en-las-empresas)
  * [1.1.3 Normativas de la serie ISO 9000](01%20Introducción/1.1%20Necesidad%20del%20Control%20de%20Calidad%20y%20Automatización%20en%20la%20Industria.md#113-normativas-de-la-serie-iso-9000)
* [1.2 Tipos de Sistemas de Control y Automatización de Producción](01%20Introducción/1.2%20Tipos%20de%20Sistemas%20de%20Control%20%20y%20Automatización%20de%20Producción.md)
  * [1.2.1 Según el tipo de industria](01%20Introducción/1.2%20Tipos%20de%20Sistemas%20de%20Control%20%20y%20Automatización%20de%20Producción.md#121-según-el-tipo-de-industria)
  * [1.2.2 Según la metodología](01%20Introducción/1.2%20Tipos%20de%20Sistemas%20de%20Control%20%20y%20Automatización%20de%20Producción.md#122-según-la-metodología)
  * [1.2.3 Según la plataforma de implementación](01%20Introducción/1.2%20Tipos%20de%20Sistemas%20de%20Control%20%20y%20Automatización%20de%20Producción.md#123-según-la-plataforma-de-implementación)
  * [1.2.4 DSP (Procesamiento digital de la señal)](01%20Introducción/1.2%20Tipos%20de%20Sistemas%20de%20Control%20%20y%20Automatización%20de%20Producción.md#124-dsp-procesamiento-digital-de-la-señal)
* [1.3 Sistemas de visión artificial para el control de producción en fabricación](01%20Introducción/1.3%20Sistemas%20de%20visión%20artificial%20para%20el%20control%20de%20producción%20en%20fabricación.md)
  * [1.3.1 Análisis de Texturas aplicado al Control de Calidad Industrial mediante Visión Artificial](01%20Introducción/1.3%20Sistemas%20de%20visión%20artificial%20para%20el%20control%20de%20producción%20en%20fabricación.md#131-análisis-de-texturas-aplicado-al-control-de-calidad-industrial-mediante-visión-artificial) 
  * [1.3.2 Análisis de la Forma de Semillas Malas Usando la Transformada de Fourier](01%20Introducción/1.3%20Sistemas%20de%20visión%20artificial%20para%20el%20control%20de%20producción%20en%20fabricación.md#132-análisis-de-la-forma-de-semillas-malas-usando-la-transformada-de-fourier)
  * [1.3.3 Clasificación de Maderas por Visión Artificial](01%20Introducción/1.3%20Sistemas%20de%20visión%20artificial%20para%20el%20control%20de%20producción%20en%20fabricación.md#133-clasificación-de-maderas-por-visión-artificial)
  * [1.3.4 Inspección de Vidrio Laminado en Tiempo Real](01%20Introducción/1.3%20Sistemas%20de%20visión%20artificial%20para%20el%20control%20de%20producción%20en%20fabricación.md#134-inspección-de-vidrio-laminado-en-tiempo-real)

## 2 Programación sobre:
* [2.1 La tarjeta QPC/C40S + el modulo MDC40IM](02%20Programación%20sobre/2.1%20La%20tarjeta%20QPCC40S%20el%20modulo%20MDC40IM.md)
  * [2.1.1 El procesador TMS320C40](02%20Programación%20sobre/2.1%20La%20tarjeta%20QPCC40S%20el%20modulo%20MDC40IM.md#211-el-procesador-tms320c40)
  * [2.1.2 El módulo de captura de imágenes MDCC40IM](02%20Programación%20sobre/2.1%20La%20tarjeta%20QPCC40S%20el%20modulo%20MDC40IM.md#212-el-módulo-de-captura-de-imágenes-mdcc40im)
* [2.2 Tarjeta multiprocesadora PCI/C44S](02%20Programación%20sobre/2.2%20Tarjeta%20multiprocesadora%20PCIC44S.md)
  * [2.2.1 Componentes de la tarjeta](02%20Programación%20sobre/2.2%20Tarjeta%20multiprocesadora%20PCIC44S.md#221-componentes-de-la-tarjeta)
* [2.3 Ejemplos](02%20Programación%20sobre/2.3%20Ejemplos.md)

## 3 El algoritmo del coeficiente morfológico
* [3.1 Introducción](03%20El%20algoritmo%20del%20coeficiente%20morfológico/3.1%20Introducción.md)
* [3.2 Funcionamiento del algoritmo](03%20El%20algoritmo%20del%20coeficiente%20morfológico/3.2%20Funcionamiento%20del%20algorítmo.md)
* [3.3 Ejemplos](03%20El%20algoritmo%20del%20coeficiente%20morfológico/3.3%20Ejemplos.md)

## 4 Algoritmos de clasificación de patrones

* [4.1 Introducción](04%20Algoritmos%20de%20clasificación%20de%20patrones/4.1%20Introducción.md)
* [4.2 Basado en distancia euclidea](04%20Algoritmos%20de%20clasificación%20de%20patrones/4.2%20Basado%20en%20distancia%20euclidea.md)
* [4.3 Basado en distancia MAO](04%20Algoritmos%20de%20clasificación%20de%20patrones/4.3%20Basado%20en%20distancia%20MAO.md)
* [4.4 Ejemplos](04%20Algoritmos%20de%20clasificación%20de%20patrones/4.4%20Ejemplos.md)

## 5 Comunicación entre tarjetas

* [5.0 Introducción](05%20Comunicación%20entre%20tarjetas/5.0%20Introducción.md)
* [5.1 Comunicación PC y DSP](05%20Comunicación%20entre%20tarjetas/5.1%20Comunicación%20PC%20y%20DSP.md)
* [5.2 Comunicación InterDSP](05%20Comunicación%20entre%20tarjetas/5.2%20Comunicación%20InterDSP.md)
* [5.3 Comunicación DSP y otro Periférico (otra tarjeta)](05%20Comunicación%20entre%20tarjetas/5.3%20Comunicación%20DSP%20y%20otro%20Periférico%20(otra%20tarjeta).md)

## 6 Batería de programas

* [6.0 Introducción](06%20Batería%20de%20programas/6.0%20Introducción.md)
* [6.1 Distribución de los programas](06%20Batería%20de%20programas/6.1%20Distribución%20de%20los%20programas.md)
* [6.2 Parámetros de los programas](06%20Batería%20de%20programas/6.2%20Parámetros%20de%20los%20programas.md)
* [6.3 Ejecución en el procesador principal](06%20Batería%20de%20programas/6.3%20Ejecución%20en%20el%20procesador%20principal.md)
* [6.4 Ejecución en la tarjeta MDC40IM](06%20Batería%20de%20programas/6.4%20Ejecución%20en%20la%20tarjeta%20MDC40IM.md)
* [6.5 Ejecución en la tarjeta PCI/C44S](06%20Batería%20de%20programas/6.5%20Ejecución%20en%20la%20tarjeta%20PCI%20C44S.md)
  * [6.5.1 Versión Secuencial](06%20Batería%20de%20programas/6.5%20Ejecución%20en%20la%20tarjeta%20PCI%20C44S.md#651-versión-secuencial)
  * [6.5.2 Versión Paralela](06%20Batería%20de%20programas/6.5%20Ejecución%20en%20la%20tarjeta%20PCI%20C44S.md#652-versión-paralela)
* [6.6 Ejecución conjunta en las dos tarjetas](06%20Batería%20de%20programas/6.6%20Ejecución%20conjunta%20en%20las%20dos%20tarjetas.md)
* [6.7 Resultados obtenidos en la ejecución de todos los programas](06%20Batería%20de%20programas/6.7%20Resultados%20obtenidos%20en%20la%20ejecución%20de%20todos%20los%20programas.md)

## 7 Experimentos realizados
* [7.0 Introducción](07%20Experimentos%20realizados/7.0%20Introducción.md)
* [7.1 Preparación de experimentos](07%20Experimentos%20realizados/7.1%20Preparación%20de%20experimentos.md)
* [7.2 Creación de las Bases de Datos](07%20Experimentos%20realizados/7.2%20Creación%20de%20las%20Bases%20de%20Datos.md)
* [7.3 Tandas](07%20Experimentos%20realizados/7.3%20Tandas.md)
  * [7.3.1 Tanda 1](07%20Experimentos%20realizados/7.3%20Tandas.md#731-tanda-1)
  * [7.3.2 Tanda 2](07%20Experimentos%20realizados/7.3%20Tandas.md#732-tanda-2)
  * [7.3.3 Tanda 3](07%20Experimentos%20realizados/7.3%20Tandas.md#733-tanda-3)
  * [7.3.4 Tanda 4](07%20Experimentos%20realizados/7.3%20Tandas.md#734-tanda-4)

## 8 Conclusiones
* [8.0 Conclusiones](08%20Conclusiones/8.0%20Conclusiones.md)

## 9 Incidencias
* [9.0 Problemas surgidos durante la realización del proyecto](09%20Incidencias/9.0%20Problemas%20surgidos%20durante%20la%20realización%20del%20proyecto.md)

## 10 Bibliografía
* [10.0 Bibliografía](10%20Bibliografía/10.0%20Bibliografía.md)

## 11 Agradecimientos
* [11.0 Agradecimientos](11%20Agradecimientos/11.0%20Agradecimientos.md)