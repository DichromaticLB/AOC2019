#ifndef INCLUDE_DAY14_HPP_
#define INCLUDE_DAY14_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
#include<map>
#include<queue>
#include<memory>

using namespace std;

struct day14{

	struct element{
		std::string name;
		uint32_t count;
		bool operator<(const element& other) const{
			if(name==other.name)
				return count<other.count;
			return name<other.name;
		}
	};

	using molecule=vector<element>;
	using reaction=map<element,molecule>;




	day14(const string& filename){
		ifstream i=ifstream("input/day14");
		element b;
		molecule m;
		bool result=false;
		while(i.good())
		{
			if(i.peek()=='=')
			{
				result=true;
				i.ignore();

			}
			char c=i.peek();
			if((c>='0'&&c<='9')||(c>='A'&&c<='Z'))
			{
				i>>b.count;
				i.ignore();
				i>>b.name;
				if(b.name.back()==',')
					b.name=b.name.substr(0,b.name.size()-1);
				if(result)
				{
					reactions[b]=m;
					m.clear();
					result=false;
				}
				else
				{
					m.push_back(b);
				}
			}
			else
				i.ignore();
		}
	}

	void minCost(std::map<string,int64_t>& required){

		while(true){
			bool finish=true;
			for(auto&v:required)
			{
				if(v.second<0&&v.first!="ORE")
				{
					finish=false;
					molecule mol;
					for(auto&m:reactions)
					{
						if(m.first.name==v.first)
						{

							int64_t needed=-v.second/m.first.count;
							if(-v.second%m.first.count)
								needed++;

							v.second+=needed*m.first.count;
							for(auto&elem:m.second)
								required[elem.name]+=-needed*elem.count;
							break;
						}
					}
					break;
				}
			}

			if(finish)
				break;
		}
	}
	using proportion=std::map<string,int64_t>;
	void getResult(){
		proportion required={{"FUEL",-1}};
		minCost(required);
		cout<<"Requires a total of "<<-required["ORE"]<<" For a singular unit of fuel"<<endl;
	}

	void getResult2(){

		proportion unit={{"FUEL",-100}};
		minCost(unit);

		int64_t limit=1000000000000;
		int64_t cost=-unit["ORE"];
		int64_t step=limit/cost,done=0;
		proportion iter={{"FUEL",0}};

		while(step>10)
		{
			step/=2;
			done+=step;
 			iter["FUEL"]=-step*100;
			minCost(iter);
		}
		done*=100;
		while(iter["ORE"]+limit>0)
		{
			iter["FUEL"]=-1;
			minCost(iter);
			done++;
		}
		cout<<"Could make a total of "<<(done-1)<<" units of fuel with "<<limit<<" ore"<<endl;


	}


	reaction reactions;
};
#endif
