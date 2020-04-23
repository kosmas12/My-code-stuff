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

// function prototypes
static float help(float a, float b);

static float quit(float a, float b);

static bool isNewlyPressed(bool is_held, bool *was_held);

static char getCommand();

static float getInput();

static void minicalc(float a, float b);

const float rads_per_degree = 0.01745329252; // Rads per degree for the *D functions

typedef struct {
    const char command; // Command character
    const char* description; // Command description
    bool use_default_input; // Do we use the default input handler or not?
    float(*handler)(float a, float b); // Pointer to the function that we will use for the operation
}MathOperation; // Create a struct for our Mathematical Operations

static float addition(float a, float b) { return a + b; }

static float multiplication(float a, float b) { return a * b; }

static float division(float a, float b) { return a / b; }

static float subtraction(float a, float b) { return a - b; }

static float cosD(float a, float b) { return cos(a * rads_per_degree); } // math.h uses rads so we need to convert degrees to rads

static float cosR(float a, float b) { return cos(a); }

static float tanD(float a, float b) { return tan(a * rads_per_degree); }

static float tanR(float a, float b) { return tan(a); }

static float sinD(float a, float b) { return sin(a * rads_per_degree); }

static float sinR(float a, float b) { return sin(a); }

static float squareRoot(float a, float b) { return sqrt(a); }

static float power(float a, float b) {return pow(a, b); }

static float reset(float a, float b) {a = 0.0f; return a; }

char user_command = '\0'; // Initialize a default character for the user's command (\0 is NULL)

float user_input = 1.0f; // Initialize a default user_input of 1.0

float result = 0.0f; // Initialize a default result which is what we will be working on

SDL_GameController *controller = NULL; // We initialize a controller and give it a NULL value for now

static void Init() {
    XVideoSetMode(640, 480, 32, REFRESH_60HZ); // Initialize the video for the console (640x480, 32bit color depth, 60Hz refresh rate)

    SDL_Init(SDL_INIT_JOYSTICK|SDL_INIT_VIDEO); // Initialize SDL
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
  { .command='h', .description="Help page", .use_default_input=false, .handler=help}
}; // Create the array with the command characters, their descriptions, if they use the default input handler and what is their operation handler

static float quit(float a, float b) { debugPrint("Exiting...\n"); exit(0); }

static float help(float a, float b) {
    static bool a_is_held = true; // Create a bool for checking if A is held in this frame

    static bool a_was_held = true; // Create a bool for checking if A was held in the previous frame

    while (true) { // Forever
        XVideoWaitForVBlank(); // Wait for next refresh

        debugClearScreen(); // Clear the screen

        SDL_GameControllerUpdate(); // Update the controller

        for(int i = 0; i < ARRAY_SIZE(operations); i++) { // For the time that i (initialized with 0) is less than the Array size of operations  
            MathOperation* o = &operations[i]; // Pointer to the memory address of operations[] with index number i (changes in each iteration) called o. Expects pointing to MathOperation things only
            debugPrint("%c: %s\n", o->command, o->description); // Print the command and description visible in the current array index
        }
        // After the loop is over, print these messages that should be printed only once
        debugPrint("Also note that you start with 0 on the result so your first operation will work with 0 and your number.\n");
        debugPrint("Press B to add 10 to the operation, X for 100, and Y for 1000");

        if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A)) { // If A is pressed
            a_is_held = true; // A is held this frame, so set a_is_held to true
        } 
        else {
            a_is_held = false;
        }
        

        if (isNewlyPressed(a_is_held, &a_was_held)) { // If A is newly pressed
            break; // Exit the while(true) loop
        }
    }
    return a; //When exiting the loop return the variable a
}

// This function can only print positive float values (negatives are not supported!)
void printpositivefloat(float value) {
    unsigned int notround = (unsigned int)(value * 1000.0 + 0.5f); // Notround can only have positive values. It is our value by 1000 and then added 0.5
    unsigned int beforeperiod = notround / 1000; // Beforeperiod (.) is notround divided by 1000 (because we multiply by 1000 in notround)
    unsigned int afterperiod = notround % 1000; // Afterperiod is the remainder of the division between notround and 1000
    debugPrint("%u.%03u", beforeperiod, afterperiod); // Print beforeperiod.afterperiod (afterperiod is maximum 3 digits)
}

// This function can print positive and negative float values
void printfloat(float value) {
    if (value < 0.0f) { // If value is negative
        debugPrint("-"); // Draw sign
        value = -value; // Make value positive
    }
    printpositivefloat(value); // Print positive value
}

static bool isNewlyPressed(bool is_held, bool *was_held) {

    if (is_held) { // If the button is held in this frame
        if (*was_held) { // If it was held in the previous frame
            return false; // The button isn't newly pressed
        } 
        else { // If it wasn't held in the previous frame
            *was_held = true; // Set was_held to true because the current frame will be the previous frame in the next frame
            return true; // The button is newly pressed, so we return true
        } 
        
    } 
    else { // If it isn't held in the current frame
        *was_held = false; // Set was_held to false
        return false; // The button isn't pressed at all, so we return false
    }
}


float remap(float value, float from_min, float from_max, float to_min, float to_max) { // Function to remap values to other values
    value = ((value - from_min) / (from_max - from_min)) * (to_max - to_min) + to_min;
    return value;
}

