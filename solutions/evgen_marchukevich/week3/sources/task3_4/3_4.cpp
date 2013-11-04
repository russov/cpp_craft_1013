#include <iostream>
#include <fstream>
#include <cstdio>
#include <math.h>
#include <string>
#include <cstring>
#include <list>
#include <queue>
#include <set>
#include <map>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include "BinRW.h"
#include <boost\thread.hpp>
#include <boost\lexical_cast.hpp>

using namespace std;


class Solution
{

private:
	
	static int file_id;
	static const int file_cnt = 999;
	static const size_t threads_count = 4;	
	bool error;
	boost :: mutex Mutex;

	bool check(const BinRW ::msg &t, const int max)
		{
			if (t.type<1 || t.type>4) return 0;
			if (max==-1) return 1;
			if (t.time <=max-2) return 0;
			return 1;
		}

	
string Get_in_file(int id)
	{
		char s[4];
		sprintf(s,"%03d",id);			
		return "/input_"+boost :: lexical_cast<string>(s)+".txt";
	}

string Get_out_file(int id)
	{
		char s[4];
		sprintf(s,"%03d",id);			
		return "/output_"+boost :: lexical_cast<string>(s)+".txt";
	}



public:

	Solution()
	{
	}

	~Solution()
	{		
	}


	
	void flows()
	{		
		boost::thread_group threads_group;
		for( size_t i = 0; i < threads_count; ++i )
		{
			threads_group.create_thread( boost::bind( &Solution::process, this ));
		}
		threads_group.join_all();
	}

	void process()
		{
			BinRW RR;
			bool open_file=false;
			{				
				boost :: mutex :: scoped_lock lock(Mutex);
				while(file_id<=file_cnt)
				{
					RR.BinOpen_inFile((SOURCE_DIR+Get_in_file(file_id)).c_str());								
					if (!RR.Bin_isOpen_inFile())
						{						
							file_id++;
							continue;
						}
					RR.BinOpen_outFile((SOURCE_DIR+Get_out_file(file_id)).c_str());						
					file_id++;
					open_file=true;
					break;
				}
			}
			if (!open_file) return;
			int T = -1;
			vector<BinRW :: msg> ans;						
			ans.clear();
			while (1)
			{
				BinRW :: msg x;
				RR.BinReader(x);
				if (!RR.Bin_nice()) break;
				if (!check(x,T)) continue;
				ans.push_back(x);
				T=max(T,x.time);
			}		
				for(vector<BinRW :: msg> :: iterator it=ans.begin();it!=ans.end();it++)
					RR.BinWriter(*it);						
				RR.BinClose_in();
				RR.BinClose_out();

		}
	

};


int Solution :: file_id = 1;


int main()
{
	Solution x;
	x.flows();	
	return 0;
}