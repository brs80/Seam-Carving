/*
  PgmManager.cpp 

  Implements management needed for pgm seam carving. 
  This includes file IO and data reserving.
*/
#include "pgmmanager.hpp"

typedef std::vector< std::vector<int> > grid; 

PgmManager::PgmManager() {}
// helper for parseFile method
void PgmManager::loadFile(const std::string& name) {
  std::ifstream ifs(name.c_str());
  parseFile(ifs);
}
/* 
  void PgmManager::makeFile(const std::string&)
  Input is name of file.
  creates the processed pgm image file
*/ 
void PgmManager::makeFile(const std::string& name) {
    size_t lastindex = name.find_last_of("."); 
    std::string rawName = name.substr(0, lastindex); 
    std::string newName = rawName + "_processed.pgm"; 
    std::ofstream ofs(newName);
    ofs << header << std::endl;
    ofs << "# " << newName << std::endl;
    ofs << numcol << ' ' << numrow << std::endl;
    ofs << greyScale << std::endl;
    int numVal = 0;
    for (int row = 0; row < numrow; ++row) {
      for (int col = 0; col < numcol; ++col) {
        ofs << pgmVals[row][col];
        if (++numVal == 15) {
          numVal = 0;
          ofs << " " << std::endl;
          continue;
        }
        ofs << ' ';
      }
    }
}
void PgmManager::setPgm(const grid& newPgm) {
  pgmVals = newPgm;
  numcol = newPgm[0].size(); 
  numrow = newPgm.size(); 
}
/* 
  void parseFile(std::ifstream&)
  - parses the input file in Pgm format and puts into 2D vector
*/ 
void PgmManager::parseFile(std::ifstream& ifs) { 
    std::string line;
    std::stringstream ss;
    int row = 0, col = 0, numrows = 0, numcols = 0;
    std::getline(ifs, line);
    header = line; 
    if(line.compare("P2") != 0) { 
      std::cout << "No P2 in file"  << line << std::endl; 
    }
    std::getline(ifs,line);
    ss << ifs.rdbuf();
    ss >> numcols >> numrows;
    ss >> greyScale; 
    
    std::vector<int> rowVals; 
    int initial_value = 0; 
    pgmVals.resize(numrows, std::vector<int>(numcols, initial_value));
   
    int array[numrows][numcols];
    for(row = 0; row < numrows; ++row) {
      for (col = 0; col < numcols; ++col) { 
        ss >> array[row][col];
        pgmVals[row][col] = array[row][col]; 
      }
    }
    ifs.close();
}