#ifndef RANDOM_H
#define RANDOM_H

#include <stdlib.h>
#include <time.h>

void DefRandom () //Задаем стартовое число в качестве системного времени
{
    srand((unsigned int)time(0)); 
}

unsigned int Random(int MIN_RAND, int MAX_RAND) //Генерируем случайное число в диапозоне
{
    return rand() % (MAX_RAND - MIN_RAND + 1) + MIN_RAND;
}

int RandomElemArrayInt(int* array, int len)
{
    return *(array + (rand() % len));
}

#endif