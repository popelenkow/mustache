#include "parser.h"

void ParserDatabase::create_box(std::istream &input, std::string &name, HashDatabase *&hash)
{
	Brackets it = find_brackets('{');
	char ch = skip_space(input);
	std::stringstream input_inside;
	if (ch == it.left)
	{
		std::string buf;
		get_inside(input, buf, it);
		input_inside << buf;
	}
	read_inside(input_inside, hash->box[name]);
}

void ParserDatabase::create_boards(std::string &str_input, HashDatabase *&hash)
{
	Brackets it = find_brackets('"');
	std::stringstream input(str_input);
	char ch = skip_space(input);
	if (it.left == ch) get_inside(input, hash->boards.left, it);
	else throw WrongDatabaseException();
	ch = skip_space(input);
	if (it.left == ch) get_inside(input, hash->boards.right, it);
	else throw WrongDatabaseException();
}

void ParserDatabase::create_date(std::string &str_input, HashDatabase *&hash)
{
	Brackets it_1 = find_brackets('"');
	Brackets it_2 = find_brackets('(');
	std::stringstream input(str_input);
	std::string key;
	std::string value;
	for (;;)
	{
		char ch = skip_space(input);
		if (input.eof()) return;
		if (it_1.left == ch) get_inside(input, key, it_1);
		else throw WrongDatabaseException();
		ch = skip_space(input);
		if (it_2.left == ch) get_inside(input, value, it_2);
		else throw WrongDatabaseException();
		hash->data[key] = value;
	}
}

void ParserDatabase::get_inside(std::istream &input, std::string &str_inside, Brackets braket)
{
	char ch;
	int index = 0;
	str_inside = "";
	for (;;)
	{
		ch = input.get();
		if (ch == braket.right) index--;
		else if (ch == braket.left) index++;
		if (index < 0)
		{
			return;
		}
		if (input.eof()) throw WrongDatabaseException();
		str_inside += ch;
	}
}

void ParserDatabase::read_box(std::istream &input, HashDatabase *&hash)
{
	std::string str_inside;
	char ch;
	for (;;)
	{
		str_inside.clear();
		ch = skip_space(input);
		if (input.eof()) return;
		bool is_ok = false;
		for (auto it = brackets.begin(); it != brackets.end(); it++)
		{
			if (ch == it->left)
			{
				is_ok = true;
				get_inside(input, str_inside, *it);
				if (it == brackets.end()) throw WrongDatabaseException();
				else if (it->left == '"')
				{
					create_box(input, str_inside, hash);
				}
				else if (it->left == '<')
				{
					create_boards(str_inside, hash);
				}
				else if (it->left == '[')
				{
					create_date(str_inside, hash);
				}
				break;
			}
		}
		if (!is_ok) throw WrongDatabaseException();
	}
}


bool ParserText::get_line(std::istream &input, std::string &str, std::string find)
{
	char ch;
	std::string buf;
	str.clear();
	while (!input.eof())
	{
		getline(input, buf, find[0]);
		if (!input.eof())
		{
			str += buf;
			bool is_ok = true;
			for (int i = 1; i < find.size(); i++)
			{
				ch = input.get();
				if (ch != find[i])
				{
					str += find[0];
					int it = input.tellg();
					input.seekg(it - i);
					is_ok = false;
					break;
				}
			}
			if (is_ok) return true;
		}
		else
		{
			str += buf;
		}
	}
	return false;
}

char ParserText::get_condition(std::istream &input)
{
	char ch = input.get();
	if ((ch == '#') || (ch == '^') || (ch == '?'))
	{
		return ch;
	}
	else
	{
		int it = input.tellg();
		input.seekg(it - 1);
		return '\0';
	}
}

void ParserText::read_box(std::istream &input, HashText *&hash)
{
	HashText::Cell buf;
	while (!input.eof())
	{
		if (get_line(input, buf.first, "{{"))
		{
			if (!buf.first.empty()) hash->box.push_back(buf);
			char ch = get_condition(input);
			if (!get_line(input, buf.first, "}}")) throw WrongTextException();
			hash->box.push_back(buf);
			HashText::Cell &it = hash->box[hash->box.size() - 1];
			if ((ch == '#') || (ch == '^') || (ch == '?'))
			{
				it.second = new HashText;
				if (ch == '?') ch = '#';
				else if (ch == '#') it.second->data_is_array = true;
				if (ch == '^') hash->negative.push_back(hash->box.size() - 1);
				else hash->positive.push_back(hash->box.size() - 1);
				std::string find = "{{/" + buf.first + "}}";
				if (!get_line(input, buf.first, find)) throw WrongTextException();
				std::stringstream input_inside(buf.first);
				read_box(input_inside, it.second);
				if (it.second->box.empty())
				{
					delete it.second;
					it.second = nullptr;
				}
			}
			else
			{
				hash->data.push_back(hash->box.size() - 1);
			}
		}
		else
		{
			if (!buf.first.empty()) hash->box.push_back(buf);
		}
	}
}
