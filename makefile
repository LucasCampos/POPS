OPTFLAGS    = -fast
CC          = g++
MAKE        = make
SHELL       = /bin/sh

CFLAGS  = -std=c++11
FOLDERS =
FREETYPE_INCLUDE = /usr/include/freetype2 
LIBS = -lpng -lz -lfreetype
EXECS = pops
SOURCES = POPS.cpp libpngwriter/pngwriter.cpp libepswriter/epswriter.cpp
CXXFLAGS= -O3 -Wall $(FT_ARG)
OBJECTS = POPS.o pngwriter.o epswriter.o

$(EXECS): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ -I $(FREETYPE_INCLUDE) $(CFLAGS) $(LIBS) 

.cpp.o: obj/
	$(CC) -c $(SOURCES) -I $(FREETYPE_INCLUDE) $(CFLAGS) $(LIBS) 

clean:
	/bin/rm -f *.o *.mod $(EXECS) *.gnu *.sh *.gif
run: $(EXECS) 
	./pops -c test/conffinal.dat 5000 0.05 -b 10 -o teste/saida.png -l "N=5000" 
	./pops -c test/conffinal.dat 5000 0.05 -b 10 -o teste/saida.eps -l "N=5000" 