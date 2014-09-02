#ifndef _L_EDGE_H
#define _L_EDGE_H
#include "l.h"

struct Edge{
	int v;
	int w;
	double weight;

	Edge(int v, int w, double weight) : v(v), w(w), weight(weight){}

	bool operator>(Edge &e) { return this->weight > e.weight; }
	bool operator<(Edge &e) { return this->weight < e.weight; }
	bool operator==(Edge &e) { return this->weight == e.weight; }
	bool operator!=(Edge &e) { return this->weight != e.weight; }

	friend ostream& operator<<(ostream& out, Edge &e);

	int other(int u) {
		if (u == v) return w;
		else return v;
	}
};

ostream& operator<<(ostream& out, Edge &e) { out << e.v << "-" << e.w << " " << fixed << setprecision(2) << e.weight; return out; }

#endif // !_L_EDGE_H
