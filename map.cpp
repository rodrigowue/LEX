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

void distribute_pins(vector<string>& common_nodes, vector<string>& in_pins, vector<string>& out_pins){
	vector<string> in_pins_tmp;
	for (auto it = begin(common_nodes); it != end(common_nodes); ++it){
		for (auto it2 = begin(in_pins); it2 != end(in_pins); ++it2){
			if(*it == *it2){
				in_pins = remove_pin(in_pins, *it2);
				out_pins.push_back(*it2);
				if (in_pins.size() == 1){
					break;
				}
				else{
					distribute_pins(common_nodes, in_pins, out_pins);
					break;
				}
			}
			
		}
	}
	return;
};

bool check_parallel(Transistor A, Transistor B){
	if ((( A.get_source() == B.get_source() ) && ( A.get_drain() == B.get_drain())) | (( A.get_drain() == B.get_source() ) && ( A.get_source() == B.get_drain()))){
		return true;
	}
	else{
		return false;
	}
}

bool check_series(Transistor A, Transistor B){
	if ((( A.get_source() == B.get_source() ) | ( A.get_drain() == B.get_drain())) | (( A.get_source() == B.get_drain() ) | ( A.get_drain() == B.get_source()))){
		return true;
	}
	else{
		return false;
	}
}

vector<Transistor> remove_two_items(vector<Transistor> PDN, Transistor A, Transistor B){
	for (auto it_rm = begin(PDN); it_rm != end(PDN); ){
		if (it_rm->get_alias() == B.get_alias()){
    		PDN.erase(it_rm);
			}
		else if(it_rm->get_alias() == A.get_alias()){
			PDN.erase(it_rm);
			}
		else{
			it_rm++;
			}
		}

	return PDN;
}

string find_expression(vector<Transistor> PDN){
	vector<Transistor> PDN_TEMP = PDN;
    string alias = "";
    string source = "";
    string drain = "";
    string gate = "";
    string bulk = "";
    string type = "";
	double diff_width = 0.0;
    int fingers=0;
    double gate_lenght = 0.0;
    int stack=0;
	string expression;
	int size = PDN_TEMP.size();
	auto a_pointer = PDN_TEMP.begin();
	if (PDN_TEMP.size() > 1){
		Transistor A = *a_pointer;
		for (auto it = begin(PDN_TEMP)+1; (it != end(PDN_TEMP)); ++it){
	  		Transistor B = *it;
	  		if (check_parallel(A,B) == true){
				expression.append(A.get_gate());
				expression.append("+");
				expression.append(B.get_gate());
				alias = "";
				alias.append("(");
				alias.append(A.get_gate());
				alias.append("+");
				alias.append(B.get_gate());
				alias.append(")");
				gate = alias;
				source = A.get_source();
				drain  = A.get_drain();
				Transistor group_transistor(alias, source, drain, gate, bulk, type, diff_width, fingers, gate_lenght, stack);
				PDN_TEMP.shrink_to_fit();
				PDN_TEMP.push_back(group_transistor); // insert the merged item
				PDN_TEMP = remove_two_items(PDN_TEMP, A, B); //remove the two items that were merged
				if (PDN_TEMP.size() == 1){
					return (PDN_TEMP.front()).get_alias();
					break;
				}
				else{
					expression = find_expression(PDN_TEMP);
					break;
				}
	  		}
	  	else if (check_series(A,B) == true){
			expression.append(A.get_gate());
			expression.append("*");
			expression.append(B.get_gate());
			alias = "";
			alias.append("(");
			alias.append(A.get_gate());
			alias.append("*");
			alias.append(B.get_gate());
			alias.append(")");
			gate = alias;

            //Find the connecting point and preserve the connection
            if ( A.get_source() == B.get_source() ){
                source = A.get_drain();
			    drain  = B.get_drain();
            }
            else if(A.get_drain() == B.get_drain()){
                source = A.get_source();
			    drain  = B.get_source();
            }
            else if(A.get_source() == B.get_drain()){
                source = A.get_drain();
			    drain  = B.get_source();
            }
            else{
                source = A.get_source();
			    drain  = B.get_drain();
            }
            
			Transistor group_transistor(alias, source, drain, gate, bulk, type, diff_width, fingers, gate_lenght, stack);
			PDN_TEMP.shrink_to_fit();
			PDN_TEMP.push_back(group_transistor);
			PDN_TEMP = remove_two_items(PDN_TEMP, A, B); //remove the two items that were merged
			if (PDN_TEMP.size() == 1){
				return (PDN_TEMP.front()).get_alias();
				break;
			}
			else{
				expression = find_expression(PDN_TEMP);
				break;
			}
	  	}
	  	else{
			cout << "not correlated items" << endl;
			a_pointer++;
	  	}
    	}
	}
	return expression;
}

int solve_boolean_expression(string expression){
	if (expression.size() > 1){
		replace_all(expression, "(0+0)", "0");
		replace_all(expression, "(0+1)", "1");
		replace_all(expression, "(1+0)", "1");
		replace_all(expression, "(1+1)", "1");

		replace_all(expression, "(0*0)", "0");
		replace_all(expression, "(0*1)", "0");
		replace_all(expression, "(1*0)", "0");
		replace_all(expression, "(1*1)", "1");

		replace_all(expression, "!1", "0");
		replace_all(expression, "!0", "1");
		return solve_boolean_expression(expression);
		}
	else{
		return atoi(expression.c_str());
	}
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
        for(char c1: literals){
			if(c1 == c){
			if (values[k]) {
          		cout << "F";
        	} else {
          		cout << "R";
        	}
			}
			else{
				cout << values[counter];
			}
			counter++;
			cout << " " ;
		}
        cout << " | ";
        if (newResult) {
          cout << "Fall";
        } else {
          cout << "Rise";
        }
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