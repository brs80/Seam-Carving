/*
	seamcarver.cpp 
    Bradley Snyder 
    December 2017 
    Algorithms project 3

    Top level file for Seam Carving. 
    Seam Carving is resizing an image without making it look distorted.
    This is done by finding the energy of vertical seams(difference of neighbors)
    and removing the ones with the lowest energy.(such as whitespace) 

	using typedef for std::vector< std::vector<int>> 
	it's named grid to avoid clutter.
*/
#include "pgmmanager.cpp"
#include "seamcarver.hpp"
#include "pixel.hpp"
typedef std::vector< std::vector<int> > grid; 
/*  
	3 arguments expected in command line. 
	argv[1] : filename of a .pgm file (including .pgm extension)
	argv[2] : amount of vertical pixels to remove
	argv[3] : amount of horizontal pixels to remove 
*/
int main(int argc, char* argv[]) {

	if(argc != 4) {
		std::cout << "Wrong Input";
	}	
	else { 
		std::string filename = argv[1];
		int vSeams = std::atoi(argv[2]);
		int hSeams = std::atoi(argv[3]); 

		PgmManager manager; 
		manager.loadFile(filename);

		grid pgmData;
		pgmData = manager.getPgm();
		// remove number of Vertical seams 
		for(int v = 0; v < vSeams; ++v) {
			grid eGridV = calcEnergy(pgmData);
			grid cGridV = calcCostV(eGridV);
			removeSeamsV(pgmData, cGridV);	
 		}
 		// now remove the horizontal seams
 		grid newGrid = pgmData;
 		for(int h = 0; h < hSeams; ++h) { 
			grid eGridH = calcEnergy(newGrid); 
			grid cGridH = calcCostH(eGridH);
			removeSeamsH(newGrid, cGridH);
		} 
		manager.setPgm(newGrid);
		manager.makeFile(filename);
	}
	return 0; 
}
/*
	grid calcEnergy(const grid&) 

	This function takes in a pgm matrix.
	Returns a same size matrix with the energies of each pixel 
	The energy of pixel is the sum of the differences of pgm values
	at the four surrounding areas. 
*/	
grid calcEnergy(const grid& pgm) { 

 	int height = (int) pgm.size(); 
	int width = (int) pgm[1].size();
	grid eData = pgm; 

    for (unsigned int h=0; h < height; ++h) { // height 
    	for (unsigned int w=0; w < width; ++w) { // width

    		int temp = pgm[h][w];  		
    		int left   =  w > 0 		? temp - pgm[h][w-1] : 0;
        	int right  =  w < width - 1 ? temp - pgm[h][w+1] : 0;
        	int top    =  h > 0         ? temp - pgm[h-1][w] : 0;	
        	int bottom =  h < height - 1? temp - pgm[h+1][w] : 0;
	        	
	        left   = std::abs(left);
	        right  = std::abs(right);
	        top    = std::abs(top);
	        bottom = std::abs(bottom); 

	        int energy = left + right + top + bottom;
	        eData[h][w] = energy; 
    	}
    	
    }
	return eData;  
}
/*
	grid calcCostV(const grid&) 
	
	Takes in our 2D matrix of our energies: const grid&.
	This function returns a same size 2D matrix(grid) with the costs for vertical seamcarving 
	The cost is the sum of energy values going to that point. 
	This is needed for seam removal.
*/ 
grid calcCostV(const grid& pgm) {

 	int height = (int) pgm.size(); 
	int width = (int) pgm[1].size();
	grid newGrid = pgm; 

	for (unsigned int h = 0; h < height; ++h) {
		for (unsigned int w = 0; w < width; ++w) {
	        if (h < 1) {
	          newGrid[0][w] = pgm[0][w];
	          continue;	
	        }
	        int prev = h - 1; 

	        const int curVal = pgm[h][w];
	        const Pixel neighbor(newGrid[prev][w]);
	        Pixel left;
	        Pixel right;
			if (w > 0) 
				left.setVal(newGrid[prev][w-1]);			
	        if (w < width - 1)
				right.setVal(newGrid[prev][w+1]);
	        Pixel temp = left < neighbor ? left : neighbor;
	        Pixel minVal = std::min(temp, right);
	        newGrid[h][w] = curVal + minVal.getVal();
	    }
	}
	return newGrid;
}
/*
	grid calcCostH(const grid&) 
	
	Takes in our 2D matrix of our energies: const grid&.
	This function returns a same size 2D matrix(grid) with the costs for horizontal Seamcarving 
	The cost is the sum of energy values going to that point. 
	This is needed for seam removal.
*/ 
grid calcCostH(const grid& pgm) {
	
 	int height = (int) pgm.size(); 
	int width = (int) pgm[1].size();
	grid newGrid = pgm; 
	//grid newGrid(height);  
	//for (int i=0;i < height;i++)
		//newGrid[i].resize(width);

	for (unsigned int w = 0; w < newGrid[0].size(); ++w) {
		for (unsigned int h = 0; h < newGrid.size(); ++h) {
	        if (w < 1) {
	          newGrid[h][0] = pgm[h][0];
	          continue;	
	        }
	        int prev = w - 1; 
	 
	        const int curVal = pgm[h][w];
	        const Pixel neighbor(newGrid[h][prev]);
	        Pixel top;
	        Pixel bottom;
			if (h > 0) {
				top.setVal(newGrid[h-1][prev]);
			}

	        if (h < newGrid.size() - 1) {
				bottom.setVal(newGrid[h+1][prev]);
	        }

	        int temp = top.getVal() < neighbor.getVal() ? top.getVal() : neighbor.getVal();
	        
	        int minVal = temp < bottom.getVal() ? temp : bottom.getVal(); 
	        newGrid[h][w] = curVal + minVal;
	        //std::cout << newGrid[w][h] << std::endl;
	    }
	}
	return newGrid;
}
/*
	void removeSeamsH(grid&, const grid&) 
	This function takes in a pgm image and the respective cost matrix.
	It will remove one horizontal seam.
*/
void removeSeamsH(grid& pgm, const grid& cost) { 
	unsigned int next = 0;
 	int height = (int) pgm.size(); //height of pgm
	int width = (int) pgm[0].size(); // width of pgm
 	int heightC = (int) cost.size(); // height of cost matrix
	int widthC = (int) cost[0].size(); // width of cost 
	// first locate smallest value for a starting point  
	for (unsigned int h = 0; h < heightC; ++h) {
		int val = cost[h][widthC-1];
      	int curVal = cost[next][widthC-1];
		if (val < curVal) {
			next = h;
		}
    }
    // travel from the start and move the smallest pixel to the end for removal
    for (unsigned int w = width - 1; w + 1 > 0; --w) {
		unsigned int newNext = 0;
		if (w > 0) {
			int left = w - 1;
	        Pixel above;
	        Pixel below;
	        const Pixel center(cost[next][left]);
	        if (next > 0) {
	        	above.setVal(cost[next-1][left]);
	        }
	        if (next < heightC - 1) {
				below.setVal(cost[next+1][left]);
	        }
	        int temp = std::min(above.getVal(), center.getVal()); 
	        int minVal = std::min(temp, below.getVal());

	        if (minVal == above) {
	          newNext = next - 1;
	        } else if (minVal == below) {
	          newNext = next + 1;
	        } else {
	          newNext = next;
	        }
      } 
      // Now shift all the values up one 
		for (unsigned int h = next + 1; h < height; ++h) {
		pgm[h-1][w] = pgm[h][w]; 
      }
      next = newNext;
    }
    pgm.resize(pgm.size() - 1); // delete the last row 
  }
