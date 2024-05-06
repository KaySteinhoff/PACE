gcc src/main.c -I include -L . -lPACE -lm -lGL -lGLU -lglfw -lGLEW -lX11 -o build/SpacE -Wall -Werror -Wl,-rpath .
