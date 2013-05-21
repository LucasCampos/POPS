#ifndef IMAGEWRITER_HPP
#define IMAGEWRITER_HPP

#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include "vectorND.hpp"
#include "../libpngwriter/pngwriter.h"
#include "../libepswriter/epswriter.hpp"

struct ImageWriter {


	virtual double NormalizingFactor() const = 0;
	virtual double Center() const =0;
	virtual void line(double xfrom, double yfrom, double xto, double yto, double width, int red, int green, int blue)=0;
	virtual void multiline (const std::vector<double>& x, const std::vector<double>& y, double width, int red, int green, int blue)=0;

	virtual void triangle(double x1, double y1, double x2, double y2, double x3, double y3, int red, int green, int blue)=0;
	virtual void filledTriangle(double x1, double y1, double x2, double y2, double x3, double y3, int red, int green, int blue)=0;

	virtual void square(double xfrom, double yfrom, double xto, double yto, int red, int green, int blue)=0;
	virtual void filledSquare(double xfrom, double yfrom, double xto, double yto, int red, int green, int blue)=0;

	virtual void circle(double xcentre, double ycentre, double radius, int red, int green, int blue)=0;
	virtual void filledCircle(double xcentre, double ycentre, double radius, int red, int green, int blue)=0;

	virtual void printname()=0;
	virtual void close()=0;
};

struct PNGWriter:ImageWriter {
	private:
		pngwriter png;
		int resolution;
		double box;
		double normalizingFactor;

		void BasicLine(double xfrom, double yfrom, double xto, double yto, int red, int green, int blue) {
			png.line(xfrom, yfrom, xto, yto, red, green, blue);
		}

	public:

		PNGWriter(int resolution_, double box_, std::string name):resolution(resolution_), box(box_){
			png = pngwriter(resolution_, resolution_, 1.0, name.data());
			normalizingFactor = resolution_/box_;
		}

		~PNGWriter() {
			png.close();
		}
		double NormalizingFactor() const {
			return normalizingFactor;
		};

		double Center() const {
			return resolution/2.0;
		}
		void line(double xfrom, double yfrom, double xto, double yto, double width, int red, int green, int blue){
		
			const int realWidth = normalizingFactor*width;
			const int off=realWidth/2;
			const int x0off = xfrom-off;
			const int y0off = yfrom-off;
			const int xfoff = xto-off;
			const int yfoff = yto-off;

			for (int i=0; i<realWidth; i++) {
				for (int j=0; j<realWidth; j++) {

					png.line(x0off+i, y0off+j, xfoff+i, yfoff+j, red, blue, green);

				}
			}
		}


		void multiline (const std::vector<double>& x, const std::vector<double>& y, double width, int red, int green, int blue) {
			const int idxMax = std::min(x.size(), y.size());
			if (idxMax < 1)
				return;

			int xfrom = x[0], yfrom = y[0];
			for (int i=1; i<idxMax; i++) {
					line(xfrom, yfrom, x[i], y[i], width, red, green, blue);
					xfrom = x[i];
					yfrom = y[i];
			}
		}

		void triangle(double x1, double y1, double x2, double y2, double x3, double y3, int red, int green, int blue) {
			png.triangle(x1, y1, x2,y2, x3, y3, red, green, blue);
		}

		void filledTriangle(double x1, double y1, double x2, double y2, double x3, double y3, int red, int green, int blue) {
			png.filledtriangle(x1, y1, x2,y2, x3, y3, red, green, blue);
		}

		void square(double xfrom, double yfrom, double xto, double yto, int red, int green, int blue) {
			png.square(xfrom, yfrom, xto, yto, red, green, blue);
		}
		void filledSquare(double xfrom, double yfrom, double xto, double yto, int red, int green, int blue) {
			png.filledsquare(xfrom, yfrom, xto, yto, red, green, blue);
		}

		void circle(double xcentre, double ycentre, double radius, int red, int green, int blue) {
			png.circle(xcentre, ycentre, radius, red, green, blue);
		}
		void filledCircle(double xcentre, double ycentre, double radius, int red, int green, int blue) {
			png.filledcircle(xcentre, ycentre, radius, red, green, blue);
		}
		void printname() {
			std::cout << "Printing with pngwriter" << std::endl;
		}
		void close() {
			png.close();
		}

};

struct EPSWriter: public ImageWriter {
	private:
		epswriter eps;
		double minBox;
		double maxBox;
		double normalizingFactor;


	public:

		EPSWriter(double minBox_, double maxBox_, std::string name):minBox(minBox_), maxBox(maxBox_),eps(epswriter(name, minBox_, minBox_, maxBox_, maxBox_)){

		}

		~EPSWriter() {
		}
		double NormalizingFactor() const {
			return 1.0;
		};

		double Center() const {
			return (maxBox+minBox)/2.0;
		}

		void line(double xfrom, double yfrom, double xto, double yto, double width, int red, int green, int blue) {
			eps.line(xfrom, yfrom, xto, yto, width, red, green, blue);
		}
		void multiline (const std::vector<double>& x, const std::vector<double>& y, double width, int red, int green, int blue) {
			eps.multiline(x,y,width,red,green,blue);
		}

		void triangle(double x1, double y1, double x2, double y2, double x3, double y3, int red, int green, int blue) {
			eps.triangle(x1, y1, x2,y2, x3, y3, red, green, blue);
		}

		void filledTriangle(double x1, double y1, double x2, double y2, double x3, double y3, int red, int green, int blue) {
			eps.filledTriangle(x1, y1, x2,y2, x3, y3, red, green, blue);
		}

		void square(double xfrom, double yfrom, double xto, double yto, int red, int green, int blue) {
			eps.square(xfrom, yfrom, xto, yto, red, green, blue);
		}
		void filledSquare(double xfrom, double yfrom, double xto, double yto, int red, int green, int blue) {
			eps.filledSquare(xfrom, yfrom, xto, yto, red, green, blue);
		}

		void circle(double xcentre, double ycentre, double radius, int red, int green, int blue) {
			eps.circle(xcentre, ycentre, radius, red, green, blue);
		}
		void filledCircle(double xcentre, double ycentre, double radius, int red, int green, int blue) {
			eps.filledCircle(xcentre, ycentre, radius, red, green, blue);
		}
		void printname() {
			std::cout << "Printing with epswriter" << std::endl;
		}
		void close() {
		}

};

#endif