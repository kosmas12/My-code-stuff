#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

static float addition(float* n, int c) { return n[0] + n[1]; }

static float multiplication(float* n, int c) { return n[0] * n[1]; }

static float division(float *n, int c) {return n[0] / n[1];}

static float subtraction(float *n, int c) {return n[0] - n[1];}

static float help(float *n, int c) 
{
    for(int i = 0; i < ARRAY_SIZE(operations); i++) 
    {
        printf("%s: %s", o->command, o->description);
    }
}

typedef struct {
        const char command;
        const char* description;
        int inputs;
        float(*handler)(float* n, int c);
    }MathOperation;

static MathOperation operations[] = {
  { .command='+', .description="Addition", .inputs=2, .handler=addition },
  { .command='*', .description="Multiplication", .inputs=2, .handler=multiplication }
  { .command='/', .description="Division", .inputs=2, .handler=division },
  { .command='-', .description="Subtraction", .inputs=2, .handler=subtraction },
  { .command='h', .description= "This help page", .inputs=0, .handler=help}
};

int main()
{
    MathOperation* o = &operations[i];
    scanf(" %c", &command);
    for(int i = 0; i < ARRAY_SIZE(operations); i++) 
    {   
        if (o->command == command)
        {
            handler = o->handler;
        }
    }
    
}