/*
	void removeSeamsV(grid&, const grid&) 
	This function takes in a pgm image and the respective cost matrix.
	It will remove one vertical seam.
*/
void removeSeamsV(grid& pgm, const grid& cost) { 
	grid newGrid = pgm;
	unsigned int next = 0;
 	int height = (int) pgm.size(); // height of the pgm matrix 
	int width = (int) pgm[0].size(); // width of the pgm matrix 
 	int heightC = (int) cost.size(); // height of cost matrix
	int widthC = (int) cost[0].size(); // width of cost matrix
	// loop iterates through bottom row, finding start of seam to remove
    for (unsigned int w = widthC - 1; w + 1 > 0; --w) {
    	int val = cost[heightC-1][w]; 
    	int curVal = cost[heightC-1][next];
    	if (val < curVal) {
    		next = w;
      	}
    }
    // travel up through seam and shift the values 
    for (unsigned int h = height - 1; h + 1 > 0; --h) {
		unsigned int newNext = 0;
		if (h > 0) {
	        int up = h - 1;
	        Pixel left;
	        Pixel right;
	        const Pixel center(cost[up][next]);
	        if (next > 0) 
				left.setVal(cost[up][next-1]);
	        if (next < widthC - 1) 
				right.setVal(cost[up][next+1]);

	        Pixel temp = left < center ? left : center; 
	        Pixel minVal = temp < right? temp : right;
	        if (minVal == left) {
	          newNext = next - 1;
	        } else if (minVal == center) {
	          newNext = next;
	        } else {
	          newNext = next + 1;
	        }
    	} // shift all the values over
    	for (unsigned int w = next + 1; w < width; ++w) {
    		pgm[h][w-1]	= pgm[h][w];
      	}
		next = newNext;
    }
    for (auto& entry : pgm) {
		entry.resize(entry.size()-1); // resize by one after seam is removed
	}
}

