 
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "transistor.h"
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
  			cout << "--------------------------------------\n" << "Fetching Information Pinage Information\n";
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
			float diff_width;
			int fingers;
			float gate_lenght;
			int stack;
  			
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

  			//========================================
  			cout << "Transistor: " << endl;
  			cout << "Alias:" << alias << endl;
  			cout << "Source:" << source << endl;
  			cout << "Gate:" << gate << endl;
  			cout << "Drain:" << drain << endl;
  			cout << "Bulk:" << bulk << endl;
  			cout << "Type:" << type << endl;
			//========================================

			while(lineStream >> token){
        		if (token.find("L=") != string::npos) {
        			token.erase(token.begin(),token.begin()+2);
        			cout << "Gate Lenght: " << token << endl; 
				}
				else if (token.find("W=") != string::npos)
				{
        			token.erase(token.begin(),token.begin()+2);
        			cout << "Width: " << token << endl; 
				}
				else if (token.find("F=") != string::npos)
				{
        			token.erase(token.begin(),token.begin()+2);
        			cout << "Fingers: " << token << endl;
				}
			}
			cout << endl;
  		}
  		//------------------------------------------------------------------
  		else if(token == ".SUBCKT"){
		//Just to ignore this line and not retrieve tokens
  		}
  		//------------------------------------------------------------------
  		else{

    	}
    	std::cout << "\n";
	}
    return 0;
    }