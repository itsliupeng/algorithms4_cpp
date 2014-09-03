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
	EdgeWeightedDigraph(int V) {
		m_V = V;
		for (int v = 0; v < V; ++v) m_adj.push_back(vector<DirectedEdge*>());
	}

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

	//copy constructor
	EdgeWeightedDigraph(EdgeWeightedDigraph *g) {
		m_V = g->V();
		m_E = g->E();
		for (int v = 0; v < m_V; ++v) {
			for (auto e : g->adj(v)) m_adj[v].push_back(new DirectedEdge(e->from(), e->to(), e->weight()));
 		}
	}

	~EdgeWeightedDigraph() {
		/*for (int i = 0; i < m_adj.size(); ++i) {
			for (int j = 0; j < m_adj[i].size(); ++j) delete m_adj[i][j];
		}*/
	}

	int V() { return m_V; }
	int E() {return m_E;}
	vector<DirectedEdge*>  adj(int v) { return m_adj[v]; }

	void addEdge(DirectedEdge* e) {
		m_adj[e->from()].push_back(e);
	}

	EdgeWeightedDigraph* reverse() {
		for (auto v : m_adj) {
			for (auto e : v)  {
				e->weight(-e->weight());
			}
		}
		return  this;
	}
	

};

ostream& operator<< (ostream& out, EdgeWeightedDigraph &G) {
	for (auto V : G.m_adj) {
		for (auto e : V) cout << *e << " ";
		cout << endl;
	}
	return out;
}



class EdgeWeightedDirectedCycle {
	vector<int> m_cycle;
	vector<bool> marked;
	vector<bool> onStack;
	vector<DirectedEdge*> edgeTo;

	void dfs(EdgeWeightedDigraph* g, int v) {
		if (hasCycle()) return;
		marked[v] = true;
		onStack[v] = true;

		for (auto e : g->adj(v)) {
			int w = e->to();
			if (onStack[w]) {
				m_cycle.push_back(w);
				for (auto x = e; x->from() != w; x = edgeTo[x->from()]) m_cycle.push_back(x->from());
				m_cycle.push_back(w);
				break;
			}
			else if (!marked[w]){
				edgeTo[w] = e;
				dfs(g, w);
			}
		}

		onStack[v] = false;
	}

public:
	EdgeWeightedDirectedCycle(EdgeWeightedDigraph* g) {
		for (int v = 0; v < g->V(); ++v) {
			marked.push_back(false);
			onStack.push_back(false);
			edgeTo.push_back(NULL);
		}

		for (int v = 0; v < g->V(); ++v) {
			if (!marked[v]) dfs(g, v);
		}
	}

	~EdgeWeightedDirectedCycle() {

	}

