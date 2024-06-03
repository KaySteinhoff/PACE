gcc -shared -fPIC src/*.c -I ./include -I /usr/include/freetype2 -L /usr/lib64 -lfreetype -lGL -lGLU -lGLEW -lglfw -lX11 -o lib/libPACE.so
