#ifndef MAP_H
#define MAP_H
#include "transistor.h"
#include <vector>
using namespace std;

bool check_parallel(Transistor A, Transistor B);

bool check_series(Transistor A, Transistor B);

vector<Transistor> remove_two_items(vector<Transistor> PDN, Transistor A, Transistor B);

string find_expression(vector<Transistor> PDN);

#endif
