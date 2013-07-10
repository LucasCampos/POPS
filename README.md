POPS - Plotter of POPS Screens
====

POPS generates images from an input file, containing bidimensional particles. These can be of circular, triangular or squared shape

It can draw figures in png and eps format.

#Example of usage

```bash
 pops -c test/conffinal.dat 5000 0.05 -b 10 -o test/saida.png -l "N=5000" -l "Yukawa" -l "Teste" 
 ```
 
This line will plot the data in the file test/conffinal.dat. It will generate 5000 circles, each with radius 0.05. The box will be from (-10, -10) to (10, 10), with three labels appearing on the right-upper corner, "N=5000", "Yukawa" and "Teste".


#Compiling

To compile POPS, one will need png and freetype libraries. On Ubuntu, these can be installed using

```bash
sudo apt-get install libpng12-dev libfreetype6-dev
```
Also, one will need a modern c++ compiler, with c++11 features. The makefile uses g++, but one may change this by modifying the CC variable on makefile.

After resolving these dependencies, one can simply use make to build and test POPS. If everything went OK, two images will be created on test/

```bash
make
make run
```

To install, super user mode is needed. Once again, one may use make, running sudo.

```bash
sudo make install
```

#Input File

For each kind of particle, POPS expects a two-column file, containing the particles' position and no header. See the file conffinal.dat on test to an example of input file

#Runtime Options

```bash
 -h
``` 
Show help dialog

```bash
 -s filename Quantidade
```
Add a new kind of square, with Quantity squares. To each square, there must be five lines on the file.

```bash
 -t filename Quantidade
```
Add a new kind of triangle, with Quantity triangles. To each triangle, there must be four lines on the file.

```bash
 -c filename Quantidade Radius
```
Add a new kind of circle, with Quantity circles. To each circle, there must be one line on the file.
  
```bash
 -o Name
```
Output filename. If the extension is set to png, it will write a png. Otherwise, will write a eps.

```bash
 -f Frame
```
Chooses the resolution of the output file. If this flag is not set, the resolution will be 1920x1920.

```bash
 -b Box
```
Box size. If unset, the value will be 10.
  
```bash
 -r Distance
```
Draw a line between particle centers' if they are closer than Distance.

```bash
 -e
```
The standard is having the box centred on (0,0). If you wish its left-down corner to be on (0,0), set this flag.	

```bash
 -w width
```
Chooses linewidth, relative to box size. The standard is 0.1 box.

```bash
 -l name
```
Adds a new label on the left-up side.

```bash
 -fs size
```
Selects the new font size.

