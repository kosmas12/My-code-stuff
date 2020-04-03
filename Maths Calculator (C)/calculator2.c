#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

typedef struct {
        char command;
        const char* description;
        int inputs;
        float(*handler)(float a, float b);
    }MathOperation;

static MathOperation operations[] = {
  { .command='+', .description="Addition", .inputs=2, .handler=addition },
  { .command='*', .description="Multiplication", .inputs=2, .handler=multiplication }
  { .command='/', .description="Division", .inputs=2, .handler=division },
  { .command='-', .description="Subtraction", .inputs=2, .handler=subtraction },
  { .command='h', .description= "This help page", .inputs=0, .handler=help},
  { .command='q', .description="Quit", .inputs=0, .handler=help}
};


static float addition(float a, float b) { return a + b; }

static float multiplication(float a, float b) { return a * b; }

static float division(float a, float b) {return a / b;}

static float subtraction(float a, float b) {return a - b;}

static float help(float a, float b) 
{
    for(int i = 0; i < ARRAY_SIZE(operations); i++) 
    {
        printf("%c: %s\n", o->command, o->description);
    }
}


int main()
{
    while (command != 'q')
    {
        float user_input = 0.0f;
        float result = 0.0f;

        MathOperation* o = &operations[i];
        scanf(" %c", &command);
        for(int i = 0; i < ARRAY_SIZE(operations); i++) 
        {
   
            if (o->command == command)
            {
                result = o->handler(result, user_input);
            }
        }

    }
    
}