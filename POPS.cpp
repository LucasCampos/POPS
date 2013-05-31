#include <boost/shared_ptr.hpp>
/*
 * Name:	POPS - Plotter of POPS Screens
 * Author:	Lucas Costa Campos
 * Email: 	Rmk236@gmail.com
 * Version:	0.9
 * License:	GNU General Public License
 * 		Copyright: 2013 Lucas Costa Campos
 * Website: 	https://github.com/LucasCampos/POPS
 */

/*
 *     This file is part of POPS.
 *
 *     POPS is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program; if not, write to the Free Software
 *     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * */
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "include/imageWriter.hpp"
#include "include/polygon.hpp"
#include "include/colorPicker.hpp"

using namespace std;

typedef boost::shared_ptr<std::ifstream>  IFStreamPtrType;
typedef boost::shared_ptr<DrawablePolygon>  DrawablePolygonPtrType;

int lineWidth = 10;

struct PolyReader {
	int _pointsPerPoly;
	IFStreamPtrType _in;
	bool _jump;
	vector<DrawablePolygonPtrType> _poly;

	PolyReader(int points, char* filename, bool jump, vector<DrawablePolygonPtrType> poly): 
		_pointsPerPoly(points), _in(new std::ifstream(filename)), _poly(poly) {
	};

	void updatePoly() {
		int size = _poly.size();
		for (int i=0; i<size; i++) {
			
			double x,y;
			(*_in) >> x >> y;

			//std::cout << i << "   " <<  charLeitor << "\n";
			
			bool notEnd = ((x != 0) && (y != 0)) &&  !(_in->eof());
			//notEnd = true;
			if (notEnd) {
				_poly[i]->_pos = Vector2D(x,y);
				_poly[i]->_vertex.clear();
			}
			for (int j =0; j<_pointsPerPoly; j++) {
				*_in >> x >> y;
				if(notEnd) 
					_poly[i]->_vertex.push_back(Vector2D(x,y));
			}
		}
	};

	void draw(ImageWriter *writer) {
		int size = _poly.size();
		//cout << size << endl;
		for (int i=0; i<size; i++) 
			_poly[i]->draw(writer);
	};

	void rescale (const double factor) {
		int size = _poly.size();
		//cout << size << endl;
		for (int i=0; i<size; i++) 
			_poly[i]->rescale(factor);
	};

	void translate (const Vector2D T) {
		int size = _poly.size();
		//cout << size << endl;
		for (int i=0; i<size; i++) 
			_poly[i]->translate(T);
	};

	void drawLines(ImageWriter *writer, vector<DrawablePolygonPtrType> p, const double distance, const int width) {

		int nHere = _poly.size();
		int nOther = p.size();

		for (int i=0; i<nHere; i++) {
			for(int j=0; j<nOther; j++) {

				Vector2D r =_poly[i]->_pos-(p[j]->_pos);
				//cout << r.norm() << " " << distance << std::endl;
				double norm = r.norm();
				if ((norm < distance) && (norm > 1e-5))
					writer->line(_poly[i]->_pos.getX(), _poly[i]->_pos.getY(), p[j]->_pos.getX(), p[j]->_pos.getY(), 0.01, 0,0,0);



			}
		}
	};
};

//Variaveis Globais
char* font =  (char*)"/usr/share/fonts/truetype/freefont/FreeMonoBold.ttf";

void Normalize(ImageWriter* writer, std::vector<PolyReader>& poly, bool extendedBox);
void TakeParameters(int argc, char* argv[], ImageWriter** writer, std::vector<PolyReader>& poly, bool& drawDistance, double& minDist, bool& extendedBox);
void Draw(ImageWriter* writer, std::vector<PolyReader>& poly, bool extendedBox, bool drawDistance, double minDist, double lineWidth);
void connectPoly(ImageWriter *writer, std::vector<PolyReader>& poly, double minDist, double lineWidth);
void SimpleHelp();

int main(int argc, char* argv[])
{
	std::vector<PolyReader> poly;
	bool extendedBox = true;
	bool drawDistance = false;
	double minDist = 1e9;
	ImageWriter* writer;
	TakeParameters(argc, argv, &writer, poly, drawDistance, minDist, extendedBox);
	writer->printname();
	Draw(writer, poly, extendedBox, drawDistance, minDist, 0.1);
	writer->close();
}

void Normalize(ImageWriter* writer, std::vector<PolyReader>& poly, bool extendedBox) {
	double factor = writer->NormalizingFactor();
	double center = writer->Center();

	Vector2D T(center,center);
	for (int i=0; i<poly.size(); i++){
		
		poly[i].rescale(factor);
		if (extendedBox)
			poly[i].translate(T);
	}
};

