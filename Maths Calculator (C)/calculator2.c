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

static float addition(float a, float b) { return a + b; }

static float multiplication(float a, float b) { return a * b; }

static float division(float a, float b) {return a / b;}

static float subtraction(float a, float b) {return a - b;}

static float quit(float a, float b) {exit(0);}

static float help(float a, float b)
{
    for(int i = 0; i < ARRAY_SIZE(operations); i++) 
    {
        MathOperation* o = &operations[i];
        printf("%c: %s\n", o->command, o->description);
    }
}

static MathOperation operations[] = {
  { .command='+', .description="Addition", .inputs=2, .handler=addition },
  { .command='*', .description="Multiplication", .inputs=2, .handler=multiplication },
  { .command='/', .description="Division", .inputs=2, .handler=division },
  { .command='-', .description="Subtraction", .inputs=2, .handler=subtraction },
  { .command='q', .description="Quit", .inputs=0, .handler=quit },
  { .command='h', .description="This help page", .inputs=0, .handler=help}
};


int main()
{
    char user_command = '\0';
    while (user_command != 'q')
    {   
        float user_input = 0.0f;
        float result = 0.0f;
        
        printf("Please tell me your desired calculation type. For help enter h");
        scanf(" %c", &user_command);
        for(int i = 0; i < ARRAY_SIZE(operations); i++) 
        {
            if (o->command == user_command)
            {
                result = o->handler(result, user_input);  
            }
        }

    }
    
}