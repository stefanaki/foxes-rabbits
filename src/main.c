#include <stdio.h>
#include "worldgen.h"
#include "cell.h"

int main()
{
    uint32_t x = 10;

    printf("%f\n", r4_uni(&x));

    Cell cell = (Cell){.boardIndex = 10};
    printf("%d\n", cell.boardIndex);
    printf("%d\n", cell.modified);
    printf("%d\n", cell.element.breedingAge);

    printf("hello world\n");
    return 0;
}