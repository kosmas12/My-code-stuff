#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//This is a maths calculator at its most basic level, but math.h is included because more features will be added in the near future


int n1;
int n2;
char calctype = '\0';

void Addition()
{
    printf("The result is: %d\n", n1 + n2);
}

void Subtraction()
{
    printf("The result is: %d\n", n1 - n2);
}
void Multiplication()
{
    printf("The result is: %d\n", n1 * n2);
}

void Division()
{
    printf("The result is: %d\n", n1 / n2);
}

void Exit()
{
    printf("Exiting...\n");
    exit(0);
}

void askForNums()
{
    printf("Also give me the numbers in the order you want them: ");
    scanf("%d", &n1);
    scanf("%d", &n2);
}

char getInput()
{
    scanf(" %c", &calctype);

    if (calctype != 'q')
    {
        askForNums();
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
                break;
            default:
                break;
       
        }
    }
    else
    {
        Exit();
    }
    
}

void PrintMenu()
{
    printf("Welcome to my calculator!\n");
    printf("Please tell me your desired calculation. Remember:\n");
    printf("1. Addition\n");
    printf("2. Subtraction\n");
    printf("3. Multiplication\n");
    printf("4. Division\n");
    printf("Press q to exit.\n");
    getInput();

}

int main()
{
    while (calctype != 'q')
    {
        PrintMenu();
    }
}
