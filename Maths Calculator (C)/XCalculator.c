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

char user_command = '\0';

const int deadzone = 4000;

float user_input = 1.0f;

float result = 0.0f;

SDL_GameController *controller = NULL;

static void Init()
{
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1"); 

    SDL_Init(SDL_INIT_JOYSTICK|SDL_INIT_VIDEO); //Initialize SDL for consoles
    
    SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");
}

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
  { .command='p', .description="Raise to the power of", .use_default_input=true, .handler=power},
  { .command='e', .description="Reset result", .use_default_input=false, .handler=reset},
  { .command='q', .description="Quit", .use_default_input=false, .handler=quit },
  { .command='h', .description="Help page", .use_default_input=false, .handler=help}
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

// Stop asking for input if the user presses A
static bool a_is_held = true;
    
static bool isAPressed() 
{
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A)) 
    {
        // A is pressed, but do we allow it?
        if (a_is_held) 
        {
            return false; // A is pressed now, but it was never released
        } 
        else 
        {
            a_is_held = true; // Remember that A was not released since this press
            return true; // A is pressed right now, but it wasn't pressed before: allow
        }
        
        
    }
    else 
    {
            a_is_held = false; // Remember that the user did not press A, so we can allow him to press it again
            return false;
    }
}

static char getCommand(void)
{
    Sint16 Xamount = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);

    while (true)
    {
        for (int i = 0; i < ARRAY_SIZE(operations);)
        {
            MathOperation* o = &operations[i];

            if (Xamount > deadzone)
            {
                i++;
                debugPrint("Selected mode is: %c (%c)", o->command, o->description);
            }
            else if (Xamount < -deadzone)
            {
                i--;
                debugPrint("Selected mode is: %c (%c)", o->command, o->description);
            }

        }
        if (isAPressed())
        {
            break;
        }
    }   
    return user_command;
}

static float getInput()
{
  while(true) 
  {
    XVideoWaitForVBlank(); // Wait for next frame
    debugClearScreen();
    SDL_GameControllerUpdate(); // Update gamepad values

    debugPrint("Your current input [*1000] is %d", (int)(user_input*1000.0f));

    float Yamount = (float)SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) / (float)0x8000; // Get input in range -1 to +1
    if (fabsf(Yamount) > 0.2f) 
    { // Check 20% deadzone
        user_input += Yamount / (float)REFRESH_DEFAULT; // Modify input
    }
  
    if(isAPressed())
    {
        break;
    }
  }
  // Return the input
  return user_input;
}


int main()
{
    
    Init();


    for (int i = 0; i < SDL_NumJoysticks(); i++)
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

while(true) { float v = getInput(); debugPrint("getInput * 1000: %d", (int)(v * 1000)); }

    debugPrint("Please tell me your desired calculation type. For help enter h.\n");

    while(user_command != 'q')
    {
        getCommand();

        for (int i; i < ARRAY_SIZE(operations); i++)
        {
            MathOperation* o = &operations[i];

            if (o->command == user_command)
            {
                if(o->use_default_input == true)
                {
                        getInput();
                }
                result = o->handler(result, user_input);
                debugPrint("Result is %f. Please tell me your next calculation.\n", result);
            }
        }

        
    }


}