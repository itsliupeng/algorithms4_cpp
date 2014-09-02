#ifndef _L_EWD_H
#define _L_EWD_H

#include "l.h"
#include "l_dedge.h"

class EdgeWeightedDigraph {
private:
	int m_V;
	int m_E;
	vector<vector<DirectedEdge*> > m_adj;

	friend ostream& operator<< (ostream& out, EdgeWeightedDigraph &G);

public:
	EdgeWeightedDigraph(const char* filename) {
		FILE* fp = fopen(filename, "r");
		fscanf(fp, "%d", &m_V);
		fscanf(fp, "%d", &m_E);
		for (int v = 0; v < m_V; ++v) m_adj.push_back(vector<DirectedEdge*>());
		int v;
		int w;
		double weight;
		for (int e = 0; e < m_E; ++e) {
			fscanf(fp, "%d %d %lf", &v, &w, &weight);
			m_adj[v].push_back(new DirectedEdge(v, w, weight));
		}
	}

	int V() { return m_V; }
	int E() {return m_E;}
	vector<DirectedEdge*>  adj(int v) { return m_adj[v]; }
	

};

ostream& operator<< (ostream& out, EdgeWeightedDigraph &G) {
	for (auto V : G.m_adj) {
		for (auto e : V) cout << *e << " ";
		cout << endl;
	}
	return out;
}

class SP_COMP {
public:
	bool operator()(pair<int, double> &p1, pair<int, double> &p2) {
		return p1.second > p2.second;
	}
};

class DijkstraSP {
private:
	vector<DirectedEdge*> m_edgeTo;
	vector<double> m_distTo;
	priority_queue<pair<int, double>, vector<pair<int, double> >, SP_COMP> m_pq;
	int s;

	void relax(EdgeWeightedDigraph* g, int v) {
		for (auto e : g->adj(v)) {
			int w = e->to();
			if (m_distTo[w] > m_distTo[v] + e->weight()) {
				m_distTo[w] = m_distTo[v] + e->weight();
				m_edgeTo[w] = e;
				m_pq.push(make_pair(w, m_distTo[w]));
			}
		}
	}



public:
	DijkstraSP(EdgeWeightedDigraph* g, int s): s(s) {
		for (int v = 0; v < g->V(); ++v) {
			m_distTo.push_back(numeric_limits<double>::max());
			m_edgeTo.push_back(NULL);
		}
		m_distTo[s] = 0.0;
		m_pq.push(make_pair(s, m_distTo[s]));

		while (!m_pq.empty()) {
			int v = m_pq.top().first;
			m_pq.pop();
			relax(g, v);
		}
	}

	~DijkstraSP(){

	}

	double distTo(int v) {
		return m_distTo[v];
	}

	vector<int> pathTo(int v) {
		vector<int> result;
		result.push_back(v);
		for (auto x = m_edgeTo[v]; x->from() != s; x = m_edgeTo[x->from()]) {
			result.push_back(x->from());
		}
		result.push_back(s);
		reverse(result.begin(), result.end());
		return result;
	}

};

class DijkstraAllPairsSP {
	vector<DijkstraSP*> all;
public:
	DijkstraAllPairsSP(EdgeWeightedDigraph* g) {
		for (int v = 0; v < g->V(); ++v) {
			all.push_back(new DijkstraSP(g, v));
		}
	}

	vector<int> path(int s, int t) {
		return all[s]->pathTo(t);
	}

	double dist(int s, int t) {
		return all[s]->distTo(t);
	}
};


class Topological {
	vector<int> postOrder;
	vector<bool> marked;

	void dfs(EdgeWeightedDigraph* g, int v) {
		marked[v] = true;
		for (auto e : g->adj(v)) {
			int w = e->to();
			if (!marked[w]) dfs(g, w);
		}
		postOrder.push_back(v);
	}

public:
	Topological(EdgeWeightedDigraph* g, int s) {
		// assert Acyclic
		for (int v = 0; v < g->V(); ++v) {
			marked.push_back(false);
			postOrder.reserve(g->V());
		}
		for (int v = s; v < g->V(); ++v) {
			if (!marked[v]) dfs(g, v);	
		}
		for (int v = 0; v < s; ++v) {
			if (!marked[v]) dfs(g, v);
		}
	}

	vector<int> order() {
		reverse(postOrder.begin(), postOrder.end());
		return postOrder;
	}
};

class AcyclicSP{
	vector<DirectedEdge*> m_edgeTo;
	vector<double> m_distTo;
	int s;

	void relax(EdgeWeightedDigraph *g, int v) {
		for (auto e : g->adj(v)) {
			int w = e->to();
			if (m_distTo[w] > m_distTo[v] + e->weight()) {
				m_distTo[w] = m_distTo[v] + e->weight();
				m_edgeTo[w] = e;
			}
		}
	}

public:
	AcyclicSP(EdgeWeightedDigraph* g, int s):s(s) {
		for (int v = 0; v < g->V(); ++v) {
			m_edgeTo.push_back(NULL);
			m_distTo.push_back(numeric_limits<double>::max());
		}

		Topological* top = new Topological(g, s);
		m_distTo[s] = 0.0;
		for (auto v : top->order()) {
			relax(g, v);
		}
	}

	vector<int> edgeTo(int v) {
		vector<int> result;
		result.push_back(v);
		for (auto x = m_edgeTo[v]; x->from() != s; x = m_edgeTo[x->from()]) {
			result.push_back(x->from());
		}
		result.push_back(s);
		reverse(result.begin(), result.end());
		return result;
	}

	double distTo(int v) {
		return m_distTo[v];
	}
};

#endif // !_L_EWD_H
