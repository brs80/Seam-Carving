/*
  PgmManager.hpp 

  PgmManager will do file manipulation and data storage.
*/
#ifndef PGMMANAGER_HPP
#define PGMMANAGER_HPP
#include <vector> 
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <string> 
#include <iostream> 
#include <sstream>

typedef std::vector< std::vector<int> > grid;
 

class PgmManager {
 
public:
    PgmManager();   
    grid getPgm() { return pgmVals; }
    void setPgm(const grid&); 
    void loadFile(const std::string&);
    void makeFile(const std::string&); 
    int greyScale;
    std::string header;
    std::string comment; 
    int numrow;
    int numcol;
    grid pgmVals;
private:
    void parseFile(std::ifstream&);
};

#endif