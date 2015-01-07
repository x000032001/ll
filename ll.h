#ifndef LH
#define LH

#include <iostream>
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
#define hasEle(a,b) a.find(b) != a.end()

#define SUCCESS 1
#define FAILURE 0

#define END_SYM "$"

using std::map;
using std::vector;
using std::set;
using std::cout;
using std::endl;
using std::deque;

class ll {
public:

	ll( vector<production>& , first_follow_set& );
	void parse( vector<production>& , terminals& , first_follow_set& , deque<string> );
	static set<string> find_predict_set( production& , first_follow_set& );
	
	void print();
	static void printStack( vector<string>& );
	static void printDeque( deque<string>& );

	void dfs( vector<production>& , 
			  terminals& ,
			  first_follow_set& ,
			  deque<string>& ,
			  string ,
			  int ,
			  string 
			);

	void parse_dfs( vector<production>& , 
			  terminals& ,
			  first_follow_set& ,
			  deque<string> 
			);

	void drawTree();

private:
	map<string ,map<string,production>> table;
	vector<string> tree;
};

#endif
