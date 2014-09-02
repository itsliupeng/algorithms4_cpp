
#ifndef _L_GRAPH
#define _L_GRAPH

#include "l.h"

struct Graph{

	vector<vector<int> > adj;

	Graph(int V) {
		for (int i = 0; i < V; i++) {
			adj.push_back(vector<int>());
		}
	}


	Graph(const char* filename) {
		FILE* fp = fopen(filename, "r");
		assert(fp);

		int V, E;
		fscanf(fp, "%d", &V);
		fscanf(fp, "%d", &E);
		for (int i = 0; i < V; i++) {
			adj.push_back(vector<int>());
		}
		int v, w;
		for (int i = 0; i < E; ++i) {
			fscanf(fp, "%d %d", &v, &w);
			adj[v].push_back(w);
			adj[w].push_back(v);
		}
	}

	

};

void p_graph(Graph* g) {
	cout << "graph: " << endl;
	int i = 0;
	for (auto v : g->adj) {
		cout << i++ << ": ";
		for (auto i : v) cout << i << " ";
		cout << endl;
	}
}

void dfs(Graph* g, int v, vector<bool> &marked, vector<int> &result) {
	marked[v] = true;
	result.push_back(v);
	for (auto w : g->adj[v]) {
		if (!marked[w]) {
			dfs(g, w, marked, result);
		}
	}
}

vector<int> dfs(Graph* g) {
	vector<bool> marked(g->adj.size());
	vector<int> result;
	for (auto i : marked) i = false;
	int s = 0;
	dfs(g, s, marked, result);
	return result;
}

vector<int> bfs(Graph* g) {
	vector<int> result;
	vector<bool> marked(g->adj.size());
	int s = 0;
	queue<int> Q;
	marked[s] = true;
	Q.push(s);
	while (!Q.empty()) {
		int v = Q.front();
		Q.pop();
		result.push_back(v);
		for (int w : g->adj[v]) {
			if (!marked[w]) {
				marked[w] = true;
				Q.push(w);
			}

		}
	}
	return result;
}

#endif // _L_GRAPH
