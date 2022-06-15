#include "lane.h"

using namespace std;

ostream &Lane::Print(ostream &os) const { return os; }

ostream &operator<<(ostream &os, const Lane &lane) { return lane.Print(os); }