	bool hasCycle() {
		return !m_cycle.empty();
	}
	vector<int> cycle(){
		reverse(m_cycle.begin(), m_cycle.end());
		return m_cycle;
	}
};


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

	~DijkstraAllPairsSP() {
		for (auto i : all) delete i;
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
		auto find_cycle = EdgeWeightedDirectedCycle(g);
		try {
			if (find_cycle.hasCycle()) {
				throw runtime_error("not acyclic");
			}
		}
		 catch(runtime_error err) {
			 cout << err.what() << endl;
		}


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

	~Topological() {

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

		delete top;
	}

	~AcyclicSP() {

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

// relax every vertice V times, O(EV)
class BellmanFord {

	vector<double> m_distTo;
	vector<DirectedEdge*> m_edgeTo;
	int m_s;

	void relax(EdgeWeightedDigraph* g, int v) {
		for (auto e : g->adj(v)) {
			int w = e->to();
			if (m_distTo[w] > m_distTo[v] + e->weight()) {
				m_distTo[w] = m_distTo[v] + e->weight();
				m_edgeTo[w] = e;
			}
		}
	}

public:
	BellmanFord(EdgeWeightedDigraph* g, int s): m_s(s) {
		for (int v = 0; v < g->V(); ++v) {
			m_distTo.push_back(numeric_limits<double>::max());
			m_edgeTo.push_back(NULL);
		}

		m_distTo[s] = 0;
		for (int pass = 0; pass < g->V(); ++pass) {
			for (int v = 0; v < g->V(); ++v) relax(g, v);
		}
	}

	~BellmanFord() {

	}

	vector<int> edgeTo(int v) {
		vector<int> result;
		result.push_back(v);
		for (auto x = m_edgeTo[v]; x->from() != m_s; x = m_edgeTo[x->from()]) {
			result.push_back(x->from());
		}
		result.push_back(m_s);
		reverse(result.begin(), result.end());
		return result;
	}

	double  distTo(int v) {
		return m_distTo[v];
	}
};

// based on queue
class BellmanFordSP {
	vector<double> m_distTo;
	vector<bool> m_onQ;
	vector<int> m_cycle;
	queue<int> m_queue;
	vector<DirectedEdge*> m_edgeTo;
	int m_cost;
	int m_s;

	void relax(EdgeWeightedDigraph* g, int v) {
		for (auto e : g->adj(v)) {
			int w = e->to();
			if (m_distTo[w] > m_distTo[v] + e->weight()) {
				m_distTo[w] = m_distTo[v] + e->weight();
				m_edgeTo[w] = e;
				if (!m_onQ[w]) {
					m_queue.push(w);
					m_onQ[w] = true;
				}
			}
	
		}
		// have relaxed V times, need to detect whether cycle exists.
		if (++m_cost % g->V() == 0) {
			findNegativeCycle();
		}
	}

	void findNegativeCycle() {

		// use m_edgeTo to build a spanning tree
		auto spt = new EdgeWeightedDigraph(m_edgeTo.size());
		for (auto e : m_edgeTo) {
			if (e) spt->addEdge(e);
		}

		// detect cycle
		auto finder = new EdgeWeightedDirectedCycle(spt);
		m_cycle = finder->cycle();

		delete spt;
		delete finder;
	}

public:
	BellmanFordSP(EdgeWeightedDigraph* g, int s): m_s(s) {
		for (int v = 0; v < g->V(); ++v) {
			m_distTo.push_back(numeric_limits<double>::max());
			m_onQ.push_back(false);
			m_edgeTo.push_back(NULL);
			m_cost = 0;
		}

		m_distTo[m_s] = 0.0;
		m_queue.push(m_s);
		m_onQ[m_s] = true;

		while (!m_queue.empty() && !hasNegativeCycle()) {
			int v = m_queue.front();
			m_queue.pop();
			m_onQ[v] = false;
			relax(g, v);
		}
	}

	~BellmanFordSP() {

	}

	bool hasNegativeCycle() {
		return !m_cycle.empty();
	}

	vector<int> negativeCycle() {
		return m_cycle;
	}
};



// CPM  Critical Path Method <=> AcyclicDigraph Longest Path

class AcyclicLP {
	AcyclicSP* lp;

public:
	AcyclicLP(EdgeWeightedDigraph *g, int s) {
		EdgeWeightedDigraph *rg = (new EdgeWeightedDigraph(*g))->reverse();	
		cout << *rg << endl;
		lp = new AcyclicSP(rg, s);

		delete rg;
	}

	~AcyclicLP() {
		delete lp;
	}

	vector<int> edgeTo(int v) { return lp->edgeTo(v); }
	double distTo(int v) { return -lp->distTo(v); }
};

class CPM {
	AcyclicLP* lp;
	int s;
	int t;
public:
	CPM(const char* filename){
		FILE *fp = fopen(filename, "r");
		assert(fp);
		int N;

		fscanf(fp, "%d", &N);
		s = 2 * N;
		t = 2 * N + 1;

		EdgeWeightedDigraph *g = new EdgeWeightedDigraph(2 * N + 2);
		char buf[20];
		fgets(buf, 20, fp);
		for (int v = 0; v < N; v++) {
			fgets(buf, 20, fp);
			auto pch = strtok(buf, " ");
			double duration = atof(pch);
			g->addEdge(new DirectedEdge(v, v + N, duration));
			g->addEdge(new DirectedEdge(s, v, 0.0));
			g->addEdge(new DirectedEdge(v + N, t, 0.0));
			while (true) {
				pch = strtok(NULL, " ");
				if (!pch) break;
				g->addEdge(new DirectedEdge(v+N, atoi(pch), 0.0));
			}
		}

		lp = new AcyclicLP(g, s);

		delete g;
	}

	~CPM() {
		delete lp;
	}

	vector<double> startTimes() {
		vector<double> result;
		for (int i = 0; i < s / 2; i++) {
			result.push_back(lp->distTo(i));
		}
		return result;
	}

	double totalTime() {
		return lp->distTo(t);
	}
};

class Arbitrage {

public:
 	Arbitrage(const char* filename) {
		FILE* fp = fopen(filename, "r");
		assert(fp);
		int N;
		fscanf(fp, "%d", &N);
		auto g = new EdgeWeightedDigraph(N);
		char str[4];
		double a[5];
		for (int i = 0; i < N; ++i) {
			fscanf(fp, "%s %lf %lf %lf %lf %lf", str, &a[0], &a[1], &a[2], &a[3], &a[4]);
			for (int j = 0; j < N; ++j) 
				g->addEdge(new DirectedEdge(i, j, -log(a[j])));
		}

		auto sp = new BellmanFordSP(g, 0);

		if (sp->hasNegativeCycle()) {
			for (auto i : sp->negativeCycle()) {
				cout << i << " ";
			}
			cout << endl;
		}

		delete g;
		delete sp;
	}

	~Arbitrage() {

	}
};

#endif // !_L_EWD_H
