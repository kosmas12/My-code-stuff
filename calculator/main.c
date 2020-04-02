#include <stdio.h>
#include <math.h>

//This is a calculator at its most basic level, but math.h is included because more features will be added in the near future


int n1;
int n2;
int calctype;

int askForNums()
    {

        printf("Give me the numbers in order");
        scanf("%d", &n1);
        scanf("%d", &n2);
        return n1, n2;

    }

int main()
{
    

    printf("Welcome to my calculator!\n");
    printf("Please say the calculation you want to do:\n");
    printf("1. Addition\n");
    printf("2. Subtraction\n");
    printf("3. Multiplication\n");
    printf("4. Division\n");

    scanf("%d", &calctype);

    switch (calctype)
    {
    case 1:
        askForNums();
        printf("The result is: %d", n1 + n2);
        break;
    case 2:
        askForNums();
        printf("The result is: %d", n1 - n2);
        break;
    case 3:
        askForNums();
        printf("The result is: %d", n1 * n2);
        break;
    case 4:
        askForNums();
        printf("The result is: %d", n1 / n2);
        break;
    default:
        break;
    }
}
