#pragma once
#include "hash.h"
#include "parser.h"
#include "traslator.h"
class Mustache
{
	static void destroy(HashDatabase *&hash)
	{
		if (hash == nullptr) return;
		auto &box = hash->box;
		for (auto it = box.begin(); it != box.end(); it++)
		{
			destroy(it->second);
		}
		delete hash;
		hash = nullptr;
	}
	static void destroy(HashText *&hash)
	{
		if (hash == nullptr) return;
		auto &box = hash->box;
		for (auto it = box.begin(); it != box.end(); it++)
		{
			destroy(it->second);
		}
		delete hash;
		hash = nullptr;
	}
	ParserDatabase parser_database;
	ParserText parser_text;
	Traslator traslator;
	HashText *text;
	HashDatabase *database;
public:
	Mustache() : parser_database()
	{
		database = new HashDatabase;
		text = new HashText;
	}
	~Mustache()
	{
		destroy(database);
		destroy(text);
	}
	void go(std::istream &input_database, std::istream &input_text, std::ostream &out_text)
	{
		try
		{
			parser_database.read(input_database, database);
			parser_text.read(input_text, text);
			out_text << traslator.translate(database, text);
		}
		catch (Exception &err)
		{
			std::cerr << err.what() << std::endl;
		}
	}
};
