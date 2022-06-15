#include "line.h"

using namespace std;

ostream &Line::Print(ostream &os) const { return os; }

ostream &BorderLine::Print(ostream &os) const { return os; }

ostream &CenterLine::Print(ostream &os) const { return os; }

ostream &operator<<(ostream &os, const Line &line) { return line.Print(os); }
