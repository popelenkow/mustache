#include "mustache.h"

#include <sstream>
#include <fstream>
#include <iostream>

std::ifstream i_open(const std::string &str)
{
	std::ifstream input(str);
	if ( !input )
	{
		std::string buf = "Not opened \"" + str + '"';
		throw buf;
	}
	return input;
}

std::ofstream o_open(const std::string &str)
{
	std::ofstream input(str);
	if ( !input )
	{
		std::string buf = "Not opened \"" + str + '"';
		throw buf;
	}
	return input;
}

int main ()
{
	try
	{
		std::ifstream &in_database = i_open("database.txt");
		std::ifstream &in_text = i_open("input.txt");
		std::ofstream &out_text = o_open("output.txt");
		
		Mustache mustache;
		mustache.go(in_database, in_text, out_text);
		
		in_database.close();
		in_text.close();
		out_text.close();
	}
	catch (std::string err)
	{
		std::cerr << err << std::endl;
	}

	
	return 0;
}
