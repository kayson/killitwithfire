#ifndef FIREPRESETS_H
#define FIREPRESETS_H

class FirePresets
{
public:
	FirePresets();
	~FirePresets();
	data (*implicitFunction)(int x, int y, int z);

private:
	static const int GRID_DIM_X;
	static const int GRID_DIM_Y;
	static const int GRID_DIM_Z;
	static const double GRID_SIZE;
};


#endif //FIREPRESETS_H