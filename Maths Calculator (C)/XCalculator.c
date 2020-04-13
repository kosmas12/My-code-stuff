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

static bool isNewlyPressed(bool is_held, bool *was_held);

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

float user_input = 1.0f;

float result = 0.0f;

SDL_GameController *controller = NULL;

static void Init()
{
    XVideoSetMode(640, 480, 32, REFRESH_60HZ);

    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1"); 

    SDL_Init(SDL_INIT_JOYSTICK|SDL_INIT_VIDEO); //Initialize SDL for consoles
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
    static bool a_is_held = true;

    static bool a_was_held = true;

    while (true)
    {
        XVideoWaitForVBlank();

        debugClearScreen();

        SDL_GameControllerUpdate();

        for(int i = 0; i < ARRAY_SIZE(operations); i++) 
        {   
            MathOperation* o = &operations[i];
            debugPrint("%c: %s\n", o->command, o->description);
        }
        debugPrint("Also note that you start with 0 on the result so your first operation will work with 0 and your number.\n");
        debugPrint("Press B to add 10 to the operation, X to add 100, and Y to add 1000");

        if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A))
        {
            a_is_held = true;
        }
        else
        {
            a_is_held = false;
        }

        if (isNewlyPressed(a_is_held, &a_was_held))
        {
            break;
        }
    }
    debugPrint("ẗest");
    return a;
}

// This function can only positive float values (negatives are not supported!)
void printpositivefloat(float value) {
    unsigned int notround = (unsigned int)(value * 1000.0 + 0.5f);
    unsigned int beforeperiod = notround / 1000;
    unsigned int afterperiod = notround % 1000; // I'd just use endallroundingmadness % 1000
    debugPrint("%u.%03u", beforeperiod, afterperiod);
}

// This function can print positive and negative float values
void printfloat(float value) {
  if (value < 0.0f) {
    debugPrint("-"); // Draw sign
    value = -value; // Make value positive
  }
  printpositivefloat(value); // Print positive value
}


static bool isNewlyPressed(bool is_held, bool *was_held) 
{

    if (is_held) 
    {
        if (*was_held) 
        {
            return false;
        } 
        else 
        {
            *was_held = true;
            return true;
        } 
        
    }
    else 
    {
        *was_held = false;
        return false;
    }
}


float remap(float value, float from_min, float from_max, float to_min, float to_max){
    value = (value - from_min) / (from_max - from_min);
    value = value * (to_max - to_min) + to_min;
    return value;
}

static float getAxis(int sdl_axis) 
{
  const float deadzone = 0.2f;

  // Get input in range -1 to +1
  float amount = (float)SDL_GameControllerGetAxis(controller, sdl_axis) / (float)0x8000;

  // Reject if the stick is in deadzone
  if (fabsf(amount) < deadzone) 
  {
    return 0.0f;
  }

  if (amount > 0.0f) {
  amount = remap(amount,  deadzone, +1.0f,  0.0f, +1.0f); // Remap from +[0.2, 1.0] to +[0.0, 1.0]
  } 
  else {
  amount = -remap(-amount,  deadzone, +1.0f,  0.0f, +1.0f); // Remap from -[0.2, 1.0] to -[0.0, 1.0]
  }
  return amount;
}

static char getCommand(void)
{
    static bool a_is_held = true;

    static bool a_was_held = true;

    static bool x_is_pushed = true;

    static bool x_was_pushed = true;

    int accessnum = 0;

    while (true)
    {
        XVideoWaitForVBlank();

        debugClearScreen();

        SDL_GameControllerUpdate();

        debugPrint("Current result is: ");
        printfloat(result);
        debugPrint(". Please tell me your desired calculation type. For help enter h.\n");

        float Xamount = getAxis(SDL_CONTROLLER_AXIS_LEFTX);

        bool is_left_analog_left = Xamount < -0.5f;

        bool is_left_analog_right = Xamount > 0.5f;

        bool was_left_analog_left = false;

        bool was_left_analog_right = false;

        if(accessnum > 0)
        {
            if(isNewlyPressed(is_left_analog_left, &was_left_analog_left)) { accessnum--; }
        }
        else
        {
            accessnum = 0;
        }
        

        if(accessnum < ARRAY_SIZE(operations) - 1)
        {
            if(isNewlyPressed(is_left_analog_right, &was_left_analog_right)) { accessnum++; }
        }
        else
        {
            accessnum = ARRAY_SIZE(operations) - 1;
        }
        
        MathOperation* o = &operations[accessnum];

        debugPrint("Current selected mode is: %c (%s)", o->command, o->description);

        if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A))
        {
            a_is_held = true;
        }
        else
        {
            a_is_held = false;
        }
        
        if (isNewlyPressed(a_is_held, &a_was_held))
        {
            user_command = o->command;
            break;
        }
    }

    return user_command;
}

static float getInput()
{
    static bool a_is_held = true;

    static bool a_was_held = true;

    static bool b_is_held = true;

    static bool b_was_held = true;

    static bool x_is_held = true;

    static bool x_was_held = true;

    static bool y_is_held = true;

    static bool y_was_held = true;

    while (true)
    {   
        XVideoWaitForVBlank();

        debugClearScreen();

        SDL_GameControllerUpdate();

        debugPrint("Please give me the number you want to use for the operation. ");

        float Yamount = getAxis(SDL_CONTROLLER_AXIS_LEFTY);

        user_input += -Yamount / (float)REFRESH_60HZ;

        debugPrint("Your input is: ");
        printfloat(user_input);
        debugPrint("\n");
        printfloat(-Yamount);



        if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A))
        {
            a_is_held = true;
        }
        else
        {
            a_is_held = false;
        }
        if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B))
        {
            b_is_held = true;
        }
        else
        {
            b_is_held = false;
        }
        if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X))
        {
            x_is_held = true;
        }
        else
        {
            x_is_held = false;
        }
        if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y))
        {
            y_is_held = true;
        }
        else
        {
            y_is_held = false;
        }

        if(isNewlyPressed(a_is_held, &a_was_held))
        {    
            break;
        }
        else if(isNewlyPressed(b_is_held, &b_was_held))
        {    
            user_input += 10;
        }
        else if(isNewlyPressed(x_is_held, &x_was_held))
        {    
            user_input += 100;
        }
        else if(isNewlyPressed(y_is_held, &y_was_held))
        {    
            user_input += 1000;
        }
    }
    return user_input;
    
}
int main()
{
    
    static bool a_is_held = true;

    static bool a_was_held = false;

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

    while(user_command != 'q')
    {
        getCommand();

        for (int i = 0; i < ARRAY_SIZE(operations); i++)
        {
            MathOperation* o = &operations[i];

            if (o->command == user_command)
            {
                if(o->use_default_input == true)
                {
                        getInput();
                }
                result = o->handler(result, user_input);
            }
        }
    }
}