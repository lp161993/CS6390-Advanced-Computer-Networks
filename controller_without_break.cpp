 /*
 	CS6390 Advanced Computer Networks
	This code is a part of the final project of the networks course
	We are implementing Unicast and multicast routing
	This code controller.cpp is used to read and write to node file. This controls the flow of messages between nodes
 
 	Professor name: Jorge Cobb
	Project Member: Lakshmi Priyanka Selvaraj
 
 */
 
 #include<iostream>
 #include<fstream>
 #include<set>
 #include<string>
 #include<cstring>
 #include<sstream>
 #include<unistd.h>
 
 using namespace std;
 
 int main(){
 	
 	//Probably the first step is to read the topology file
 	//int w_flag, r_flag;
 	
 	//w_flag = (ios::trunc | ios::app | ios::out);
 	//r_flag = (ios::app | ios::in );
 	int data;
 	int adjacency_matrix[10][10];
 	ifstream file_ptr;
 	file_ptr.open("topology.txt");
 	
 	int data_collector[100]={};
 	int i = 0;
 	
 	while (!file_ptr.eof()) {                                     
        file_ptr >> data;
        data_collector[i++] = data;
    }
 	for(int h=0;h<10;h++){
		for(int a=0; a<10; a++){
			adjacency_matrix[h][a] =0;
		}
	}
 	for(int h=0; h <i ;h =h+2)
 	{
 		////cout<<data_collector[h]<<" ";
 	    //cout<<data_collector[h+1]<<endl;
 	    adjacency_matrix[data_collector[h]][data_collector[h+1]] = 1;
	}
	
	for(int h=0;h<10;h++){
		for(int a=0; a<10; a++){
			//cout<<adjacency_matrix[h][a]<<" ";
		}
		//cout<<endl;
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
	
	for(int h=0; h < num_nodes; h++)
	{		
		string file_name ="output_"+to_string(node_id[h])+".txt";
		ofstream outfile;
		outfile.open(file_name, ios::app); 			//We open input and output files for all the nodes given
		outfile.close();
		file_name	=	"input_"+to_string(node_id[h])+".txt";
		outfile.open(file_name, ios::app);
		outfile.close();
	}
	
	streampos end_ptr[num_nodes];
	
	for(int h=0; h <num_nodes; h++){
		
		string file_name ="output_"+to_string(node_id[h])+".txt"; 
		fstream file_ptr;
		file_ptr.open(file_name);
		file_ptr.seekg(0,ios::beg); //Check in the end, use ios::beg or ios::end appropriately
		end_ptr[h] = file_ptr.tellg();
		file_ptr.close();
		//cout<<"End pointer of "<<node_id[h]<<" is "<<end_ptr[h]<<endl;
	}
	
	//cout<<"Break: ";
	//int hulk;
	//cin>>hulk;
	
	for(int h=0; h <40; h++){
		
		//read all output files if the file pointer has changed location, then read the changed number of bytes and output to the new file
		for(int f=0;f<num_nodes; f++){
			string file_name ="output_"+to_string(node_id[f])+".txt"; 
			fstream file_ptr;
			file_ptr.open(file_name, ios::in  | ios::binary);
			streampos current;
			file_ptr.seekg(0,ios::end);
			current= file_ptr.tellg();
			//cout<<"Current pointer is "<<current<<endl;
			if(end_ptr[f] != current){
				//cout<<"The file with different end pointers is : "<<node_id[f]<<endl;
				int no_of_bytes = current - end_ptr[f];
				//cout<<"No_of_Bytes "<<no_of_bytes<<endl;
				char output_str[no_of_bytes];
				file_ptr.seekg(end_ptr[f]);
				file_ptr.read(output_str, no_of_bytes);				
				end_ptr[f] = current;
				file_ptr.close();
				//cout<<"Checking for neighbors of node "<<node_id[f]<<endl;
				for(int i =0; i<10; i++){
					//cout<<"Adjacency Matrix value for "<<node_id[f]<<" and "<<i <<" is "<<adjacency_matrix[node_id[f]][i]<<endl;
					if(adjacency_matrix[node_id[f]][i] == 1){
						//cout<<"Neighbor is :"<<i<<endl;
						string neigh_file = "input_"+to_string(i)+".txt";
						fstream neigh_file_ptr;
						neigh_file_ptr.open(neigh_file, ios::out | ios::app | ios::binary);
					 	neigh_file_ptr << output_str;
					 	neigh_file_ptr.close();
					}
				}
				
			}
		}
		sleep(1);	
	}
	
 	return 0;
 }
