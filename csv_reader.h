// csv_reader.h

#ifndef CSV_READER_H
#define CSV_READER_H	

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

class CSVReader
{
	string fileName;
	string delimeter;

    public:
        CSVReader(const string& filename, const string& delm = ",") {
                fileName = filename;
                delimeter = delm;
        }

        // Function to fetch data from a CSV File
        vector<vector<string>> getData();
};

#endif
