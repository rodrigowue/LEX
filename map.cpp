#include "map.h"
#include "transistor.h"
#include <unordered_set>
#include <vector>
#include <math.h>
using namespace std;

void print_logo(){
cout << "======================================" << endl;
cout << " SPICE STD-CELL TIMING ARCS EXTRACTOR" << endl;
cout << "       [UNDER DEVELOPMENT]" << endl;
cout << "======================================" << endl;
};

void print_transistor(Transistor& t1){
	cout << "-------------------------" << endl;
	cout << "Alias:" << t1.get_alias() << endl;
	cout << "source:" << t1.get_source() << endl;
	cout << "gate:" << t1.get_gate() << endl;
	cout << "drain:" << t1.get_drain() << endl;
	cout << "-------------------------" << endl;

};

vector<string> remove_pin(vector<string> pin_list, string pin){
	for (auto it_rm = begin(pin_list); it_rm != end(pin_list); ){
		if (*it_rm == pin){
			//cout << "deleting " << *it_rm << " from the list" << endl;
    		pin_list.erase(it_rm);
			}
		else{
			it_rm++;
			}
		}

	return pin_list;
}

void distribute_pins(vector<string>& common_nets, vector<string>& in_pins, vector<string>& out_pins){
	vector<string> in_pins_tmp;
	for (string common_net: common_nets){
		for (string pin : in_pins){
			if(common_net == pin){
				in_pins = remove_pin(in_pins, pin);
				out_pins.push_back(pin);
				if (in_pins.size() == 1){
					break;
				}
				else{
					distribute_pins(common_nets, in_pins, out_pins);
					break;
				}
			}
			
		}
	}
	return;
};

// -----------------------------------------------------------------------------------
//                            CHECKS
// -----------------------------------------------------------------------------------

bool check_parallel(Transistor& A, Transistor& B){
	if ((( A.get_source() == B.get_source() ) && ( A.get_drain() == B.get_drain())) | (( A.get_drain() == B.get_source() ) && ( A.get_source() == B.get_drain()))){
		return true;
	}
	else{
		return false;
	}
}

bool check_common_net(Transistor& T0, string& common_net){
	if (((T0.get_source() == common_net)|(T0.get_drain() == common_net))){
		return true;
	}
	else{
		return false;
	}
}

bool check_pg_pin(string pin, vector<string> power_pins, vector<string> ground_pins){
		for (string power_pin : power_pins){
			if(pin == power_pin){
				return false;
			}
		}
		for (string ground_pin : ground_pins){
			if(pin == ground_pin){
				return false;
			}
		}
		return true;
}

bool check_series(Transistor& A, Transistor& B, vector<string> power_pins, vector<string> ground_pins){
	string a_src = A.get_source();
	string b_src = B.get_source();
	string a_dra = A.get_drain();
	string b_dra = B.get_drain();
	if ( (( a_src == b_src ) & (check_pg_pin(a_src, power_pins, ground_pins)) & (a_dra != b_dra)) |
		 (( a_src == b_dra ) & (check_pg_pin(a_src, power_pins, ground_pins)) & (a_dra != b_src)) |
		 (( a_dra == b_src ) & (check_pg_pin(a_dra, power_pins, ground_pins)) & (a_src != b_dra)) |
		 (( a_dra == b_dra ) & (check_pg_pin(a_dra, power_pins, ground_pins)) & (a_src != a_src)) ){
		return true;
	}
	else{
		return false;
	}
}

// -----------------------------------------------------------------------------------
//                            FLATTENING
// -----------------------------------------------------------------------------------

Transistor merge_parallel(Transistor& A, Transistor& B){
		string type = "";
		string alias = "";
		string bulk = "";
		string source = "";
		string drain = "";
		int fingers = 0;
		double diff_width = 0.0;
		double gate_lenght = 0.0;
		int stack = A.get_stack();
		alias.append("(");
		alias.append(A.get_gate());
		alias.append("+");
		alias.append(B.get_gate());
		alias.append(")");
        source = A.get_source();
		drain  = A.get_drain();
		Transistor group_transistor(alias, source , drain, alias, bulk, type, diff_width, fingers, gate_lenght, stack);
		return group_transistor;
}

