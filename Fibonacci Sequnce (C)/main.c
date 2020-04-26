#include <stdio.h>

int main()
{
    int n1 = 0;
    int n2 = 1;

    int nn = n1 + n2;
    for (int i = 0; i < 22; i++)
    {
        printf("%d\n", nn);
        n1 = nn;
        nn = n1 + n2;
        printf("%d\n", nn);
        n2 = nn;
        nn = n1 + n2;
    }
    
}