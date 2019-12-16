#ifndef INCLUDE_DAY16_HPP_
#define INCLUDE_DAY16_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
#include<map>
#include<queue>
#include<memory>

using namespace std;

struct day16{

	struct sumCache{

		sumCache(const vector<int64_t> &nums){
			cache.push_back(nums[0]);
			for(uint64_t i=1;i<nums.size();i++)
				cache.push_back(cache.back()+nums[i]);
		}

		int64_t at(uint64_t start,uint64_t len){

			if(start>=cache.size())
				return 0;

			if(!(start+len<cache.size()))
				len=cache.size()-start-1;

			int64_t res= cache[start+len];

			if(start>0)
				res-=cache[start-1];

			return res;
		}
		std::vector<int64_t> cache;
	};

	vector<int64_t> runPhase(vector<int64_t>& a){

		std::vector<int64_t> res(a.size(),0);
		sumCache cache(a);

		for(uint64_t i=0;i<a.size();){
			uint64_t start=i;
			uint64_t inc=(i+1)*2;
			while(start<a.size())
			{
				res[i]+=cache.at(start,i);
				start+=inc;
				res[i]-=cache.at(start,i);
				start+=inc;
			}
			i++;
		}

		for(auto&v:res)
		{
			if(v<0)
				v*=-1;
			v=v%10;
		}

		return res;

	}

	day16(const string& filename){
		ifstream in(filename);
		string s;
		in>>s;
		for(auto&v:s)
			data.push_back(v-'0');

		indexResult2=std::atoi(s.substr(0,7).c_str());


	}

	void getResult(){
		vector<int64_t> d2=data;
		for(int i=0;i<100;i++)
			d2=runPhase(d2);

		cout<<"Transform 1 result: ";
		for(int i=0;i<8;i++)
			cout<<d2[i];
		cout<<endl;
	}

	void getResult2(){
		vector<int64_t> d2;
		for(unsigned i=0;i<10000;i++)
			d2.insert(d2.end(),data.begin(),data.end());

		cout<<"Processing transform 2";
		for(int i=0;i<100;i++)
		{
			d2=runPhase(d2);
			cout<<". ";
			cout.flush();
		}

		cout<<endl<<"Transform 2 result: ";
		for(int64_t i=indexResult2;i<indexResult2+8;i++)
			cout<<d2[i];
		cout<<endl;

	}

	vector<int64_t> data;
	int64_t indexResult2;
};


#endif
