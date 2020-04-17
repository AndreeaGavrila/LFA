#include <iostream>
#include <set>
#include <map>
#include <string>

using namespace std;

class DFA
{
	set<int> Q, F;
	set<char> Sigma;
	int q0;
	map<pair<int, char>, int> delta;

public:
	DFA() { this -> q0 = 0; }
	DFA( set<int> Q,
         set<char> Sigma,
         map<pair<int,char>,int> delta,int q0,
         set<int> F)

	{
		this -> Q = Q;
		this -> Sigma = Sigma;
		this -> delta = delta;
		this -> q0 = q0;
		this -> F = F;
	}

	bool isFinalState(int);
	int deltaStar(int, string);
};

bool DFA::isFinalState(int q)
{
	return F.find(q) != F.end();
}

int DFA::deltaStar(int q, string w)
{
	if (w.length() == 0)
		return q;

	if (w.length() == 1)
        return delta[{q, (char)w[0]}];

	int new_q = delta[{q, (char)w[0]}];

	return deltaStar( new_q, w.substr(1,w.length() -  1) );
}

int main()
{
	DFA M( {0,1,2},
           {'a','b'},
           { {{0,'a'},1},  {{1,'b'}, 2}  },
           0,
           {2} );

    cout << M.isFinalState(M.deltaStar(0,"ab"));
	return 0;
}
