#ifndef _L_DEDGE_H
#define _L_DEDGE_H

#include "l.h"
class DirectedEdge {
private:
	int v;
	int w;
	double wei;
public:
	DirectedEdge(int v, int w, double we) : v(v), w(w), wei(we) {}
	int from() { return v; }
	int to() { return w; }
	double weight() { return wei; }

	bool operator<(DirectedEdge& e) { return this->weight() < e.weight(); }
	bool operator>(DirectedEdge &e) { return this->weight() > e.weight(); }

	friend ostream& operator<< (ostream& out, DirectedEdge &e);

};

ostream& operator<< (ostream &out, DirectedEdge &e) {
	out << e.from() << "->" << e.to() << " " << fixed << setprecision(2) << e.weight();
	return out;
}
#endif // !_L_DEDGE_H
