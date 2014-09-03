#ifndef _L_FLOWNETWORK_H
#define _L_FLOWNETWORK_H

#include "l.h"
#include "l_flowedge.h"

class FlowNetwork {
	int m_V;
	int m_E;
	vector<vector<FlowEdge*> > m_adj;

public:
	FlowNetwork(const char* filename) {


		FILE* fp = fopen(filename, "r");
		assert(fp);
		fscanf(fp, "%d", &m_V);
		fscanf(fp, "%d", &m_E);

		for (int v = 0; v < m_V; ++v) m_adj.push_back(vector<FlowEdge*>());
		int v;
		int w;
		double c;
		for (int e = 0; e < m_E; ++e) {
			fscanf(fp, "%d %d %lf", &v, &w, &c);
			addEdge(new FlowEdge(v, w, c));
		}
	}

	void addEdge(FlowEdge* e) {
		m_adj[e->from()].push_back(e);
		m_adj[e->to()].push_back(e);
	}

	int V() { return m_V;  }
	int E() { return m_E; }
	vector<FlowEdge*> adj(int v) { return m_adj[v]; }

	friend ostream& operator<<(ostream &out, FlowNetwork &fn) {
		int i = 0;
		for (int v = 0; v < fn.V(); ++v) {
			cout << i++ << ": ";
			for (auto e : fn.adj(v)) cout << *e << "\t";
			cout << endl;
		}
		return out;
	}


};
#endif // !_L_FLOWNETWORK_H
