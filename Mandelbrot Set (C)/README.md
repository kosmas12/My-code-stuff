# mandelbrot-c-sdl2

Forked from https://github.com/Geospace/mandelbrot-c-sdl2/tree/master

The Xbox port is made by me

Using the C language and the SDL2 library to draw the Mandelbrot set.

## Features

Controls:
* Use **D-Pad** to move
* Use **B** to zoom in
* Use **X** to zoom out

## Requirements

PC:
* C standard library with (G)CC
* SDL2 library
* Possibly MinGW if running Windows

Xbox:
* nxdk (https://github.com/XboxDev/nxdk)

To compile for nxdk, use:

make -f Makefile.nxdk 

## Compilation

```
$ make help
Just use make (without any argument) to compile.
Use make clean in order to delete mandelbrot.
If you have errors during compilation, you can contact me for help.
Windows users, you can use MinGW with this project.
```

Then run the program and enjoy ! :-)

![screenshot](screenshotx.png "This is beautiful...")

Happy fractal !