static float getAxis(int sdl_axis) { // Function to get an axis from the controller

  const float deadzone = 0.2f;

  // Get input in range -1 to +1
  float amount = (float)SDL_GameControllerGetAxis(controller, sdl_axis) / (float)0x8000;

  // Reject if the stick is in deadzone
  if (fabsf(amount) < deadzone) {
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

static char getCommand() {
    static bool a_is_held = true;

    static bool a_was_held = true;

    int accessnum = 0; // We use an access number to iterate through the operations[] table

    while (true) { // Forever
        XVideoWaitForVBlank();

        debugClearScreen();

        SDL_GameControllerUpdate();

        debugPrint("Current result is: ");
        printfloat(result); // Use printfloat() to print our current result
        debugPrint(". Please tell me your desired calculation type. For help enter h.\n");

        float Xamount = getAxis(SDL_CONTROLLER_AXIS_LEFTX); // Xamount is the amount returned by getAxis for the X axis of the left analog stick

        bool is_left_analog_left = Xamount < -0.5f; // Initialize a boolean to check if Xamount is lesser than -0.5. If so, the left analog is left

        bool is_left_analog_right = Xamount > 0.5f; // Initialize a boolean to check if Xamount is greater than 0.5. If so, the left analog is right

        bool was_left_analog_left = false;

        bool was_left_analog_right = false;

        // Code to prevent the access number from going out of bounds from operations[]
        if(accessnum > 0) { // If accessnum is bigger than 0
            if(isNewlyPressed(is_left_analog_left, &was_left_analog_left)) { accessnum--; } // If the left analog is left reduce the accessnum
        } 
        else {
            accessnum = 0; // If it is smaller than 0, set it to 0
        }
        
        // ARRAY_SIZE(operations) returns all of the elements, but since it also counts index 0 we want accessnum to see if accessnum is lesser than the size - 1
        if(accessnum < ARRAY_SIZE(operations) - 1) {
            if(isNewlyPressed(is_left_analog_right, &was_left_analog_right)) { accessnum++; } // If the left analog is right increase the accessnum
        }
        else {
            accessnum = ARRAY_SIZE(operations) - 1; // If accessnum is bigger than the size - 1 then set it to size - 1
        }
        
        MathOperation* o = &operations[accessnum]; // Like in help() but with an accessnum instead of i

        debugPrint("Current selected mode is: %c (%s)\n", o->command, o->description); // Print the current command and description

        if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A)) {
            a_is_held = true;
        }
        else {
            a_is_held = false;
        }
        
        if (isNewlyPressed(a_is_held, &a_was_held)) {
            user_command = o->command; // Set the user command to the current command in operations[]
            break;
        }
    }

    return user_command;
}

static float getInput() {
    static bool a_is_held = true;

    static bool a_was_held = true;

    static bool b_is_held = true;

    static bool b_was_held = true;

    static bool x_is_held = true;

    static bool x_was_held = true;

    static bool y_is_held = true;

    static bool y_was_held = true;

    while (true) {  

        XVideoWaitForVBlank();

        debugClearScreen();

        SDL_GameControllerUpdate();

        debugPrint("Please give me the number you want to use for the operation. ");

        float Yamount = getAxis(SDL_CONTROLLER_AXIS_LEFTY);

        user_input += -Yamount / (float) 144; // user_input is equal to its current value + inverted Yamount (Workaround for inverted Y axis)

        debugPrint("Your input is: ");
        printfloat(user_input);
        debugPrint("\n");
        printfloat(-Yamount);



        if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A)) {
            a_is_held = true;
        }
        else {
            a_is_held = false;
        }
        if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B)) {
            b_is_held = true;
        }
        else {
            b_is_held = false;
        }
        if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X)) {
            x_is_held = true;
        }
        else {
            x_is_held = false;
        }
        if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y)) {
            y_is_held = true;
        }
        else {
            y_is_held = false;
        }

        if(isNewlyPressed(a_is_held, &a_was_held)) {    
            break;
        }
        else if(isNewlyPressed(b_is_held, &b_was_held)) {    
            user_input += 10;
        }
        else if(isNewlyPressed(x_is_held, &x_was_held)) {    
            user_input += 100;
        }
        else if(isNewlyPressed(y_is_held, &y_was_held)) {    
            user_input += 1000;
        }
    }
    return user_input;
    
}
int main() {
    
    static bool a_is_held = true;

    static bool a_was_held = false;

    Init(); // Initialize using the Init() function for video and SDL


    for (int i = 0; i < SDL_NumJoysticks(); i++) { // For the time that i is smaller than the number of connected Joysticks

        if(SDL_IsGameController(i)) { // If i (which we use to iterate through the connected controllers) as a port number is a Game Controller
            controller = SDL_GameControllerOpen(i); // Open the controller

            if(controller) { // If we find that we opened a controller
                break; // Exit the loop
            }
                
        }
    }

    while(user_command != 'q') {

        getCommand();

        for (int i = 0; i < ARRAY_SIZE(operations); i++) {

            MathOperation* o = &operations[i]; // Iterate through operations[] using i from the for() loop

            if (o->command == user_command) { // When the command number of the index of i in operations[] equals our user_command
            
                if(o->use_default_input == true) { // If it uses the default input handler
                
                    getInput();
                }
                result = o->handler(result, user_input); // Calculate our result and store it
                user_input = 1.0f; // Resets user_input after every operation
            }
        }
    }
}