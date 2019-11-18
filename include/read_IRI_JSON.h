#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/detail/exceptions.hpp>
using nlohmann::json;

struct txs_change
{
	unsigned long all_tx = 0;
	unsigned long new_tx = 0;
	unsigned long requ_tx = 0;
	unsigned long sent_tx = 0;
};

class read_IRI_JSON
{
public:
	virtual ~read_IRI_JSON();

	bool get_Neighbors_refresh(std::string json_string);
	static void get_Neighbors_once(std::string json_string, bool complete = false);
	static void get_Info(std::string json_string, bool complete = false);

private:
	json* pNeighbors = nullptr;
	txs_change* pPrev_tx = nullptr;
};

