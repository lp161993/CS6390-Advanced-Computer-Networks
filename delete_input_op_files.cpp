 /*This file is used to delete all the input and output files generated during every execution*/


#include<fstream>
 #include<stdio.h>
 #include<set>
 #include<sstream>
 #include <bits/stdc++.h>
 
 using namespace std;
 
 int main(){
 	fstream file_ptr;
 	file_ptr.open("topology.txt");
 	int data_collector[100]={};
 	int i = 0;
 	int data;
 	
 	while (!file_ptr.eof()) {                                     
        file_ptr >> data;
        data_collector[i++] = data;
    }
    
    set<int> sa(data_collector,data_collector+i);
	int num_nodes = sa.size();
	int node_id[num_nodes];
	
	for(int h=0; h < num_nodes; h++)
	{
		string node_val = to_string(*next(sa.begin(),h)); 
		stringstream node_id_int(node_val);
		node_id_int >> node_id[h];
	}
	
	for(int h=0;h<num_nodes ;h++){
		string abc = "output_"+to_string(node_id[h])+".txt";
		char file_name[20];
		strcpy(file_name, abc.c_str());
		int r= remove(file_name);
		abc	=	"input_"+to_string(node_id[h])+".txt";
		strcpy(file_name, abc.c_str());
		r = remove(file_name);
	}
    return 0;
    
 }