Transistor merge_series(Transistor& A, Transistor& B, vector<string>& power_pins, vector<string>& ground_pins){
		string type = "";
		string alias = "";
		string source = "";
		string bulk = "";
		string drain = "";
		int fingers=0;
		double diff_width = 0.0;
		double gate_lenght = 0.0;
		int stack = A.get_stack() + B.get_stack();
		string a_src = A.get_source();
		string b_src = B.get_source();
		string a_dra = A.get_drain();
		string b_dra = B.get_drain();
		//set alias for the new gate
		alias.append("(");
		alias.append(A.get_gate());
		alias.append("*");
		alias.append(B.get_gate());
		alias.append(")");
   		//Find the connecting point and preserve the connection
   		if ( a_src == b_src){
			source = a_dra;
			drain  = b_dra;
        	}
        else if(a_src == b_dra){
        	source = b_src;
			drain  = a_dra;
        	}
        else if(a_dra == b_src){
        	source = a_src;
			drain  = b_dra;
        	}
        else{
        	source = a_src;
			drain  = b_src;
        }
		Transistor group_transistor(alias, source, drain, alias, bulk, type, diff_width, fingers, gate_lenght, stack);
		return group_transistor;
}

vector<Transistor> collapse_parallel(int circuit_columns, vector<Transistor>& transistor_network){
	for (int i = 0; i < transistor_network.size() - 1; i++) {
        Transistor& t1 = transistor_network[i];
        for (int j = i + 1; j < transistor_network.size(); j++) {
            Transistor& t2 = transistor_network[j];
	  				if ((check_parallel(t1,t2))){
						Transistor group_transistor = merge_parallel(t1, t2);
						transistor_network.push_back(group_transistor); // insert the merged item
						transistor_network.erase(transistor_network.begin() + j);
                		transistor_network.erase(transistor_network.begin() + i);
						if(transistor_network.size() == circuit_columns){
							return transistor_network;
						}
						else{
							i = -1;
							break;
						}
						
	  				}
		}
	}
	return transistor_network;
}

vector<Transistor> collapse_series(int circuit_columns, vector<Transistor>& transistor_network, vector<string>& power_pins, vector<string>& ground_pins){
	for (int i = 0; i < transistor_network.size() - 1; i++) {
        Transistor& t1 = transistor_network[i];
        for (int j = i + 1; j < transistor_network.size(); j++) {
            Transistor& t2 = transistor_network[j];
	  				if ((check_series(t1, t2, power_pins, ground_pins))){
					Transistor group_transistor = merge_series(t1, t2, power_pins, ground_pins);
					transistor_network.push_back(group_transistor); // insert the merged item
					transistor_network.erase(transistor_network.begin() + j);
                	transistor_network.erase(transistor_network.begin() + i);
					if(transistor_network.size() == circuit_columns){
						return transistor_network;
					}
					else{
						i = -1;
						break;
					}
	  			}
			}
	}
	return transistor_network;
}

vector<string> find_expression(int circuit_columns, vector<string> common_nets, vector<Transistor> transistor_network, vector<string>& power_pins, vector<string>& ground_pins){
	vector<string> expressions;
	vector<Transistor> temp_transistor_network = transistor_network;
	//collapse until its is done
	int it_count = 0;
	while ((temp_transistor_network.size() > circuit_columns) & (it_count < 1000)){
		//Find Parrallel Transistors and Collapse them into Pseudo Transistors
		collapse_parallel(circuit_columns, temp_transistor_network);
		//If the number of pseudo transistors is the same as the amount of common nets
		if(temp_transistor_network.size() == circuit_columns){
			break;
		}
		else{
			//Find Series Transistors and Collapse them into Pseudo Transistors
			collapse_series(circuit_columns, temp_transistor_network, power_pins, ground_pins);
		}
		it_count++;
	}
	
	for(string common_net: common_nets){
		for(int i = 0; i < temp_transistor_network.size(); i++){
			//print_transistor(temp_transistor_network[i]);
			//cout << common_net << endl;
			if(check_common_net(temp_transistor_network[i], common_net)){
					temp_transistor_network[i].set_alias(temp_transistor_network[i].get_gate());
					expressions.push_back(temp_transistor_network[i].get_alias());
				}
			}
	}
	return expressions;
}

string flatten_expression(vector<string> common_nets, vector<string> expressions){
	string expression = expressions.front();
	if (expressions.size() > 1){
		for (int i = 0 ; i < common_nets.size(); i++){
			for (int it2 = 0; it2 < expressions.size(); ++it2){
			if(expressions[it2].find(common_nets[i]) != string::npos){
				//cout << "it2:" << *it2 << endl;
				//cout << "exp:" << expressions.at(i) << endl;
				//cout << "commo:" << common_nets.at(i) << endl;
				string temp = expressions[it2];
				string what_it_is =  common_nets[i];
				string what_it_will_be = "!(" + expressions[i] + ")";
				replace_all(temp, what_it_is, what_it_will_be);
				cout << "Result:" << temp << endl;
				expressions.erase(expressions.begin() + it2);	
				expression = flatten_expression(common_nets, expressions);
			}
			}
		}	
	}
	return expression;
}


// -----------------------------------------------------------------------------------
//                            SOLVER
// -----------------------------------------------------------------------------------

