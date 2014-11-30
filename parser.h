#pragma once
#include "hash.h"
#include "exception.h"
#include <sstream>
#include <fstream>
#include <iostream>
class ParserDatabase
{
	struct Brackets
	{
		char left;
		char right;
	};
	char skip_space(std::istream &input)
	{
		int ch;
		do
		{
			ch = input.get();
		} while (isspace(ch));
		return ch;
	}

	Brackets find_brackets(char ch)
	{
		auto it = brackets.begin();
		for (; it != brackets.end(); it++)
		{
			if (it->left == ch) break;
		}
		return *it;
	}

	void create_box(std::istream &input, std::string &name, HashDatabase *&hash);

	void create_boards(std::string &str_input, HashDatabase *&hash);

	void create_date(std::string &str_input, HashDatabase *&hash);

	void get_inside(std::istream &input, std::string &str_inside, Brackets braket);

	void read_inside(std::istream &input_inside, HashDatabase *&hash_inside)
	{
		hash_inside = new HashDatabase;
		read_box(input_inside, hash_inside);
		if (hash_inside->empty())
		{
			delete hash_inside;
			hash_inside = nullptr;
		}
	}

	void read_box(std::istream &input, HashDatabase *&hash);

	std::vector<Brackets> brackets;
public:
	ParserDatabase() : brackets({ { '"', '"' }, { '(', ')' }, { '<', '>' }, { '[', ']' }, {'{', '}'} })
	{
	}
	void read(std::istream &input, HashDatabase *&hash)
	{
		read_box(input, hash);
	}
};


class ParserText
{
	bool get_line(std::istream &input, std::string &str, std::string find);

	char get_condition(std::istream &input);

	void read_box(std::istream &input, HashText *&hash);
public:
	void read(std::istream &input, HashText *&hash)
	{
		read_box(input, hash);
	}
};
