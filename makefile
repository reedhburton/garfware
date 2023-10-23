CC = g++
CFLAGS = -Wall -g -luuid -lstdc++fs -lole32

garfware: garfware.o conversions.o pathhelper.o shortcut.o
	$(CC) obj/garfware.o obj/conversions.o obj/pathhelper.o obj/shortcut.o -o bin/garfware $(CFLAGS)

# The main.o target can be written more simply
 
garfware.o: src/garfware.cpp src/conversions.h src/pathhelper.h src/shortcut.h
	$(CC) $(CFLAGS) -c src/garfware.cpp -o obj/garfware.o
 
conversions.o: src/conversions.cpp
	$(CC) $(CFLAGS) -c src/conversions.cpp -o obj/conversions.o

pathhelper.o: src/pathhelper.cpp
	$(CC) $(CFLAGS) -c src/pathhelper.cpp -o obj/pathhelper.o

shortcut.o: src/shortcut.cpp
	$(CC) $(CFLAGS) -c src/shortcut.cpp -o obj/shortcut.o

clean:
	del bin\*
	del obj\*

garf:
	.\bin\garfware.exe