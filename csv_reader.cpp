// csv_reader.cpp

#include "csv_reader.h"

/*
* Parses through csv file line by line and returns the data
* in vector of vector of strings.
*/
vector<vector<string>> CSVReader::getData()
{
	std::ifstream file(fileName);

	vector<vector<string>> dataList;

	string line = "";
	// Iterate through each line and split the content using delimeter
	while (getline(file, line))
	{
		vector<string> vec;
		boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
		dataList.push_back(vec);
	}
	// Close the File
	file.close();

	return dataList;
}