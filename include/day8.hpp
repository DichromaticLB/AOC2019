#ifndef INCLUDE_DAY8_HPP_
#define INCLUDE_DAY8_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
#include<map>
using namespace std;

struct day8{

	day8(const string& filename){
		ifstream in(filename);
		char c;
		in>>c;
		uint32_t index=0;

		while(in.good())
		{
			if(!(index%150))
				layers.emplace_back();

			layers.back().push_back(c);
			in>>c;
			index++;
		}


	}

	uint32_t count(char c,vector<char>&v)
	{
		uint32_t res=0;
		for(auto&cc:v)
			if(cc==c)
				res++;
		return res;
	}

	void getResult(){
		uint32_t min=150,dex=0,it=0;

		for(auto&vec:layers)
		{
			if(count('0',vec)<min)
			{
				min=count('0',vec);
				dex=it;
			}
			it++;
		}

		std::cout<<(count('1',layers[dex])*count('2',layers[dex]))<<endl;

	}

	void getResult2(){
		std::vector<char> res(150,' ');
		for(uint32_t i=0;i<150;i++)
		{
			for(auto&v:layers)
			{
				if(v[i]!='2')
				{
					res[i]=(v[i]=='0')?' ':'#';
					break;
				}
			}
		}

		uint32_t dex=0;
		for(auto&v:res)
		{

			std::cout<<v;
			dex++;
			if(!(dex%25))
				std::cout<<endl;
		}


	}

	std::vector<std::vector<char>> layers;

};
#endif
