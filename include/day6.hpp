#ifndef INCLUDE_DAY6_HPP_
#define INCLUDE_DAY6_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
#include<map>
using namespace std;

struct elem{
	elem(){
		count=-1;
		distance=numeric_limits<int64_t>::max();
	}
	std::vector<std::string> orbits;
	std::vector<std::string> orbiting;

	int64_t getCount(std::map<std::string,elem>& celestials){
		if(count!=-1)
			return count;

		count =0;
		for(auto&v:orbiting)
		{
			count=1+celestials[v].getCount(celestials);
		}

		return count;
	}

	void distances(std::map<std::string,elem>& celestials){

		for(auto&v:orbits)
			if(celestials[v].distance>(distance+1))
			{
				celestials[v].distance=distance+1;
				celestials[v].distances(celestials);
			}

		for(auto&v:orbiting)
			if(celestials[v].distance>(distance+1))
			{
				celestials[v].distance=distance+1;
				celestials[v].distances(celestials);
			}
	}


	int64_t distance;
	int64_t count;
};

struct day6{

	day6(const string& filename){
		ifstream in(filename);
		std::string i;
		in>>i;
		celestials["COM"];
		while(!in.fail())
		{

			std::string orbitee=i.substr(0,3);
			std::string orbiter=i.substr(4,3);
			celestials[orbitee].orbits.push_back(orbiter);
			celestials[orbiter].orbiting.push_back(orbitee);
			in>>i;
		}

		celestials["YOU"].distance=0;
	}

	void getResult(){
		int64_t res=0;

		for(auto&v:celestials)
		{
			res+=v.second.getCount(celestials);
		}
		std::cout<<"Orbits + indirect orbits: "<<res<<endl;
	}

	void getResult2(){
		celestials[celestials["YOU"].orbiting[0]].distance=0;
		celestials[celestials["YOU"].orbiting[0]].distances(celestials);
		std::cout<<"Minimum number of jumps to get in orbit along SAN:"<<celestials[celestials["SAN"].orbiting[0]].distance<<endl;
	}

	std::map<std::string,elem> celestials;

};



#endif
