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

/*
 *
 * This file sets the interface to imageWriter, and creates two instances,
 * one to write eps files, and one to write png files. 
 *
 * The imageWriter is needed abstract the specific interfaces of several 
 * image writing libraries. Indeed, if one wants to extend the available
 * formats to say, jpeg, one must create a imageWriter version to jpeg.
 *
 * */

#ifndef IMGEPSWRITER_HPP
#define IMGEPSWRITER_HPP

#include "imageWriter.hpp"
#include "../libepswriter/epswriter.hpp"
/*
 *
 * Implementation of ImageWrite, to make eps images
 *
 * */

struct EPSWriter: public ImageWriter {
	private:
		epswriter eps;
		double minBox;
		double maxBox;


	public:

		EPSWriter(double minBox_, double maxBox_, std::string name):minBox(0), maxBox(maxBox_-minBox_),eps(epswriter(name, 0,0, maxBox_-minBox_, maxBox_-minBox_)) {

			left = down = 0;
			right = up = maxBox_-minBox_;

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
		void writeText(double x, double y, std::string name, double fontSize, int red, int green, int blue)  {
			eps.writeText(x,y,name,fontSize*NormalizingFactor(), red,green,blue);
		};
		void printname() {
			std::cout << "Printing with epswriter" << std::endl;
		}
		void close() {
		}

};

#endif