int solve_boolean_expression(string expression){
	if (expression.size() > 1){
		replace_all(expression, "0+0", "0");
		replace_all(expression, "0+1", "1");
		replace_all(expression, "1+0", "1");
		replace_all(expression, "1+1", "1");

		replace_all(expression, "0*0", "0");
		replace_all(expression, "0*1", "0");
		replace_all(expression, "1*0", "0");
		replace_all(expression, "1*1", "1");

		replace_all(expression, "!1", "0");
		replace_all(expression, "!(1)", "0");
		replace_all(expression, "!0", "1");
		replace_all(expression, "!(0)", "1");
		replace_all(expression, "(0)", "0");
		replace_all(expression, "(1)", "1");
		return solve_boolean_expression(expression);
		}
	else{
		return atoi(expression.c_str());
	}
	return atoi(expression.c_str());
}

void replace_all(
    std::string& s,
    std::string const& toReplace,
    std::string const& replaceWith
) {
    std::string buf;
    std::size_t pos = 0;
    std::size_t prevPos;

    // Reserves rough estimate of final size of string.
    buf.reserve(s.size());

    while (true) {
        prevPos = pos;
        pos = s.find(toReplace, pos);
        if (pos == std::string::npos)
            break;
        buf.append(s, prevPos, pos - prevPos);
        buf += replaceWith;
        pos += toReplace.size();
    }

    buf.append(s, prevPos, s.size() - prevPos);
    s.swap(buf);
}

// -----------------------------------------------------------------------------------
//                            ARC FINDER
// -----------------------------------------------------------------------------------

vector<string> find_arcs(vector<string> in_pins, string expression){
	vector<string> arcs;
	string expr = expression;
  	unordered_set<char> literals;
  
  for (char c : expr) {
    if (isalpha(c)) {
      literals.insert(c);
    }
  }

  int numLiterals = literals.size();
  cout << "Number of literals: " << numLiterals << endl;


  int maxVal = 1 << numLiterals;
  bool values[numLiterals];

  for (int i = 0; i < maxVal; i++) {
    // Set the values of the literals
    int t = i;
    int j = 0;
    for (char c : literals) {
      values[j++] = t & 1;
      t >>= 1;
    }

    // Evaluate the boolean expression
    string local_expression = expr;
    
    for (size_t i = 0; i < local_expression.size(); ++i) {
        int it = 0;
        for (char c : literals) {
            if (local_expression[i] == char(c)) {
                local_expression.replace(i, 1, to_string(values[it]));
            }
            it++;
        }
    }
    
    bool result = solve_boolean_expression(local_expression);
    //bool result = !((values[0] + values[1]) * (values[0] * values[1]));
    
    // Check for transition arcs
    int k = 0;
    for (char c : literals) {
      int t = i ^ (1 << k);
      int l = 0;
      for (char d : literals) {
        values[l++] = t & 1;
        t >>= 1;
      }
      string local_expression = expr;
      for (size_t i = 0; i < local_expression.size(); ++i) {
        int it = 0;
        for (char c : literals) {
            if (local_expression[i] == char(c)) {
                local_expression.replace(i, 1, to_string(values[it]));
            }
            it++;
        }
    }
    
      //cout << local_expression << endl;
      bool newResult = solve_boolean_expression(local_expression);

      if (newResult != result) {
		int counter = 0;
		string arc = "";
        for(char c1: literals){
			if(c1 == c){
			if (values[k]) {
          		cout << "F";
				arc += "F";
        	} else {
          		cout << "R";
				arc += "R";
        	}
			}
			else{
				cout << values[counter];
				if (values[counter] == 1){
					arc += "1";
				}
				else{
					arc += "0";
				}
			}
			counter++;
			cout << " " ;
		}
        cout << " | ";
        if (newResult) {
          cout << "Fall";
		  arc += "F";
        } else {
          cout << "Rise";
		  arc += "R";
        }
		arcs.push_back(arc);
        cout << endl;
      }

      k++;
    }
  }
	return arcs;

}

vector<string> truth_table(vector<string> in_pins, string expression){
	vector<string> arcs;
	int counter = 0;
	int amount_of_inputs = pow(2,(in_pins.size()));
	while (counter < amount_of_inputs){
		string local_expression = expression;
		for (auto it = begin(in_pins); (it != end(in_pins)); ++it){
			int index = it - in_pins.begin();
			int teste = (counter >> index) & 1;
			replace_all(local_expression, *it, to_string(teste));
			cout << teste;
		}
		if (solve_boolean_expression(local_expression) == 1){
			cout << "|" << 1  << endl;
		}
		else{
			cout << "|" << 0  << endl;
		}
		counter++;
	}
	return arcs;

}