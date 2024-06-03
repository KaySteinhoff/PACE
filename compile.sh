gcc src/main.c -I include -I /usr/include/freetype2 -L . -lPACE -L /usr/local/lib -lfreetype -lm -lGL -lGLU -lglfw -lGLEW -lX11 -o build/SpacE -Wall -Werror -Wl,-rpath .
