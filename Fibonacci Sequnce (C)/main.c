#include <stdio.h>

int main()
{
    __uint64_t n1 = 0;
    __uint64_t n2 = 1;

    __uint64_t nn = n1 + n2;
    for (int i = 0; i < 50; i++)
    {
        printf("%ld\n", nn);
        n1 = nn;
        nn = n1 + n2;
        printf("%ld\n", nn);
        n2 = nn;
        nn = n1 + n2;
    }
    
}