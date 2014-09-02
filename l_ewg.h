#ifndef _L_EWG_H
#define _L_EWG_H

#include "l.h"
#include "l_edge.h"

struct EdgeWeightedGraph {
	int V;
	int E;
	vector<vector<Edge*> > adj;

	EdgeWeightedGraph(const char* filename) {
		FILE *fp = fopen(filename, "r");
		assert(fp);
		fscanf(fp, "%d", &V);
		fscanf(fp, "%d", &E);
		for (int v = 0; v < V; ++v) {
			adj.push_back(vector<Edge*>());
		}
		for (int e = 0; e < E; e++) {
			int v;
			int w;
			double weight;
			fscanf(fp, "%d %d %lf", &v, &w, &weight);

			addEdge(new Edge(v, w, weight));
		}
	}

	void addEdge(Edge* e) {
		int v = e->v;
		int w = e->w;
		adj[v].push_back(e);
		adj[w].push_back(e);
	}

	void delete_ewg() {
		//
	}

};

void p_ewg(EdgeWeightedGraph &g) {
	int i = 0;
	for (auto v : g.adj) {
		cout << i++ << ": ";
		for (auto e : v) {
			cout << *e << "; ";
		}
		cout << endl;
	}
	cout << "print EdgeWeightedGraph end" << endl;

}

class PQ_COMP {
public:
	bool operator()(Edge* &lhs, Edge* &rhs) {
		return *lhs > *rhs;
	}
};

class LazyPrimeMST {
public:
	vector<bool> marked;
	vector<Edge*> mst;
	priority_queue<Edge*, vector<Edge*>, PQ_COMP> pq;

	LazyPrimeMST(EdgeWeightedGraph &G) {
		marked = vector<bool>(G.V);
		for (auto i : marked) i = false;

		visit(G, 0);
		while (!pq.empty()) {
			auto e = pq.top();
			pq.pop();
			int v = e->v;
			int w = e->w;
			if (marked[v] && marked[w]) continue;
			mst.push_back(e);
			if (!marked[v]) visit(G, v);
			if (!marked[w]) visit(G, w);	
		}
	}

	void visit(EdgeWeightedGraph &G, int v) {
		marked[v] = true;
		for (auto e : G.adj[v]) 
			if (!marked[e->other(v)]) pq.push(e);
	}
};

class PrimeMST_COMP {
public:
	bool operator()(const pair<int, double> &lhs, const pair<int, double> &rhs) const{
		return lhs.second > rhs.second;
	}
};
class PrimeMST {
public:
	vector<double> m_distTo;
	vector<bool> m_marked;
	vector<Edge*> m_edgeTo;
	priority_queue<pair<int, double>, vector<pair<int, double> >, PrimeMST_COMP > pq;

	void relax(EdgeWeightedGraph* g, int v) {
		m_marked[v] = true;
		for (auto e : g->adj[v]) {
			int w = e->other(v);
			if (m_marked[w]) continue;
			if (m_distTo[w] > e->weight) {
				m_distTo[w] = e->weight;
				m_edgeTo[w] = e;
				pq.push(make_pair(w, m_distTo[w]));
			}
		}
	}

public:
	PrimeMST(EdgeWeightedGraph* g) {
		for (int v = 0; v < g->V; ++v) {
			m_distTo.push_back(numeric_limits<double>::max());
			m_marked.push_back(false);
			m_edgeTo.push_back(NULL);
		}

		int s = 0;
		m_distTo[s] = 0.0;
		pq.push(make_pair(s, m_distTo[s]));

		while (!pq.empty()) {
			int v = pq.top().first;
			pq.pop();
			relax(g, v);
		}
	}

	~PrimeMST(){}

	vector<Edge* > edges() {
		vector<Edge*> result;
		for (auto e : m_edgeTo) {
			if (e) result.push_back(e);
		}
		return result;
	}

	double weight() {
		double result(0.0);
		for (auto e : edges()) {
			result += e->weight;
		}
		return result;
	}



};

#endif // !_L_EWG_H
