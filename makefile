CC          = g++
MAKE        = make
SHELL       = /bin/sh

FOLDERS =
FREETYPE_INCLUDE = /usr/include/freetype2 
LIBS = -lpng -lz -lfreetype
EXECS = pops
SOURCES = POPS.cpp  libepswriter/epswriter.cpp #libpngwriter/pngwriter.cpp
CXXFLAGS= -std=c++11 -D SKIPPNG
OBJECTS = POPS.o epswriter.o #libpngwriter/pngwriter.cpp

$(EXECS): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ -I $(FREETYPE_INCLUDE) $(CXXFLAGS) $(LIBS) 

.cpp.o: 
	$(CC) -c $(SOURCES) -I $(FREETYPE_INCLUDE) $(CXXFLAGS) $(LIBS) 

install: $(EXECS)
	mv $(EXECS) /usr/bin

clean:
	/bin/rm -f *.o *.mod $(EXECS) *.gnu *.sh *.gif
run: $(EXECS) 
	./$(EXECS) -c test/conffinal.dat 5000 0.05 -b 10 -o test/saida.png -l "N=5000" -l "Yukawa" -l "Teste" 
	./$(EXECS) -c test/conffinal.dat 5000 0.05 -b 10 -o test/saida.eps -l "N=5000" -l "Yukawa" -l "Teste" 
