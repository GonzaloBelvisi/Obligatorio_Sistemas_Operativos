# Gestor de Memoria para Sistemas Operativos

Este proyecto implementa un gestor de memoria utilizando el algoritmo de First Fit para asignar bloques de memoria en un mapa de bits (bitmap). Es una simulación de cómo se podría gestionar la memoria en un sistema operativo.

## Funcionalidad

El programa permite:

- Crear un mapa de bits que representa la memoria.
- Asignar bloques de memoria de un tamaño específico en la primera posición disponible (First Fit).
- Liberar bloques de memoria asignados previamente.
- Imprimir el estado actual del mapa de bits en la consola.

Utiliza una función de ajuste primero (First Fit) para encontrar el primer bloque de bits disponibles que sea lo suficientemente grande para satisfacer una solicitud de asignación de memoria. Este enfoque es común en sistemas operativos para la gestión eficiente de la memoria.

## Integrantes

- Diego Barrios
- Juan Manuel Sanchez
- Gonzalo Belvisi

## Materia

Sistemas Operativos

## Universidad

Universidad de Montevideo

## Uso del Programa

El programa se compila y ejecuta en un entorno de línea de comandos. Proporciona opciones para probar diferentes aspectos del gestor de memoria, incluyendo asignación, liberación y visualización del estado de la memoria.
