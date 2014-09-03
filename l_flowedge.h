#ifndef _L_FLOWEDGE_H
#define _L_FLOWEDGE_H

#include "l.h"

class FlowEdge {
	int v;
	int w;
	double flow;
	double capacity;

public:
	FlowEdge(int v, int w, double c) : v(v), w(w), capacity(c), flow(0.0){}
	int from() { return v; }
	int to() { return w; }
	int other(int i) {
		if (i == v) return w;
		else return v;
	}

	double residualCapacityTo(int i) {
		// from
		if (i == v) return flow;
		// to
		else if (i == w) return capacity - flow;
		else throw(runtime_error("wrong vertice"));
	}

	void addResidualFlowTo(int i, double delta) {
		// from
		if (i == v) flow -= delta;
		// to
		else if (i == w) flow += delta;
		else throw(runtime_error("wrong vertice"));
	}

	friend ostream& operator<<(ostream& out, FlowEdge &e) {
		out << e.v << "->" << e.w << " " << fixed << setprecision(2) <<  e.capacity << " " << e.flow;
		return out;
	}
};
#endif // !_L_FLOWEDGE_H
