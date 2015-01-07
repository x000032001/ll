#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <deque>
#include <stack>

#include "production.h"
#include "terminals.h"
#include "ffset.h"
#include "ll.h"

using namespace std;

vector<string> readfile( const char* name )
{
	FILE* fin = fopen( name , "r" );
	
	if( !fin )
	{
		cout << "error: unable to open file: " << name << endl;
		exit(1);
	}
	
	char buff[256] = {};
	
	vector<string> res;

	while( fgets( buff , 256 , fin ) )
		res.push_back( string(buff) );

	fclose(fin);

	return res;
}

vector<production> readcfg( vector<string> &raw , const string& a , const char& b )
{
	vector<production> res;
	int count = 1;
	for_it( it , raw )
		res.push_back(production( *it , count++ , a , b ));	

	return res;
}

deque<string> readcode( const char* name , const char* delim )
{
	FILE* fin = fopen( name , "r" );
	
	if( !fin )
	{
		cout << "error: unable to open file: " << name << endl;
		exit(1);
	}

	char buff[256] = {};
	
	deque<string> res;

	while( fgets( buff , 256 , fin ) )
	{
		char* pch = strtok( buff , delim );
		while( pch )
		{
			string tmp = pch;
			tmp = production::trim(tmp);
			if( tmp != "" )
				res.push_back(tmp);
			pch = strtok( NULL , delim );
		}
	}

	fclose(fin);

	return res;
}


int main()
{
	const char code_delim[] = " ;"; // only use in code split
	const string prod_delim = ">";
	const char rhs_delim = ' ';

	vector<string> raw = readfile("grammer.txt");
	vector<production> ps = readcfg( raw , prod_delim , rhs_delim );

	cout << "terminals result ===================" << endl;
	terminals t(ps);
	t.print();
	cout << "====================================" << endl;

	cout << "first and follow set result ========" << endl;
	first_follow_set f( ps , t );
	f.print();
	cout << "====================================" << endl;

	cout << " ll table result ===================" << endl;
	ll l( ps , f );
	for_it( it , ps )
		printf("  (%2d). %s" , it->ruleNum , it->origin.c_str() );
	cout << endl;
	l.print();
	cout << "====================================" << endl;

	deque<string> context = readcode( "code.txt" , code_delim );

	l.parse( ps , t , f , context );
		
	return 0;
}
