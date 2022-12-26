#include "map.h"
#include "transistor.h"
#include <vector>
using namespace std;



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

string find_expression(vector<Transistor> PDN, vector<Transistor> PUN){
	vector<Transistor> PDN_TEMP = PDN;
	vector<Transistor> PUN_TEMP = PUN;
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
					expression = find_expression(PDN_TEMP,PUN_TEMP);
					break;
				}
	  		}
	  	else if (check_series(A,B) == true){
			expression.append(A.get_gate());
			expression.append(".");
			expression.append(B.get_gate());
			alias = "";
			alias.append("(");
			alias.append(A.get_gate());
			alias.append(".");
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
				expression = find_expression(PDN_TEMP,PUN_TEMP);
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