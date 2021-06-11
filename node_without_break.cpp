
/*
	CS6390 Advanced Computer Networks
	This code is a part of the final project of the networks course
	We are implementing Unicast and multicast routing
	This code node.cpp aims to initialise every node based on the given input
	
	Professor name: Jorge Cobb
	Project Member: Lakshmi Priyanka Selvaraj

*/
#include<iostream>
#include<string.h>
#include<sstream>
#include<fstream>
#include<ctime>
#include<unistd.h>
#include<regex>
#include<map>
#include<queue>
#include<climits>

using namespace std;

int parent_array[10];
int V=10;

int min_distance(int dist[], bool stSet[]) 
{ 
    // Initialize min value 
    int min = INT_MAX, min_index; 
  
    for (int v = 0; v < V; v++) 
        if (stSet[v] == false && dist[v] <= min) 
            min = dist[v], min_index = v; 
  
    return min_index; 
} 
  
// Function to implement Dijkstra's  shortest path algorithm 
// for a graph represented using adjacency matrix representation 
void dijkstra_algo(int graph[10][10], int src_node) 
{ 
    int dist[10]; // The output array.  dist[i] will hold the shortest 
    // distance from src to i 
  
    bool stSet[10]; // sptSet[i] will be true if vertex i is included in shortest 
    // path tree or shortest distance from src to i is finalized 
  
    // Initialize all distances as INFINITE and stpSet[] as false 
    for (int i = 0; i < V; i++) 
        dist[i] = INT_MAX, stSet[i] = false; 
  
    // Distance of source vertex from itself is always 0 
    dist[src_node] = 0; 
  
    // Find shortest path for all vertices 
    for (int count = 0; count < V - 1; count++) { 
        // Pick the minimum distance vertex from the set of vertices not 
        // yet processed. u is always equal to src in the first iteration. 
        int u = min_distance(dist, stSet); 
  
        // Mark the picked vertex as processed 
        stSet[u] = true; 
  
        // Update dist value of the adjacent vertices of the picked vertex. 
        for (int v = 0; v < V; v++) 
  
            // Update dist[v] only if is not in sptSet, there is an edge from 
            // u to v, and total weight of path from src to  v through u is 
            // smaller than current value of dist[v] 
            if(graph[v][u] == 1){
	            if (!stSet[v] && graph[u][v] && dist[u] != INT_MAX 
	                && dist[u] + graph[u][v] < dist[v]) 
	                {
	                	dist[v] = dist[u] + graph[u][v]; 
						parent_array[u] = v;	
					}
			}
    } 
    
    /*for(int f=0;f<10;f++){
    	cout<<"I am node "<<f<<" and my parent is "<<parent_array[f]<<endl;
	}*/
  
    // print the constructed distance array 
    //printSolution(dist); 
} 

