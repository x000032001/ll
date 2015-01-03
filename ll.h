#ifndef LH
#define LH

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <deque>
#include <stack>

#include "terminals.h"
#include "production.h"
#include "ffset.h"

#define for_it(a,b) for( auto a = b.begin() ; a != b.end() ; ++a )

#define SUCCESS 1
#define FAILURE 0

using std::map;
using std::vector;
using std::set;
using std::cout;
using std::endl;
using std::deque;

class ll {
public:

	ll( vector<production>& , terminals& , first_follow_set& );
	void parse( vector<production>& , terminals& , first_follow_set& , deque<string> );
	set<string> find_predict_set( production& , terminals& , first_follow_set& );
	void print();
	void printStack( vector<string>& );
	void printDeque( deque<string>& );

private:
	map<string ,map<string,production>> table;
};


class node {
	public:
	string s;
	string end;
	int pos;
	vector<node> child;

	node(string ss)
	{ s = ss; }

void treePrint( node& t )
{
	cout << "s: " << t.s;
	if( t.end != "" )
	{
		cout << "end: " << endl;
		return;
	}

	for( int i = 0 ; i < t.child.size() ; ++i )
		treePrint( t.child[i] );
}


};


#endif
