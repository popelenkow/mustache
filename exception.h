#pragma once

#include <string>


struct Exception
{
	virtual std::string what(){return "";}
};

struct WrongDatabaseException : public Exception
{
	std::string what() {return "Wrong database";}
};

struct WrongTextException : public Exception
{
	std::string what() {return "Wrong text";}
};
