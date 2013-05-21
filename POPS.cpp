#include <boost/shared_ptr.hpp>
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

void Normaliza(ImageWriter* writer, std::vector<PolyReader>& poly, bool extendedBox);
void PegaParametros(int argc, char* argv[], ImageWriter** writer, std::vector<PolyReader>& poly, bool& drawDistance, double& minDist, bool& extendedBox);
void Draw(ImageWriter* writer, std::vector<PolyReader>& poly, bool extendedBox, bool drawDistance, double minDist, double lineWidth);
void connectPoly(ImageWriter *writer, std::vector<PolyReader>& poly, double minDist, double lineWidth);
void AjudaSimples();

int main(int argc, char* argv[])
{
	std::vector<PolyReader> poly;
	bool extendedBox = true;
	bool drawDistance = false;
	double minDist = 1e9;
	ImageWriter* writer;
	PegaParametros(argc, argv, &writer, poly, drawDistance, minDist, extendedBox);
	writer->printname();
	Draw(writer, poly, extendedBox, drawDistance, minDist, 0.1);
	writer->close();
}

void Normaliza(ImageWriter* writer, std::vector<PolyReader>& poly, bool extendedBox) {
	double factor = writer->NormalizingFactor();
	double center = writer->Center();

	Vector2D T(center,center);
	for (int i=0; i<poly.size(); i++){
		
		poly[i].rescale(factor);
		if (extendedBox)
			poly[i].translate(T);
	}
};

void PegaParametros(int argc, char* argv[], ImageWriter** writer, std::vector<PolyReader>& poly, bool& drawDistance, double& minDist, bool& extendedBox){

	bool temUm = false;
	std::string nameOutput;
	double box;
	double resolution=1920;

	if (argc == 1) {
		AjudaSimples();
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

			temUm = true;

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

			temUm = true;

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
	
			temUm = true;

		} else if (string(argv[i]) == "-o") {

			if (argc <= i+1) {
				cout << "Parametros -o incompletos\n";
				exit(1);
			}
			nameOutput= std::string(argv[i+1]);	
		} else if (string(argv[i]) == "-f") {

			if (argc <= i+1) {
				cout << "Parametros -f incompletos\n";
				exit(1);
			}
			resolution = atoi(argv[i+1]);	
		} else if (string(argv[i]) == "-b") {

			if (argc <= i+1) {
				cout << "Parametros -b incompletos\n";
				exit(1);
			}
			box = atof(argv[i+1]);	
		} else if (string(argv[i]) == "-r") {

			if (argc <= i+1) {
				cout << "Parametros -r incompletos\n";
				exit(1);
			}

			drawDistance = true;
			minDist = atof(argv[i+1]);
		} else if (string(argv[i]) == "-w") {

			if (argc <= i+1) {
				cout << "Parametros -r incompletos\n";
				exit(1);
			}

			lineWidth = atoi(argv[i+1]);
		} else if (string(argv[i]) == "-h") {
			AjudaSimples();
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
	if (!temUm) {
		AjudaSimples();
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

	Normaliza(writer, poly, extendedBox);
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

void AjudaSimples() {

	cout << "\nLucas Campos, Laboratório de Computação Científica, Departamento de Física, UFPE\n";
	cout << "Em caso de bugs, por favor mande um email para lqcc@df.ufpe.br\n\n";  
	cout << "Versao 2.00Alpha\n\n";	
	std::cout << "Comandos: \n\n";
	std::cout << " -h\n\tMostra o diálogo de ajuda\n\n"; 
	std::cout << " -s filename Quantidade\n\tAdiciona um novo tipo de quadrado, com Quantidade quadrados por frame. A cada quadrado deve ser associado 5 linhas do arquivo\n\n";
	std::cout << " -t filename Quantidade\n\tAdiciona um novo tipo de triangulo, com Quantidade triangulos por frame. A cada triangulo deve ser associado 4 linhas do arquivo\n\n";
	std::cout << " -c filename Quantidade Raio\nAdiciona um novo tipo de circulo, com Quantidade circulos por frame. A cada circulo deve ser associado 1 linha do arquivo\t\n\n";
	std::cout << " -o Name\n\tEscolhe o nome do arquivo de saida\n\n";
	std::cout << " -f Frame\n\tEscolhe a resolucao da imagem. Por padrao, 1920x1920\n\n";
	std::cout << " -b Box\n\tEscolhe o tamanho da caixa. Por padrao e 10\n\n";
	std::cout << " -r Distance\n\tDesenha uma linha entre particulas com distancia menor que Distance\n\n";
	std::cout << " -e Extended Box\n\tA caixa vai de 0 a box, ao inves de -box a box.\n\n";
	std::cout << " -w width\n\tEscolha a grossura das linhas. Por padrao, 5.\n\n";
}
