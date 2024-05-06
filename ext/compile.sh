gcc -shared -fPIC *.c -I ../include -L . -lPACE -lm -lGL -lGLU -lGLEW -lglfw -lX11 -o ../build/ext.so -Wall -Werror -Wl,-rpath .
