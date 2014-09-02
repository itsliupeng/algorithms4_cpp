
#ifndef _L_DGRAPH_H
#define _L_DGRAPH_H

#include "l.h"

class  DirectdGraph{
public:
	vector<vector<int> > adj;
	int V;
	int E;
	vector<bool> marked;
	vector<int> edgeTo;
	vector<int> cycle;
	vector<bool> onStack;


	DirectdGraph(const char* filename) {
		FILE* fp = fopen(filename, "r");
		assert(fp);
		fscanf(fp, "%d", &V);
		fscanf(fp, "%d", &E);
		int v, w;
		for (int i = 0; i < V; ++i) {
			adj.push_back(vector<int>());
		}
		for (int i = 0; i < E; i++) {
			fscanf(fp, "%d %d", &v, &w);
			adj[v].push_back(w);
		}

		marked = vector<bool>(V);
		onStack = vector<bool>(V);
		edgeTo = vector<int>(V);
		for (int i = 0; i < V; i++) marked[i] = false;
		for (int i = 0; i < V; i++) onStack[i] = false;

	}
	
	vector<int> dfs(int s) {
		for (int i = 0; i < V; i++) marked[i] = false;
		for (int i = 0; i < V; i++) onStack[i] = false;
		vector<int> result;
		dfs(s, result);
		return result;
	}

	vector<int> bfs(int s) {
		for (int i = 0; i < V; i++) marked[i] = false;
		vector<int> result;
		queue<int> Q;
		marked[s] = true;
		Q.push(s);
		while (!Q.empty()) {
			auto v = Q.front();
			Q.pop();
			result.push_back(v);
			for (auto w : adj[v]) {
				if (!marked[w]) {
					marked[w] = true;
					Q.push(w);
				}
			}
		}
		return result;
	}

	bool hasCycle() {
		return !cycle.empty();
	}

	vector<int> topo_sort() {
		vector<int> result;
		for (auto i : marked) i = false;
		for (int s = 0; s < V; ++s) {
			if (!marked[s]) post_order_dfs(s, result);
		}
		reverse(result.begin(), result.end());
		return result;


	}
private:
	void dfs(int v, vector<int> &result) {
		onStack[v] = true;
		marked[v] = true;
		result.push_back(v);
		for (auto w : adj[v]) {
			if (hasCycle()) return;
			else if (!marked[w]) {
				edgeTo[w] = v;
				dfs(w, result);
			}
			else if (onStack[w]) {
				for (int x = v; x != w; x = edgeTo[x]) cycle.push_back(x);
				cycle.push_back(w);
				cycle.push_back(v);
				reverse(cycle.begin(), cycle.end());
			}
		}
		onStack[v] = false;
	}

	void post_order_dfs(int v, vector<int> &result) {
		marked[v] = true;	
		for (auto w : adj[v]) 
			if (!marked[w]) post_order_dfs(w, result);

		result.push_back(v);
	}
};



void p_dgraph(DirectdGraph* g) {
	cout << "graph: " << endl;
	int i = 0;
	for (auto v : g->adj) {
		cout << i++ << ": ";
		for (auto i : v) cout << i << " ";
		cout << endl;
	}
}
#endif // !_L_DGRPAH_H
