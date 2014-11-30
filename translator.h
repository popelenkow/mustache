#pragma once
#include "hash.h"
#include <iostream>
#include <sstream>
class Traslator
{
	bool check_condition(std::stringstream &input, HashDatabase *&it_database);

	std::string get_ready_text(HashText *text_inside)
	{
		std::string buf = "";
		for (auto it = text_inside->box.begin(); it != text_inside->box.end(); it++)
		{
			buf += it->first;
		}
		return buf;
	}

	void go_condition(HashDatabase *database_inside, HashText *text_inside, std::vector<int> &condition, bool is_positive);

	std::string go_data(HashDatabase *database_inside, HashText *text_inside);

	std::string go(HashDatabase *database_inside, HashText *text_inside)
	{
		go_condition(database_inside, text_inside, text_inside->positive, true);
		go_condition(database_inside, text_inside, text_inside->negative, false);
		return go_data(database_inside, text_inside);
	}
	HashDatabase *database;
	HashText *text;
public:
	std::string translate(HashDatabase *_database, HashText *_text)
	{
		database = _database;
		text = _text;
		return go(database, text);
	}
};
