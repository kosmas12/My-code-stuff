# mandelbrot-c-sdl2

Using the C language and the SDL2 library to draw the Mandelbrot set.

## Features

PC controls (for master branch):

* Use the **arrow keys** to move
* Use the **+ key** to zoom in
* Use the **- key** to zoom out

Xbox controls (for nxdk branch):

* Use **left analog stick** to move
* Use **RT** to zoom in
* Use **LT** to zoom out


## Requirements

PC:
* C standard library with (G)CC
* SDL2 library
* Possibly MinGW if running Windows

Xbox:
* nxdk (https://github.com/XboxDev/nxdk)

## Compilation

```
$ make help
Just use make (without any argument) to compile.
Use make clean in order to delete mandelbrot.
If you have errors during compilation, you can contact me for help.
Windows users, you can use MinGW with this project.
```

Then run the program and enjoy ! :-)

![screenshot](screenshot.png "This is beautiful...")

Happy fractal !
