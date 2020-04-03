#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

static float help(float a, float b);

static float quit(float a, float b);

typedef struct {
    const char command;
    const char* description;
    bool need_input;
    float(*handler)(float a, float b);
}MathOperation;

static float addition(float a, float b) { return a + b; }

static float multiplication(float a, float b) { return a * b; }

static float division(float a, float b) { return a / b;}

static float subtraction(float a, float b) { return a - b;}

static MathOperation operations[] = {
  { .command='+', .description="Addition", .need_input=true, .handler=addition },
  { .command='*', .description="Multiplication", .need_input=true, .handler=multiplication },
  { .command='/', .description="Division", .need_input=true, .handler=division },
  { .command='-', .description="Subtraction", .need_input=true, .handler=subtraction },
  { .command='q', .description="Quit", .need_input=false, .handler=quit },
  { .command='h', .description="This help page", .need_input=false, .handler=help}
};

static float quit(float a, float b) { exit(0);}

static float help(float a, float b)
{
    for(int i = 0; i < ARRAY_SIZE(operations); i++) 
    {
        MathOperation* o = &operations[i];
        printf("%c: %s\n", o->command, o->description);
    }
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
                if(o->need_input == 1)
                {
                    scanf(" %f", &user_input);
                }
                result = o->handler(result, user_input);
                printf("Result is %f. Please tell me your next calculation.\n", result);
            }
        }

    }
    
}