void TakeParameters (int argc, char* argv[], ImageWriter** writer, std::vector<PolyReader>& poly, bool& drawDistance, double& minDist, bool& extendedBox){

	bool hasOne = false;
	std::string nameOutput;
	double box;
	double resolution=1920;

	if (argc == 1) {
		SimpleHelp();
		exit(0);
	}

	for (int i=0;i<argc; i++){
		if (string(argv[i]) == "-s"){
			char* file = argv[i+1];
			int N = atoi(argv[i+2]);

			vector<DrawablePolygonPtrType> p;
			Color c = ColorPicker::getColor();

			for (int j=0; j<N; j++) {
				DrawablePolygonPtrType polygon(new Square(c.red, c.green,c.blue));
				p.push_back(polygon);
			}

			poly.push_back(PolyReader(4, file, true,p));

			hasOne = true;

		} else if (string(argv[i]) == "-t"){
			char* file = argv[i+1];
			int N = atoi(argv[i+2]);

			vector<DrawablePolygonPtrType> p;
			Color c = ColorPicker::getColor();

			for (int j=0; j<N; j++) {
				DrawablePolygonPtrType polygon(new Triangle(c.red, c.green, c.blue));
				p.push_back(polygon);
			}

			poly.push_back(PolyReader(3, file, true,p));

			hasOne = true;

		} else if (string(argv[i]) == "-c"){
			char* file = argv[i+1];
			int N = atoi(argv[i+2]);
			double radius = atof(argv[i+3]);

			vector<DrawablePolygonPtrType> p;
			Color c = ColorPicker::getColor();

			for (int j=0; j<N; j++) {
				DrawablePolygonPtrType polygon(new Circle(radius, c.red,c.green,c.blue));
				p.push_back(polygon);
			}

			poly.push_back(PolyReader(0, file, true,p));
	
			hasOne = true;

		} else if (string(argv[i]) == "-o") {

			if (argc <= i+1) {
				cout << "Incomplete parameters for -o \n";
				exit(1);
			}
			nameOutput= std::string(argv[i+1]);	
		} else if (string(argv[i]) == "-f") {

			if (argc <= i+1) {
				cout << "Incomplete parameters for -f \n";
				exit(1);
			}
			resolution = atoi(argv[i+1]);	
		} else if (string(argv[i]) == "-b") {

			if (argc <= i+1) {
				cout << "Incomplete parameters for -b \n";
				exit(1);
			}
			box = atof(argv[i+1]);	
		} else if (string(argv[i]) == "-r") {

			if (argc <= i+1) {
				cout << "Incomplete parameters for -r \n";
				exit(1);
			}

			drawDistance = true;
			minDist = atof(argv[i+1]);
		} else if (string(argv[i]) == "-w") {

			if (argc <= i+1) {
				cout << "Incomplete parameters for -r \n";
				exit(1);
			}

			lineWidth = atoi(argv[i+1]);
		} else if (string(argv[i]) == "-h") {
			SimpleHelp();
			exit(0);
		} else if (string(argv[i]) == "-e"){
			extendedBox = false;
		}/* else if (string(argv[i]) == "-l") {
			if (argc <= i+1) {
				cout << "Parametro -l incompleto\n";
				exit(1);
			}
			char *nome = argv[i+1];
			aEscrever.push_back(nome);
		}*/
	}
	if (!hasOne) {
		SimpleHelp();
		exit(1);
	}

	if (nameOutput.substr(nameOutput.length()-4) == ".png")
		*writer = new PNGWriter(resolution, (extendedBox?2:1)*box, nameOutput);
	else{
		if (!(nameOutput.substr(nameOutput.length()-4) == ".eps"))
			nameOutput+=".eps";
		if (extendedBox)
			*writer = new EPSWriter(-box, box, nameOutput);
		else
			*writer = new EPSWriter(0, box, nameOutput);

	}
}

void Draw(ImageWriter* writer, std::vector<PolyReader>& poly, bool extendedBox, bool drawDistance, double minDist, double lineWidth)
{
	for (int i=0; i<poly.size(); i++) 
		poly[i].updatePoly();

	Normalize(writer, poly, extendedBox);
	for (int i=0; i<poly.size(); i++) 
		poly[i].draw(writer);

	if (drawDistance) 
		connectPoly(writer, poly, minDist, lineWidth);
/*
	for (int i=0; i<aEscrever.size(); i++) {
		char *name=new char[aEscrever[i].size()+1];
		name[aEscrever[i].size()]=0;
		memcpy(name,aEscrever[i].c_str(),aEscrever[i].size());
		png.plot_text(font, 90, (int)(0.05*window_width), (int)(0.9*window_height-170*i), 0., name, 0., 0., 0.);
	}*/
}

void connectPoly(ImageWriter *writer, std::vector<PolyReader>& poly, double minDist, double lineWidth){

	cout << "Connecting points\n";

	int N = poly.size();
	for (int i=0; i<N;i++){
		for (int j=0; j<N; j++) {
			poly[i].drawLines(writer, poly[j]._poly, minDist, lineWidth);
		}
	}
}


void SimpleHelp() {
	
	cout << "If you find any bug, please write to lqcc@df.ufpe.br\n\n";
	cout << "POPS: 2.00Alpha\n\n";	
	std::cout << "Commands: \n\n";
	std::cout << " -h\n\tShow this help dialog\n\n"; 
	std::cout << " -s filename Quantidade\n\tAdd a new kind of square, with Quantity squares. To each square, there must be five lines on the file.\n\n";
	std::cout << " -t filename Quantidade\n\tAdd a new kind of triangle, with Quantity triangles. To each triangle, there must be four lines on the file.\n\n";
	std::cout << " -t filename Quantidade\n\tAdd a new kind of circle, with Quantity circles. To each circle, there must be one line on the file.\n\n";
	std::cout << " -o Name\n\tOutput filename.\n\n";
	std::cout << " -f Frame\n\tChooses the resolution of the output file. If this flag is not set, the resolution will be 1920x1920.\n\n";
	std::cout << " -b Box\n\tBox size. If unset, the value will be 10.\n\n";
	std::cout << " -r Distance\n\tDesenha draw a line between particle centers' if they are close than Distance.\n\n";
	std::cout << " -e\nThe standard is having the box centred on (0,0). If you wish its left-down corner to be on (0,0), set this flag.\t\n\n";
	std::cout << " -w width\n\tChooses linewidth, relative to box size. The standard is 0.1 box.\n\n";
}
