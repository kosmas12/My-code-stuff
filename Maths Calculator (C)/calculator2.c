#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

static float help(float a, float b);

static float quit(float a, float b);

const float rads_per_degree = 0.01745329252;

float rads;

typedef struct {
    const char command;
    const char* description;
    bool use_default_input;
    float(*handler)(float a, float b);
}MathOperation;

static float addition(float a, float b) { return a + b; }

static float multiplication(float a, float b) { return a * b; }

static float division(float a, float b) { return a / b;}

static float subtraction(float a, float b) { return a - b;}

static float cosD(float a, float b) {rads = a * rads_per_degree; return cos(rads);}

static float cosR(float a, float b) { return cos(a); }

static float tanD(float a, float b) {rads = a * rads_per_degree; return tan(rads);}

static float tanR(float a, float b) { return cos(a); }

static float sinD(float a, float b) {rads = a * rads_per_degree; return sin(rads);}

static float sinR(float a, float b) { return cos(a); }

static float squareRoot(float a, float b) { return sqrt(a); }

static float power(float a, float b) {return pow(a, b);}

static MathOperation operations[] = {
  { .command='+', .description="Addition", .use_default_input=true, .handler=addition },
  { .command='*', .description="Multiplication", .use_default_input=true, .handler=multiplication },
  { .command='/', .description="Division", .use_default_input=true, .handler=division },
  { .command='-', .description="Subtraction", .use_default_input=true, .handler=subtraction },
  { .command='r', .description="Square root of result", .use_default_input=false, .handler=squareRoot},
  { .command='s', .description="Sin with angle in degrees", .use_default_input=true, .handler=sinD},
  { .command='S', .description="Sin with angle in radians", .use_default_input=true, .handler=sinR},
  { .command='c', .description="Cos with angle in degrees", .use_default_input=true, .handler=cosD},
  { .command='C', .description="Cos with angle in radians", .use_default_input=true, .handler=cosR},
  { .command='t', .description="Tan with angle in degrees", .use_default_input=true, .handler=tanD},
  { .command='T', .description="Tan with angle in radians", .use_default_input=true, .handler=tanR},
  { .command='q', .description="Quit", .use_default_input=false, .handler=quit },
  { .command='p', .description="Raise to the power of", .use_default_input=true, .handler=power},
  { .command='h', .description="This help page", .use_default_input=false, .handler=help}
};

static float quit(float a, float b) { printf("Exiting...\n"); exit(0);}

static float help(float a, float b)
{
    for(int i = 0; i < ARRAY_SIZE(operations); i++) 
    {
        MathOperation* o = &operations[i];
        printf("%c: %s\n", o->command, o->description);
    }
    printf("Also note that you start with 0 on the result so your first operation will work with 0 and your number. ");
    return a;
}



int main()
{
    char user_command = '\0';
    float user_input = 1.0f;
    float result = 0.0f;
    while (user_command != 'q')
    {   
        printf("Please tell me your desired calculation type. For help enter h.\n");
        scanf(" %c", &user_command);
        for(int i = 0; i < ARRAY_SIZE(operations); i++) 
        {
            MathOperation* o = &operations[i];
            if (o->command == user_command)
            {
                if(o->use_default_input == true)
                {
                    printf("Please give me the number you want to use for the operation. ");
                    scanf(" %f", &user_input);
                }
                result = o->handler(result, user_input);
                printf("Result is %f. Please tell me your next calculation.\n", result);
            }
        }

    }
    
}