int main(int argc, char** argv){
	
	//We need to define a switch case to read the arguments that appear in command line
	int no_of_args = argc;
	int node_id;
	string sender_string = "";
	bool i_am_receiver = false;
	int sender_id;
	bool complete_join_msg = false;
	bool join_sent =false;
	bool i_am_sender = false;
	int root_child[10][2];
	int root_child_iter =0;
	
	//cout<<"No of arguments is :" <<argc<<endl;
	switch(no_of_args){
		case 1: 
			cout<<"Not enough arguments"<<endl;
			break;
		case 2:
			cout<<"Not enough arguments"<<endl;
			break;
		case 3:
			//cout<<argv[2]<<" is the node ID"<<endl;
			break;
		case 4:
			//cout<<"Not correct syntax"<<endl;
			break;
		case 5:
			//cout<<"This node is "<<argv[3] <<" and ID is "<<argv[2];
			if(strcmp(argv[3],"receiver") == 0){
				//cout<<"This is a receiver of the multicast group" <<endl;
				i_am_receiver = true;
				stringstream sender_id_str(argv[4]);
				sender_id_str >> sender_id;
				
			}
			if(strcmp(argv[3],"sender") == 0){
				//cout<<"This is a sender of the multicast group"<<endl;
				for(int h=4; h< argc; h++){
					sender_string +=argv[h];
					sender_string +=" ";
				}
			}
			break;
		default:
			if(strcmp(argv[3],"sender") == 0){
				//cout<<"This is a sender of the multicast group"<<endl;
				for(int h=4; h< argc; h++){
					sender_string +=argv[h];
					sender_string +=" ";
					i_am_sender=true; 
					//cout<<argv[h]<<" ";
				}
			}
			else{
				cout<<"There were too many arguments"<<endl;	
			}
			break;
	}
	//cout<<sender_string;
	
	//node_id = to_string(argv[2]);
	stringstream node_id_str(argv[2]);
	node_id_str >> node_id;
	string op_file_name = "output_"+to_string(node_id)+".txt";
	fstream outfile;
	string ip_file_name ="input_"+to_string(node_id)+".txt";
	fstream infile;
	//Noting my end pointer 
	streampos my_end_ptr;
	infile.open(ip_file_name, ios::in  | ios::binary);
	infile.seekg(0,ios::beg);
	my_end_ptr = infile.tellg();
	infile.close();
	time_t hello_timer_1, hello_timer_2;
	time_t lsa_timer_1, lsa_timer_2;
	time_t join_timer_1, join_timer_2;
	time(&hello_timer_1);
	time(&lsa_timer_1);
	time(&join_timer_1);
	bool received_join_msg = false;
	
	int timestamp=0;
	int neighbors[10];
	time_t neighbors_ts[10];	//This stores the timestamp of the hello messages
	int linkstate_ts[10];
	
	for(int h=0;h <10;h++){
		neighbors[h]=0;
		time(&neighbors_ts[h]);
		linkstate_ts[h]=-1;
		parent_array[h]=-1;
	}
	
	int topology_matrix[10][10];
	
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			topology_matrix[i][j] =0;
		}
	}
	/*cout<<"Break: ";
	int hulk;
	cin>>hulk;*/

	string hello_msg = "Hello "+to_string(node_id)+"\n";
	for(int h=0; h <40 ;h++){
		//if it is we send hello, else  we go to next part, do other processing and then come back and check if again and so on. 
		//take a timestamp, send a hello, next loop, wait for difference of previous timestamp and current to be 5 and send hello again
		//cout<<"Loop no: "<<h <<endl;
		time(&hello_timer_2);
		if(difftime(hello_timer_2,hello_timer_1) >= 5 or h == 0){
			outfile.open(op_file_name, ios::out | ios::app | ios::binary); 
			outfile << hello_msg;
			//cout<<"Round "<<h <<" Time difference: "<<hello_timer_2 <<" "<<hello_timer_1 <<" "<<difftime(hello_timer_2,hello_timer_1)<<endl;
			time(&hello_timer_1);		
			outfile.close();
		}

		//Hello works
		//Next would be for the controller to learn about each hello and forward to the neighbors based on the topology. 
		
		//Here i also send out link state advertisements. 
		time(&lsa_timer_2);
		if(difftime(lsa_timer_2,lsa_timer_1) >= 10 or h ==0){
			//send out a link state advertisement
			//cout<<"Time for another link state ad"<<endl;
			string lsa = "linkstate "+to_string(node_id)+" "+to_string(timestamp);
			timestamp++;
			for(int n=0; n <10; n++){
				if(neighbors[n] == 1){
					lsa = lsa +" "+to_string(n);
				}
			}
			//cout<<lsa<<endl;
			lsa+="\n";
			outfile.open(op_file_name, ios::out | ios::app | ios::binary); 
			outfile<< lsa;
			time(&lsa_timer_1);
			outfile.close();
			linkstate_ts[node_id]++;
		}

		
		//Process the new messages that i have received
		infile.open(ip_file_name, ios::in  | ios::binary);
		streampos current;
		infile.seekp(0,ios::end);
		current= infile.tellp();
		//cout<<"Current pointer is "<<current<<endl;
		if(my_end_ptr != current){
			//cout<<"The end pointers are different"	<<endl;
			int no_of_bytes = current - my_end_ptr;
			//cout<<"No_of_Bytes "<<no_of_bytes<<endl;
			char ip_str[no_of_bytes];
			infile.seekg(my_end_ptr);
			infile.read(ip_str, no_of_bytes);
			my_end_ptr = current;
			//cout<<ip_str;
			string ip_string(ip_str);
			
			regex hello_regex(R"([hH]ello (\d+))");
			sregex_iterator iter(ip_string.begin(),ip_string.end(),hello_regex);
			sregex_iterator end;
			
			while(iter != end){
				//for(int i=0; i<iter->size(); i++){
					//cout<<" match is "<<(*iter)[1]<<endl;
					//int id = (*iter)[1];
					smatch match=*(iter);
					string mat = match.str();
					////cout<<mat<<endl;
					char str_id = mat.back();
					int id = (int)(str_id);
					//cout<<"We found a matching hello with id "<<id-48 <<endl;
					id =id-48;
					neighbors[id] = 1;
					//here update the timestamp
					time(&neighbors_ts[id]);
				//}
				++iter;
			}
			
			regex ls_regex(R"([lL]inkstate (\d) (\d).*)");
	
			sregex_iterator iter_ls(ip_string.begin(),ip_string.end(),ls_regex);
			sregex_iterator end_ls;
			
			while(iter_ls != end_ls){
				smatch match = *(iter_ls);
				string mat = match.str();
				//cout<<"We found a matching LSA"<<endl;
				//cout<<mat<<endl;
				
				stringstream ss;
				ss << mat;
				string temp;
				int num_found;
				int msg_iter=0;
				int lsa_node_id;
				int lsa_neigh;
				
				while(!ss.eof()){
					ss>>temp;
					if(stringstream(temp)>>num_found){
						if(msg_iter ==0){
							//I found a node_if in the lsa
							lsa_node_id = num_found;
						}
						else if(msg_iter == 1)
						{
							if(num_found > linkstate_ts[lsa_node_id]){
								//As soon as i find an LSA from another, I forward to neighbors if timestamp greater than previous
								outfile.open(op_file_name, ios::out | ios::app | ios::binary); 
								mat=mat+"\n";
								outfile<< mat;
								outfile.close();
								linkstate_ts[lsa_node_id]=num_found;
							}
						}
						else if(msg_iter >= 2){
							lsa_neigh = num_found;
							topology_matrix[lsa_node_id][lsa_neigh] = 1;
							//cout<<"Topology matrix updated: "<<topology_matrix[lsa_node_id][lsa_neigh]<<endl;
						}
						else{
							//do nothing;
						}
						msg_iter++;
					}
					temp="";
				}
				++iter_ls;
			}
			
			regex join_regex(R"([jJ]oin (\d) (\d).*)");
			sregex_iterator iter_join(ip_string.begin(),ip_string.end(),join_regex);
			sregex_iterator end_join;			
			string refined_join_msg ="join ";
			string my_join_msg="join_tree ";
			int my_join_msg_iter=0;
			bool should_i_frwd_msg = false;
			
			
			while(iter_join != end_join){
				smatch match = *(iter_join);
				string mat = match.str();
				//cout<<mat<<endl;
				
				stringstream ss;
				ss << mat;
				string temp;
				int num_found;
				int msg_iter=0;
				int join_node_id;
				int join_sender_id;
				int join_next_hop;
				
				
				while(!ss.eof()){
					ss>>temp;
					if(stringstream(temp)>>num_found){
						if(msg_iter ==0){
							//I found a node_if in the lsa
							join_node_id = num_found; //This node wants to join sender
							refined_join_msg = refined_join_msg + temp +" ";
						}
						else if(msg_iter == 1)
						{
							//If my node_id is root, then receiver has joined the root id
							join_sender_id = num_found; //This is the root node
							refined_join_msg = refined_join_msg + temp +" ";
							if(join_sender_id == node_id){
								////cout<<"The join has reached the sender"<<endl;	
								//sender has to start sending data to the receiver
								//We probably take this outside the loop
								received_join_msg=true;
							}
							//I will send the sender string/msg to my child on the tree
							
							
						}
						else if(msg_iter == 2){
							//if my node_id is first among next hops, then i forward this join to my neighbors
							join_next_hop = num_found;
							if(join_next_hop == node_id) {
								should_i_frwd_msg = true;
								my_join_msg = my_join_msg+ temp + " "+to_string(join_sender_id);
								my_join_msg_iter++;
								////cout<<"I am next_hop with ms_iter 2 "<<join_next_hop<<endl;
								//modify join msg and send to output file
							}	
						}
						else{
							//just collect the first nodeid after my id and append to the join_src
							if(my_join_msg_iter == 1){
								my_join_msg = my_join_msg+" "+temp;
							}
							refined_join_msg = refined_join_msg + temp +" ";
						}
						msg_iter++;
					}
					temp="";
				}
				++iter_join;
			}
			
			if(should_i_frwd_msg){
				refined_join_msg = refined_join_msg + "\n";
				outfile.open(op_file_name, ios::out | ios::app | ios::binary); 
				outfile<< refined_join_msg;
				outfile.close();	
				
				my_join_msg =my_join_msg+"\n";
				outfile.open(op_file_name, ios::out | ios::app | ios::binary); 
				outfile<< my_join_msg;
				outfile.close();
			}
			
			
			regex join_tree_regex(R"([jJ]oin_tree (\d) (\d).*)");
			sregex_iterator iter_join_tree(ip_string.begin(),ip_string.end(),join_tree_regex);
			sregex_iterator end_join_tree;
			
			while(iter_join_tree != end_join_tree){
				smatch match = *(iter_join_tree);
				string mat = match.str();
				//cout<<mat<<endl;
				
				stringstream ss;
				ss << mat;
				string temp;
				int num_found;
				int msg_iter=0;
				int root_id;
				int child_id;
				int my_id;
				bool last_hop=false;
				bool update_child = false;		
				
				while(!ss.eof()){
					ss>>temp;
					if(stringstream(temp)>>num_found){
						if(msg_iter == 0){
							child_id = num_found;
						}
						else if(msg_iter == 1)
						{
							root_id = num_found;
						}
						else if(msg_iter == 2)
						{
							my_id = num_found;
							if(my_id == node_id){
								update_child = true;	
							}
						}
						else{
							
						}
						msg_iter++;
					}
					temp="";
				}
				++iter_join_tree;
				
				if(msg_iter == 2 and root_id == node_id){
					//this is of the form join_tree 4 0
					//cout<<"I am "<<root_id << " and my child is "<<child_id <<" and my root is "<<root_id<<endl;
					root_child[root_child_iter][0]=root_id;
					root_child[root_child_iter][1]=child_id;
					root_child_iter++;
				}
				if(msg_iter ==3 and update_child)
				{
					//this is of the form join_tree 5 0 4
					//cout<<"I am "<< my_id <<" and my child is "<<child_id <<" and my root is "<<root_id<<endl;
					root_child[root_child_iter][0]=root_id;
					root_child[root_child_iter][1]=child_id;
					root_child_iter++;
				}
				//cout<<endl;
			}
			
			regex data_regex(R"([dD]ata (\d) (\d).*)");
			sregex_iterator data_msg_iter(ip_string.begin(),ip_string.end(),data_regex);
			sregex_iterator data_msg_end;
			
			while(data_msg_iter != data_msg_end){
				smatch match = *(data_msg_iter);
				string mat = match.str();
				////cout<<mat<<endl;
				
				stringstream ss;
				ss << mat;
				string temp;
				int num_found;
				int msg_iter=0;
				int root_id;
				bool frwd_msg = false;
				
				while(!ss.eof()){
					ss>>temp;
					if(stringstream(temp)>>num_found){
						if(msg_iter ==0){
							if(num_found == node_id){
								frwd_msg = true;
							}
						}
						else if(msg_iter == 1)
						{
							root_id = num_found;
							//I will send the sender string/msg to my child on the tree
						}
						else{
							
						}
						msg_iter++;
					}
					temp="";
				}
				++data_msg_iter;
				if(frwd_msg){
					//find my child for this specific root_id
					//cout<<"-----------------";
					//cout<<"I am going to forward the data msg"<<endl;
					//cout<<"The number of root_child ids i have is "<< root_child_iter<<endl;
					for(int u=0; u <root_child_iter; u++){		
						//cout<<"I did come into the for loop"<<endl;
						int stored_root   = root_child[u][0];
						int stored_child  = root_child[u][1];
						//cout<<"Stored root is "<<stored_root<<" and it' child is "<<stored_child<<endl;
						if(stored_root == root_id){
							//cout<<"For the root "<<stored_root<<" the child is "<<stored_child <<endl;
							string replace_id = to_string(stored_child);
							string refined_data_msg = ip_string;
							refined_data_msg.replace(5,1,replace_id);
							//cout<<"Refined data msg is "<<refined_data_msg<<endl;
							outfile.open(op_file_name, ios::out | ios::app | ios::binary); 
							outfile << refined_data_msg;
							outfile.close();
							frwd_msg=false;
						}
						/*else{
							root_child.pop();
							root_child.push(incoming);
						}*/
					}
				}
			}
		}
		infile.close();
		if(received_join_msg && i_am_sender){
			time(&join_timer_2);
			if(difftime(join_timer_2,join_timer_1) >= 10){
				for(int k=0; k < root_child_iter; k++){
					int root_id   = root_child[k][0];
					int child_id  = root_child[k][1];
		            if(root_id == node_id){
			            //cout<<"My child_id is "<<child_id <<" and my root is "<<root_id<<endl;
						string sender_msg = "data "+to_string(child_id)+" "+to_string(node_id)+" "+sender_string+"\n";
						//Time difference is greater than 10, this message can be sent again
						outfile.open(op_file_name, ios::out | ios::app | ios::binary); 
						outfile << sender_msg;
						outfile.close();
						time(&join_timer_1);
						}
					}
				}
		}
		
		//
		
		//if i am a receiver for a multicast group, i need to formulate and send a join message
		if((i_am_receiver == true) and (join_sent == false)){
			//so first check if the receiver has a route to the parent.
			for(int w=0; w<10; w++){
				for(int e =0;e<10;e++){
					//cout<<topology_matrix[w][e]<<" ";
				}
				//cout<<endl;
			}
			dijkstra_algo(topology_matrix,node_id);
			int next_hop = parent_array[node_id];
			string join_msg = "join "+to_string(node_id)+" "+to_string(sender_id);
			string join_tree_msg = "join_tree "+to_string(node_id)+" "+to_string(sender_id)+" "+to_string(next_hop)+"\n";
			for(int d=0;d<10;d++){
				//cout<<parent_array[d]<<" ";
			}
			while(next_hop != -1){
				//until we reach the sender_id, we run this loop
				if(next_hop != sender_id){
					join_msg = join_msg+" "+to_string(next_hop);
					next_hop = parent_array[next_hop];	
				}
				else if(next_hop == sender_id){
					complete_join_msg = true; //that is we found the complete shortest path to the root
					next_hop = -1;						//if break doesn't work, set next_hop to -1
				}
				else{
					//do nothing
					}
				}
			
			if(complete_join_msg){
				//I have a full join message
				//I can send it to all my neighbors, rather I append to my output file
				join_msg = join_msg+"\n";
				outfile.open(op_file_name, ios::out | ios::app | ios::binary); 
				outfile << join_msg;
				outfile<< join_tree_msg;
				outfile.close();
				join_sent =true;
			}
		}
		
		//Once i made x as a neighbor, i also need to store and process timestamps
		time_t present_time;
		time(&present_time);
		//To check for expired hello messages	
		for(int n=0; n <10; n++){
			if(difftime(present_time,neighbors_ts[n]) >= 10){
				//Expiration of a neighbor if hello was sent more than 10 seconds ago
				neighbors[n] = 0;
				topology_matrix[node_id][n] = 0;
				topology_matrix[n][node_id] = 0;	
				}
			}
			sleep(1);
		}

	return 0;
	
}
