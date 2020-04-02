#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//This is a calculator at its most basic level, but math.h is included because more features will be added in the near future


int n1;
int n2;
char calctype;

int askForNums()
{

    printf("Give me the numbers in the order you want them: ");
    scanf("%d", &n1);
    scanf("%d", &n2);
    return n1, n2;

}


void Addition()
{
    askForNums();
    printf("The result is: %d\n", n1 + n2);
}

void Subtraction()
{
    askForNums();
    printf("The result is: %d\n", n1 - n2);
}
void Multiplication()
{
    askForNums();
    printf("The result is: %d\n", n1 * n2);
}

void Division()
{
    askForNums();
    printf("The result is: %d\n", n1 / n2);
}

void Exit()
{
    printf("Exiting...\n");
    exit(0);
}

char checkInput()
{
    switch (calctype)
    {
        case '1':
            Addition();
            break;
        case '2':
            Subtraction();
            break;
        case '3':
            Multiplication();
            break;
        case '4':
            Division();
            break;
        case 'q':
            Exit();
        default:
            break;
       
    }
}
void PrintMenu()
{
    printf("Welcome to my calculator!\n");
    printf("Please say the calculation you want to do:\n");
    printf("1. Addition\n");
    printf("2. Subtraction\n");
    printf("3. Multiplication\n");
    printf("4. Division\n");
    printf("Press q to exit.\n");
    scanf("%c", &calctype);
    checkInput();
}



int main()
{
    while (calctype != 'q')
    {
        PrintMenu();
    }
}
