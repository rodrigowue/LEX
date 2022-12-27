#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <bits/stdc++.h>
#include "transistor.h"
#include "map.h"
using namespace std;



int main(int argc, char** argv)
	{
	string line;
	char delim[]=" ";
	vector<string> power_pins;
	vector<string> ground_pins;
	vector<string> in_pins;
	vector<string> out_pins;
	vector<string> common_nodes;
	vector<Transistor> PDN;
	vector<Transistor> PUN;

	if (argc < 1 ){
		cout << "call: ./main [*].sp" << endl;
	}
	ifstream myfile (argv[1]);

	if (0==(myfile.is_open())){
		cout << "Cannot open file:" << argv[1] << endl;
	}

	while(getline(myfile,line)){
    	stringstream lineStream(line);
    	string token;
    	lineStream >> token;
    	//------------------------------------------------------------------
    	// IF FIRST LINE TOKEN == "*.pininfo"
    	//------------------------------------------------------------------
  		if(token == "*.pininfo"){
  			cout << "--------------------------------------\n" << "Fetching Pinage Information\n";
  			while(lineStream >> token)
    		{
        		if (token.find(":I") != string::npos) {
        			token.erase(token.end()-2,token.end());
        			// cout << "INPUT:" << token << endl;
        			in_pins.push_back(token);
				}
				else if (token.find(":O") != string::npos)
				{
        			token.erase(token.end()-2,token.end());
        			// cout << "OUTPUT:" << token << endl;
        			out_pins.push_back(token);
				}
				else if (token.find(":P") != string::npos)
				{
        			token.erase(token.end()-2,token.end());
        			// cout << "POWER:" << token << endl;
        			power_pins.push_back(token);
				}
				else if (token.find(":G") != string::npos)
				{
        			token.erase(token.end()-2,token.end());
        			// cout << "GROUND:" << token << endl;
        			ground_pins.push_back(token);
				}
    		}
  		}
  		//------------------------------------------------------------------
  		//IF FIRST LINE TOKEN STARTS WITH "M" (Transistor)
  		//------------------------------------------------------------------
  		else if(token[0]=='M'){
        string alias;
        string source;
        string drain;
        string gate;
        string bulk;
        string type; //PMOS,NMOS,LVT,HVT
        double diff_width;
        int fingers=0;
        double gate_lenght;
        int stack=0;
        char * tail;

  			alias = token;
  			lineStream >> token;
  			source = token;
  			lineStream >> token;
  			gate = token;
  			lineStream >> token;
  			drain = token;
  			lineStream >> token;
  			bulk = token;
  			lineStream >> token;
  			type = token;

  			/*========================================
  			cout << "Transistor: " << endl;
  			cout << "Alias:" << alias << endl;
  			cout << "Source:" << source << endl;
  			cout << "Gate:" << gate << endl;
  			cout << "Drain:" << drain << endl;
  			cout << "Bulk:" << bulk << endl;
  			cout << "Type:" << type << endl;
			*/

			while(lineStream >> token){
        		if (token.find("L=") != string::npos) {

        			token.erase(token.begin(),token.begin()+2);
        			gate_lenght = strtod(token.c_str(),&tail);
        			//cout << "Gate Lenght: " << gate_lenght << endl;
				}
				else if (token.find("W=") != string::npos)
				{
        			token.erase(token.begin(),token.begin()+2);
        			diff_width = strtod(token.c_str(),&tail);
        			//cout << "Width: " << diff_width << endl;
				}
				else if (token.find("F=") != string::npos)
				{
        			token.erase(token.begin(),token.begin()+2);
        			fingers = atoi(token.c_str());
        			//cout << "Fingers: " << fingers << endl;
				}
			}
			if(type[0]=='P'){
				Transistor p_transistor(alias, source, drain, gate, bulk, type, diff_width, fingers, gate_lenght, stack);
				PUN.push_back(p_transistor);
				//cout << "PMOS ADDED TO PUN LIST" << endl;
			}
			else{
				Transistor n_transistor(alias, source, drain,	gate, bulk,	type, diff_width, fingers, gate_lenght, stack);
				PDN.push_back(n_transistor);
				//cout << "NMOS ADDED TO PDN LIST" << endl;
			}
  		}
  		//------------------------------------------------------------------
  		else if(token == ".SUBCKT"){
		//Just to ignore this line and not retrieve tokens
  		}
  		//------------------------------------------------------------------
  		else{

    	}
    	//std::cout << "\n";
	   }

	   //------------------------------------------------------------------
     int V = 5;
     vector<pair<int, int> > adj[V];
     int x=0;

     cout << "----------------------------------------" << endl;
     cout << "PMOS:" << endl;
     for (auto it = begin(PUN); it != end(PUN); ++it){
      cout << x << ":" << it->get_alias() << " " << it->get_source() << " " << it->get_drain() << endl;
      x++;
     }

     cout << "NMOS:" << endl;
     for (auto it = begin(PDN); it != end(PDN); ++it){
      cout << x << ":" << it->get_alias() << " " << it->get_source() << " " << it->get_drain() << endl;
      x++;
     }

     cout << "----------------------------------------" << endl;
	/* In the future we could iterate the PDN until there is only one transistor left. 
	At each iteration it will replace 2 transistors from the vector for a 
	macro transistor (by macro it is a normal transistor but the alias will be the expression like "(MN1.MN2)")
	So: If found a parallel or series pair, remove than and create another transistor*/

	//NOTE THAT THIS IS THE CMOS ARRANJMENT ONLY, THE ACTUAL EXPRESSION is negated of that:
	cout << "PDN Expression: " << find_expression(PDN) << endl;
	cout << "PUN Expression: " << find_expression(PUN) << endl;
	cout << "----------------------------------------" << endl;
	
	string expression;
	expression.append("!(");
	expression.append(find_expression(PUN));
	expression.append("*");
	expression.append(find_expression(PDN));
	expression.append(")");

	cout << "Expression: " << expression << endl; 
	cout << "----------------------------------------" << endl;
	cout << "TRUTH TABLE:" << endl;
	find_arcs(in_pins, expression);
	cout << "----------------------------------------" << endl;
	//replace_all(expression, "A", "1");
	//replace_all(expression, "B", "1");
	//replace_all(expression, "C", "1");
	//cout << "Expression: " << expression << endl; 
	//if (solve_boolean_expression(expression)==1){
	//	cout << "1" << endl;
	//}
    return 0;
    }

