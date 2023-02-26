#ifndef MAP_H
#define MAP_H
#include "transistor.h"
#include <vector>
using namespace std;

void print_logo();

vector<string> remove_pin(vector<string> pin_list, string pin);

void distribute_pins(vector<string>& common_nets, vector<string>& in_pins, vector<string>& out_pins);

bool check_parallel(Transistor A, Transistor B);

bool check_pg_pin(string pin, vector<string>& power_pins, vector<string>& ground_pins);

bool check_series(Transistor A, Transistor B, vector<string>& power_pins, vector<string>& ground_pins, string& common_net);

vector<Transistor> remove_two_items(vector<Transistor> PDN, Transistor A, Transistor B);

string find_expression_v2(int circuit_columns, string common_net, vector<Transistor> PDN, vector<string>& power_pins, vector<string>& ground_pins);

string find_expression(vector<Transistor> PDN);

string flatten_expression(vector<string> common_nets, vector<string> expressions);

int solve_boolean_expression(string expression);

void replace_all(std::string& s, std::string const& toReplace, std::string const& replaceWith);

vector<string> find_arcs(vector<string> in_pins, string expression);

vector<string> truth_table(vector<string> in_pins, string expression);

#endif
