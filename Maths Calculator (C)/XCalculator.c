#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_gamecontroller.h>
#include <hal/debug.h>
#include <hal/video.h>
#include <windows.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

static float help(float a, float b);

static float quit(float a, float b);

const float rads_per_degree = 0.01745329252;

float rads;

typedef struct {
    const int accessnum;
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

static float tanR(float a, float b) { return tan(a); }

static float sinD(float a, float b) {rads = a * rads_per_degree; return sin(rads);}

static float sinR(float a, float b) { return sin(a); }

static float squareRoot(float a, float b) { return sqrt(a); }

static float power(float a, float b) {return pow(a, b);}

static float reset(float a, float b) {a = 0.0f; return a;}

static MathOperation operations[] = {
  { .accessnum='0', .command='+', .description="Addition", .use_default_input=true, .handler=addition },
  { .accessnum='1', .command='*', .description="Multiplication", .use_default_input=true, .handler=multiplication },
  { .accessnum='2', .command='/', .description="Division", .use_default_input=true, .handler=division },
  { .accessnum='3', .command='-', .description="Subtraction", .use_default_input=true, .handler=subtraction },
  { .accessnum='4', .command='r', .description="Square root of result", .use_default_input=false, .handler=squareRoot},
  { .accessnum='5', .command='s', .description="Sin with angle in degrees", .use_default_input=true, .handler=sinD},
  { .accessnum='6', .command='S', .description="Sin with angle in radians", .use_default_input=true, .handler=sinR},
  { .accessnum='7', .command='c', .description="Cos with angle in degrees", .use_default_input=true, .handler=cosD},
  { .accessnum='8', .command='C', .description="Cos with angle in radians", .use_default_input=true, .handler=cosR},
  { .accessnum='9', .command='t', .description="Tan with angle in degrees", .use_default_input=true, .handler=tanD},
  { .accessnum='10', .command='T', .description="Tan with angle in radians", .use_default_input=true, .handler=tanR},
  { .accessnum='11', .command='p', .description="Raise to the power of", .use_default_input=true, .handler=power},
  { .accessnum='12', .command='e', .description="Reset result", .use_default_input=false, .handler=reset},
  { .accessnum='13', .command='q', .description="Quit", .use_default_input=false, .handler=quit },
  { .accessnum='14', .command='h', .description="This help page", .use_default_input=false, .handler=help}
};

static float quit(float a, float b) { debugPrint("Exiting...\n"); exit(0);}

static float help(float a, float b)
{
    for(int i = 0; i < ARRAY_SIZE(operations); i++) 
    {
        MathOperation* o = &operations[i];

        debugPrint("%c: %s\n", o->command, o->description);
    }
    printf("Also note that you start with 0 on the result so your first operation will work with 0 and your number.\n");
    printf("Press B to add 10 to the operation, X to add 100, and Y to add 1000");
    return a;
}



int main()
{
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);
    SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO); //Initialize SDL for consoles
    SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");
    int user_command = 0;
    float user_input = 1.0f;
    float result = 0.0f;
    const int deadzone = -8000;
    int xdir;
    int ydir;

    SDL_GameController *controller = NULL;

    for (int i = 0; i < SDL_NumJoysticks; i++)
        {
            if(SDL_IsGameController(i))
            {
                controller = SDL_GameControllerOpen(i);
                if(controller)
                {
                    break;
                }
                
            }
            
        }

    debugPrint("Please tell me your desired calculation type. For help enter h (access number 14).\n");

    while (user_command != 13)
    {   
        
        for (int i; i<ARRAY_SIZE(operations);)
        {
            MathOperation* o = &operations[i]; 

            switch (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) < deadzone)
            {
            case true:
                xdir = -1;
                break;
            
            case false:
                xdir = 1;
                break;
            default:
                xdir = 0;
                break;
            }

            switch (xdir)
            {
            case -1:
                i--;
                break;
            
            case 1:
                i++;
                break;

            default:
                break;
            }

            user_command = i;

            debugPrint(user_command);

            if (o->accessnum == user_command)
            {
                if(o->use_default_input == true)
                {
                    debugPrint("Please give me the number you want to use for the operation. ");
                    switch (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) < deadzone)
                    {
                        case true:
                            ydir = -1;
                            break;
            
                        case false:
                            ydir = 1;
                            break;
                        default:
                            ydir = 0;
                            break;
                    }

                    switch (ydir)
                    {
                        case -1:
                            user_input--;
                            break;
            
                        case 1:
                            user_input++;
                            break;

                        default:
                            break;
                    }
                }
            }   result = o->handler(result,user_input);
                debugPrint("Result is %f. Please tell me your next calculation.\n", result);
                    
        
        }
    }
    
}