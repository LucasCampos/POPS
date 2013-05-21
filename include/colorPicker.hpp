#ifndef COLOR_PICKER_H
#define COLOR_PICKER_H

struct Color {
	int red, blue, green;
	Color(): red(0), blue(0), green(0) {};
	Color(int r, int g, int b): red(r), blue(b), green(g) {};
};

class ColorPicker{
	static int current;
	public:
	static Color getColor() {

		switch(current) {
			case 0:
				current++;
				return Color(65535,0,0);
				break;
			case 1: 
				current++;
				return Color(0.0,65535,.0);
				break;
			case 2: 
				current++;
				return Color(0.0,0.0,65535);
				break;
			default: return Color (rand()%65535, rand()%65535, rand()%65535);
		}
	}
};

int ColorPicker::current = 0;

#endif
