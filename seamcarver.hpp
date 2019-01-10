/*
	Definitions for SeamCarver.cpp file
	Seam Carving algorithm has 3 main parts 
		* Calculate energy of each pixel 
		* Calculate the cost of matrix 
		* Remove the Seams
*/
#ifndef SEAMCARVER_HPP
#define SEAMCARVER_HPP
typedef std::vector< std::vector<int> > grid; 

grid calcEnergy(const grid&);
grid calcCostV(const grid&);
grid calcCostH(const grid&);
void removeSeamsH(grid& pgm, const grid& cost);
void removeSeamsV(grid& pgm, const grid& cost);

#endif