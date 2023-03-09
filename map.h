#ifndef MAP_H
#define MAP_H
#include "transistor.h"
#include <vector>
using namespace std;

//------------------------- CLI -------------------------------------------
void print_logo();

void print_transistor(Transistor& t1);

//------------------------- Pin Related -----------------------------------
vector<string> remove_pin(vector<string> pin_list, string pin);

void distribute_pins(vector<string>& common_nets, vector<string>& in_pins, vector<string>& out_pins);

//-------------------------Checks------------------------------------------
bool check_parallel(Transistor& A, Transistor& B);

bool check_common_net(Transistor& T0, string& common_net);

bool check_pg_pin(string pin, vector<string>& power_pins, vector<string>& ground_pins);

bool check_series(Transistor& A, Transistor& B, vector<string>& power_pins, vector<string>& ground_pins, string& common_net);

//------------------------- Flattening ------------------------------------
//-- find and merge parallel
Transistor merge_parallel(Transistor A, Transistor B);
vector<Transistor> collapse_parallel(int circuit_columns, vector<Transistor>& transistor_network);

//-- find and merge parallel
Transistor merge_series(Transistor A, Transistor B, vector<string> power_pins, vector<string> ground_pins);
vector<Transistor> collapse_series(int circuit_columns, string common_net, vector<Transistor>& transistor_network, vector<string>& power_pins, vector<string>& ground_pins);

// find the expression.
string find_expression(int circuit_columns, string common_net, vector<Transistor> transistor_network, vector<string>& power_pins, vector<string>& ground_pins);

string flatten_expression(vector<string> common_nets, vector<string> expressions);


//------------------------- Boolean Solver --------------------------------
int solve_boolean_expression(string expression);

// ------------------------- Truth Table ---------------------------------
vector<string> truth_table(vector<string> in_pins, string expression);

// ------------------------- Finding ARCS ---------------------------------
vector<string> find_arcs(vector<string> in_pins, string expression);

// ------------------------- Misc ---------------------------------
void replace_all(std::string& s, std::string const& toReplace, std::string const& replaceWith);

#endif
