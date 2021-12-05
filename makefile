CC = x86_64-w64-mingw32-g++

bin/flying-toasters.scr: obj/flying-toasters.o
	mkdir -p bin
# Copy appropriate dlls into bin folder
	cp lib/SFML-2.5.1/bin/sfml-graphics-2.dll bin
	cp lib/SFML-2.5.1/bin/sfml-system-2.dll bin
	cp lib/SFML-2.5.1/bin/sfml-window-2.dll bin
	cp lib/SFML-2.5.1/bin/sfml-window-2.dll bin
	cp /usr/lib/gcc/x86_64-w64-mingw32/9.3-win32/libstdc++-6.dll bin
	cp /usr/x86_64-w64-mingw32/lib/libwinpthread-1.dll bin
# Construct sheet.hpp using 'xxd' command
	$(CC) -O3 -o bin/flying-toasters.scr obj/flying-toasters.o -Llib/SFML-2.5.1/lib -lmingw32 -lgdi32 -ldwmapi -lsfml-graphics -lsfml-window -lsfml-system

obj/flying-toasters.o: src/flying-toasters.cpp
	mkdir -p obj
	xxd -i src/sheet.png > src/sheet.hpp
	$(CC) -o obj/flying-toasters.o -c -Ilib/SFML-2.5.1/include src/flying-toasters.cpp

clean:
	rm -rf bin
	rm -rf obj
	rm src/sheet.hpp