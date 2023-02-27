//consumer code
//non-optimized version run as ./consumer id file_name
//optimized version run as ./consumer id file_name -optimize
#include <iostream>
#include "graph.h"
#include <unistd.h>
#include <vector>
#include <queue>
#include <fstream>
#include <stack>
#include <cstring>
#include <sstream>
using namespace std;

//infinity value
#define inf (1e9)+7

//this function runs multi-source dijkstra algorithm on graph G
//source_nodes contains a list of intervals of the node_ids which are mapped to this consumer
//dist and par are initialized by this function, dist[i] = distance of node i from closest source node
//par[i] = parent of i i.e. the immediate predecessor of i which is traversed from the source in order to reach i through the shortest path
void multisource_dijkstra(graph *G,vector<pair<int,int>> &source_nodes,vector<int> &dist, vector<int> &par){
	priority_queue <pair<int,int>> pq;
	
	int n = par.size();
	for(auto &it:source_nodes){
		int i1= it.first,i2 = it.second;
		for(int i=i1;i<=i2;i++){
			pq.push({0,i});
			dist[i]=0;
			par[i]=-1;
		}
	}

	vector <bool> vis(n,0);
	while(!pq.empty()){
		auto it = pq.top();
		pq.pop();
		int i = it.second, d = -it.first;
		if(vis[i]==1)continue;
		vis[i]=1;
		int curr = (G->heads[i]).next;
		while(curr!=-1){
			int j = (G->nodes[curr]).id;
			if(j>=n){
				curr = (G->nodes[curr]).next;
				continue;
			}
			if(d+1 < dist[j]){
				dist[j] = d+1;
				par[j] = i;
				pq.push({-dist[j],j});
			}
			curr = (G->nodes[curr]).next;
		}
	}
	return;
}

//using parent vector the path of a node to the closest source node is computed and then written/appended to the file
void write_to_file(string file_name,vector <int> &par,int iter){
    ofstream ofile;
	if(iter>1)ofile.open(file_name,ios::app);
    else ofile.open(file_name,ios::out);

	if(!ofile){
		perror("open():");
		return;
	}
	fflush(stdout);
	int n = par.size();
    stack <int> st;
	ofile << "Iteration: " << iter << "\n";
    for(int i=0;i<n;i++){
	    if(par[i]==-1)continue;
        int curr = i;
        while(curr!=-1){
            st.push(curr);
            curr = par[curr];
        }
        while(!st.empty()){
            ofile << st.top();
            st.pop();
            if(st.empty())ofile << endl;
            else ofile << ' ';
        }
    }
	ofile << "\n";
    ofile.close();
}


//this function runs multi-source bfs algorithm on graph G
//source_nodes contains a list of intervals of the node_ids which are mapped to this consumer
//dist and par are updated by this function, dist[i] = distance of node i from closest source node
//par[i] = parent of i i.e. the immediate predecessor of i which is traversed from the source in order to reach i through the shortest path
//n = number of nodes in G
void multisource_bfs(graph *G,vector<pair<int,int>> &source_nodes,vector<int> &dist, vector<int> &par,int n){
	queue <int> q;
	
	if(n>(int)dist.size()){
		for(int i=dist.size();i<n;i++){
			dist.push_back(inf);
			par.push_back(-1);
		}
	}
	for(auto &it:source_nodes){
		int i1= it.first,i2 = it.second;
		for(int i=i1;i<=i2;i++){
			q.push(i);
			dist[i]=0;
			par[i]=-1;
		}
	}

	while(!q.empty()){
		int i = q.front();
		q.pop();
		int d = dist[i];
		int curr = (G->heads[i]).next;
		while(curr!=-1){
			int j = (G->nodes[curr]).id;
			if(j>=n){
				curr = (G->nodes[curr]).next;
				continue;
			}
			if(d+1 < dist[j]){
				dist[j] = d+1;
				par[j] = i;
				q.push(j);
			}
			curr = (G->nodes[curr]).next;
		}
	}
	return;
}

int main(int argc,char *argv[]){
	if(argc<3 || argc>4){
		cout << "Invalid arguments" << endl;
		return 0;
	}


	int idx = atoi(argv[1]);
	string file_name(argv[2]);
	bool optimize = 0;

	if(argc==4){
		if(strcmp(argv[3],"-optimize")==0)optimize = 1;
		else{
			cout << "Invalid arguments" << endl;
			return 0;
		}
	}
	
	graph *G = NULL;
	bool is_first = 1;
	int curr_cnt,prev_cnt=-1;
	vector <pair<int,int>> source_nodes;
	source_nodes.clear();
	vector <int> dist,par;
	int iter=1;
	while(1){
		//attach shared memory
		G = activate_graph();
		curr_cnt = *(G->n);
		//no optimization for first iteration
		if(is_first){
			is_first = 0;
			dist.clear();
			dist.assign(curr_cnt,inf);
			par.clear();
			par.assign(curr_cnt,-1);
			int start_idx = (idx-1)*(curr_cnt/10);
			int end_idx = start_idx + (curr_cnt/10) - 1;
			if(idx==10){
				end_idx = curr_cnt-1;
			}
			source_nodes.push_back({start_idx,end_idx});
			multisource_dijkstra(G,source_nodes,dist,par);
			write_to_file(file_name,par,iter);
			prev_cnt = curr_cnt;
		}
		else{
			if(prev_cnt == curr_cnt){
				//if graph has not updated then no need to recompute any path
				write_to_file(file_name,par,iter);
			}
			else{
				//update the source_nodes vector
				int x = curr_cnt-prev_cnt;
				int start_idx = (idx-1)*(x/10);
				int end_idx = start_idx + (x/10) - 1;
				if(idx==10){
					end_idx = x-1;
				}
				source_nodes.push_back({start_idx+prev_cnt,end_idx+prev_cnt}); 
			}
			if(prev_cnt!=curr_cnt && (!optimize)){
				//for non optimized version we simply run multi-source dijkstra again
				par.assign(curr_cnt,-1);
				dist.assign(curr_cnt,inf);
				multisource_dijkstra(G,source_nodes,dist,par);
				write_to_file(file_name,par,iter);
			}
			else if(prev_cnt!=curr_cnt && (optimize==1)){
				//for optimized version we re-use the previous distance and parent vectors
				// because sources will only be added to mapped set of a consumer so distance will always either decrease or stay same
				//this helps pruning out paths that do not change
				//also we run mutlisource bfs in place of multisource dijkstra because all edge weights are 1
				//this improves running time
				multisource_bfs(G,source_nodes,dist,par,curr_cnt);
				write_to_file(file_name,par,iter);
			}
		}
		iter++;
		deactivate_graph(G); //detach from shared memory
        sleep(30);
	}
}