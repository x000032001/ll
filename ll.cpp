#include "ll.h"

ll::ll( vector<production> &ps , first_follow_set &f )
{
	for_it( it , ps )
	{
		set<string> pset = find_predict_set( *it , f );

		for_it( iter , pset )
		{
			production &p = table[ it->LHS ][ *iter ];
			if( p.ruleNum != UNINIT_NUM )
			{
				cout << "ERROR find conflict entry(more than one):" << endl;
				cout << "ll table[" << (it->LHS) << "][" << (*iter) << "] already have rule:" << endl;
				cout << p.ruleNum << ":" << p.origin << endl;
				cout << "but I am trying to fill rule:" << endl;
				cout << (it->ruleNum) << ":" << (it->origin) << endl;
				exit(1);
			}
			
			table[ it->LHS ][ *iter ] = *it;
		}
	}
}

void ll::parse( vector<production> &ps , terminals &t , first_follow_set &f , deque<string> context )
{
	vector<string> pool;
	pool.push_back( ps[0].LHS );

	deque<string> context_c(context);

	while( !pool.empty() && !context.empty() )
	{
		puts("----Status------------------------------------");
		printStack(pool);
		printDeque(context);

		string nt = pool.back();
		string code = context.front();
		cout << "current Non-terminal:(" << nt << ")\t" << "terminal symbol:(" << code << ")" << endl;
		puts("----Action------------------------------------");

		if( nt == code )
		{
			cout << "- Match symbol (" << code << ")" << endl;
			pool.pop_back();
			context.pop_front();
		}
		else if( !t.isTerminal(nt) &&
				table[nt].find(code) != table[nt].end() )
		{
			production &p = table[nt][code];
			cout << "- Apply rule (" << p.ruleNum << ")." << p.origin;
			pool.pop_back();

			for( auto it = p.RHS.rbegin() ; it != p.RHS.rend() ; ++it )
			{
				if( *it != NIL )
				{
					pool.push_back( *it );
				}
				else
				{
					cout << "--- Ignore null in rhs" << endl;
				}
			}
		}
		else if( table[nt].find( NIL ) != table[nt].end() || t.isNullable(nt) )
		{
			cout << "- REMOVE null (shouldn't do this)." << endl;
			pool.pop_back();
		}
		else
		{
			puts("- ERROR no action can do. exited.");
			return;
		}
		puts("----------------------------------------------\n");
	}

	while( !pool.empty() )
	{
		string nt = pool.back();
		cout << " WARN: still not empty in stack (" << nt << ")" << endl;
		if( table[nt].find( NIL ) != table[nt].end() || t.isNullable(nt) )
		{
			pool.pop_back();
			cout << "    " << nt << "has been removed." << endl;
		}
		else
		{
			cout << "    " << nt << "is not null. exited." << endl;
			break;
		}
	}

	if( context.size() == 0 && pool.size() == 0 )
	{
		puts("context accepted.");
		parse_dfs( ps , t , f , context_c );
	}
	else
	{
		puts("context NOT accepted.");
		puts("----Status-----------------------------------");
		printStack(pool);
		printDeque(context);
		puts("---------------------------------------------");
	}
}


void ll::parse_dfs( vector<production> &ps , 
		  terminals &t ,
		  first_follow_set &f ,
		  deque<string> context
		)
{
	tree.clear();

	cout << " parsing tree =================================" << endl;
	dfs( ps , t , f , context , ps[0].LHS , 0 , "- " );

	drawTree();
	//for_it( it , tree )
		//cout << (*it) << endl;
	cout << " ==============================================" << endl;
}

void ll::drawTree()
{
	char map[100][100] = {};

	for( size_t i = 0 ; i < tree.size() ; ++i )
	{
		size_t j;
		for( j = 0 ; j < tree[i].size() ; ++j )
			map[i][j] = tree[i][j];
		map[i][j] = '\n';
	}

	for( int i = 0 ; i < 100 ; ++i )
		for( int j = 0 ; map[i][j]  ; ++j )
			putchar( map[i][j] );
}

void ll::dfs( vector<production> &ps , 
		  terminals &t ,
		  first_follow_set &f ,
		  deque<string> &context ,
		  string ntp ,
		  int depth ,
		  string prefix
		)
{
		if( context.size() == 0 )
			return;

		string nt = ntp;
		string code = context.front();


		if( nt == code )
		{
			tree.push_back( 
				string(prefix + nt + " == " + code)  );
			context.pop_front();
		}
		else if( !t.isTerminal(nt) &&
				 hasEle( table[nt] , code ) )
		{
			tree.push_back( 
				string(prefix + nt)  );
			prefix = "   " + prefix;


			production &p = table[nt][code];

			for_it( it , p.RHS )
			{
					if( *it != NIL )
						dfs( ps , t , f , context , *it , depth+1 , prefix );
					else
						tree.push_back( 
							string(prefix + NIL + " == " + NIL)  );
			}
		}
}

set<string> ll::find_predict_set( production &p , first_follow_set &f )
{
	set<string> res;
	set<string> first = f.find_first( p.RHS );

	if( SUCCESS == first.erase( NIL ) )
		res.insert( f.follow_set[p.LHS].begin() , f.follow_set[p.LHS].end() );

	res.insert(first.begin() , first.end());

	return res;
}

void ll::print()
{
	set<string> column;

	for_it( it , table )
	{
		for_it( iter , it->second )
			column.insert( iter->first );
	}

	for( size_t i = 0 ; i < (column.size()+1)*9-1 ; ++i )
		putchar('-');
	putchar('\n');

	cout << "|\t\t";
	for_it( iter , column )
		cout << "|" << (*iter) << "\t";
	cout << "|" << endl;

	for( size_t i = 0 ; i < (column.size()+1)*9-1 ; ++i )
		putchar('-');
	putchar('\n');


	for_it( it , table )
	{
		cout << "|" << (it->first) << "\t\t";
		map<string,production>& smap = it->second;
		for_it( iter , column )
		{
			auto t = smap.find( *iter );
			if( t != smap.end() )
				cout << "|" << (smap[*iter].ruleNum) << "\t";
			else
				cout << "|" << "    "   << "\t";
		}
		cout << "|" << endl;
	}

	for( size_t i = 0 ; i < (column.size()+1)*9-1 ; ++i )
		putchar('-');
	putchar('\n');


}

void ll::printStack( vector<string> &stk )
{
	cout << "Stack :" << endl;
	cout << "bottom [ ";
	for_it( it , stk )
	{
		cout << "\t" << (*it);
	}
	cout << " ] top";
	cout << endl;
}

void ll::printDeque( deque<string> &que )
{
	cout << "Queue :" << endl;
	cout << "front [ ";
	for_it( it , que )
	{
		cout << "\t" << (*it);
	}
	cout << " ] back";
	cout << endl;
}




