#include "translator.h"

bool Traslator::check_condition(std::stringstream &input, HashDatabase *&it_database)
{
	std::string buf;
	for (;;)
	{
		getline(input, buf, '.');
		if (input.eof())
		{
			if (buf.empty()) return true;
			else if (it_database->this_exists(buf))
			{
				it_database = it_database->get_next(buf);
				return true;
			}
			else return false;
		}
		it_database = it_database->get_next(buf);
		if (it_database == nullptr) return false;
	}
}

void Traslator::go_condition(HashDatabase *database_inside, HashText *text_inside, std::vector<int> &condition, bool is_positive)
{
	for (int i = 0; i < condition.size(); i++)
	{
		int pos = condition[i];
		HashText::Cell &it = text_inside->box[pos];
		std::stringstream input(it.first);
		HashDatabase *it_database = database;
		it.first.clear();
		if (is_positive == check_condition(input, it_database))
		{
			if (it_database != nullptr) it.first += it_database->boards.left;
			it.first += go(it_database, it.second);
			if (it_database != nullptr) it.first += it_database->boards.right;
		}
	}
}

std::string Traslator::go_data(HashDatabase *database_inside, HashText *text_inside)
{
	if (database_inside == nullptr)
	{
		for (int i = 0; i < text_inside->data.size(); i++)
		{
			int pos = text_inside->data[i];
			HashText::Cell &it = text_inside->box[pos];
			it.first.clear();
		}
		return get_ready_text(text_inside);
	}
	else
	{
		if (text_inside->data_is_array)
		{
			std::vector<std::string> buf;
			text_inside->data_is_array = false;
			for (int i = 0; i < text_inside->data.size(); i++)
			{
				int pos = text_inside->data[i];
				HashText::Cell &it = text_inside->box[pos];
				buf.push_back(it.first);
			}
			std::string str;
			for (auto it = database_inside->box.begin(); it != database_inside->box.end(); it++)
			{
				if (it != database_inside->box.begin()) str += '\n';
				if (it->second != nullptr) str += it->second->boards.left;
				str += go_data(it->second, text_inside);
				if (it->second != nullptr) str += it->second->boards.right;
				for (int i = 0; i < text_inside->data.size(); i++)
				{
					int pos = text_inside->data[i];
					HashText::Cell &it = text_inside->box[pos];
					it.first = buf[i];
				}
			}
			return str;
		}
		else
		{
			for (int i = 0; i < text_inside->data.size(); i++)
			{
				int pos = text_inside->data[i];
				HashText::Cell &it = text_inside->box[pos];
				it.first = database_inside->get_data(it.first);
			}
			return get_ready_text(text_inside);
		}
	}
}
