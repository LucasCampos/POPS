POPS - Plotter of POPS Screens
====

POPS generates images from an input file, containing bidimensional particles. These can be of circular, triangular or squared shape

It can draw figures in png and eps format.

#Compiling

To compile POPS, one will need png and freetype libraries. On Ubuntu, these can be installed using

```bash
sudo apt-get install libpng12-dev libfreetype6-dev
```
Also, one will need a modern c++ compiler, with c++11 features. The makefile uses g++, but one may change this by modifying the CC variable on makefile
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

#Runtime Options

#Example of usage
