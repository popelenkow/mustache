#pragma once
#include <string>
#include <map>
#include <vector>
struct HashDatabase
{
	struct Boards
	{
		bool empty()
		{
			return left.empty() && right.empty();
		}
		std::string left;
		std::string right;
	};
	HashDatabase* get_next(const std::string &buf)
	{
		auto it = box.find(buf);
		if (it == box.end()) return nullptr;
		return it->second;
	}
	bool this_exists(const std::string &buf)
	{
		auto it = box.find(buf);
		if (it == box.end()) return false;
		return true;
	}
	std::string get_data(const std::string &buf)
	{
		auto it = data.find(buf);
		if (it == data.end()) return "";
		return it->second;
	}
	bool empty()
	{
		return boards.empty() && box.empty() && data.empty();
	}
	Boards boards;
	std::map<std::string, std::string> data;
	std::map<std::string, HashDatabase*> box;
};
struct HashText
{
	struct Cell
	{
		Cell()
		{
			second = nullptr;
		}
		std::string first;
		HashText *second;
	};
	HashText()
	{
		data_is_array = false;
	}
	bool data_is_array;
	std::vector<int> data;
	std::vector<int> negative;
	std::vector<int> positive;
	std::vector<Cell> box;
};
