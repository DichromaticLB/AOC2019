#ifndef INCLUDE_DAY22_HPP_
#define INCLUDE_DAY22_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
#include<map>
#include<queue>
#include<memory>
#include<stack>
#include"day9.hpp"

using namespace std;

	struct day22{
#define CUT 0
#define DEALNEWSTACK 1
#define DEALINCREMENT 2

		struct move{
			char action;
			int64_t parameter;
		};



		day22(const string& filename){
			ifstream i(filename);
			move m;
			string action;
			i>>action;
			while(i.good())
			{
				if(action[0]=='c')
				{
					m.action=CUT;
					i>>m.parameter;

				}
				else{
					i>>action;
					if(action[0]=='i')
					{
						m.action=DEALNEWSTACK;
						i>>action;
						i>>action;
					}
					else
					{
						m.action=DEALINCREMENT;
						i>>action;
						i>>m.parameter;
					}
				}
				moves.push_back(m);

				i.ignore();
				i>>action;

			}

		}

		uint64_t track(uint64_t pos,uint64_t total)
		{
			for(move m:moves)
			{
				if(m.action==CUT)
				{
					if(m.parameter<0)
						m.parameter+=total;

					if((int64_t)pos<m.parameter)
						pos=total-m.parameter+pos;
					else
						pos-=m.parameter;

				}
				else if(m.action==DEALNEWSTACK)
				{
					pos=total-pos-1;
				}
				else
				{
					pos=(pos*m.parameter)%total;
				}

			}

			return pos;
		}


		void getResult(){
			cout<<track(2019,10007)<<endl;
		}

		void getResult2(){


		}
		vector<move> moves;
};

#endif
