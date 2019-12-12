#ifndef INCLUDE_DAY12_HPP_
#define INCLUDE_DAY12_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
#include<map>
#include<queue>
#include<memory>
#include<cmath>
#include<algorithm>
#include<numeric>
#include<set>
#include<cassert>

using namespace std;

struct moon{
	int64_t pos[3]={0};
	int64_t vel[3]={0};

	void move(){
		pos[0]+=vel[0];
		pos[1]+=vel[1];
		pos[2]+=vel[2];
	}

	uint64_t totalE(){
		return (abs(pos[0])+abs(pos[1])+abs(pos[2]))*(abs(vel[0])+abs(vel[1])+abs(vel[2]));
	}
};

istream& operator>>(istream& i,moon& m)
{

	unsigned index=0;
	while(i.good()&&index<3)
	{
		char peek=i.peek();
		if(peek=='\n'||peek=='<'||peek=='>'||peek=='='||peek==','||peek==' '||(peek>='x'&&peek<='z'))
			i.ignore();
		else
			i>>m.pos[index++];
	}
	return i;
}


struct day12{

	day12(const string& filename):moons(4){
		ifstream in(filename);
		in>>moons[0];
		in>>moons[1];
		in>>moons[2];
		in>>moons[3];
	}

#define updateSpeed(x) if(m1.pos[(x)]<m2.pos[(x)])\
							m1.vel[(x)]++;\
						else if(m1.pos[(x)]>m2.pos[(x)])\
							m1.vel[(x)]--;

	void step(std::vector<moon> &moons){
		for(auto&m1:moons)
			for(auto&m2:moons)
			{
				updateSpeed(0);
				updateSpeed(1);
				updateSpeed(2);
			}

		for(auto&moon:moons)
			moon.move();
	}

	void getResult(){
		std::vector<moon> mc=moons;
		for(unsigned i=0;i<1000;i++)
			step(mc);

		std::cout<<"Total energy: "<<std::accumulate(mc.begin(),mc.end(),0,[](int& i,moon&m){return i+m.totalE();})<<endl;
	}

	union sign{
		sign(std::vector<moon>&mc,uint8_t dex){

				xyz.var1=mc[0].pos[dex];
				xyz.var2=mc[1].pos[dex];
				xyz.var3=mc[2].pos[dex];
				xyz.var4=mc[3].pos[dex];
				xyz.sp1=mc[0].vel[dex];
				xyz.sp2=mc[1].vel[dex];
				xyz.sp3=mc[2].vel[dex];
				xyz.sp4=mc[3].vel[dex];
		}


		bool operator<(const sign&other) const{
			if(sig[0]==other.sig[0])
				return sig[1]<other.sig[1];

			return sig[0]<other.sig[0];
		}

		struct {
			int16_t var1,var2,var3,var4,sp1,sp2,sp3,sp4;
		} xyz;
		uint64_t sig[2];
	};

	using cycle=pair<int64_t,int64_t>;

	cycle iterateIndex(uint8_t dex){
		std::vector<moon> mc=moons;
		std::map<sign,uint64_t> pos;
		uint64_t it=0;
		while(true)
		{
			sign p(mc,dex);
			if(pos.count(p))
			{
				return cycle(pos[p],it);
			}
			pos[p]=it;
			step(mc);
			it++;
		}
		return cycle(-1,-1);
	}

	std::map<uint64_t,uint64_t> factor(uint64_t num)
	{
		std::map<uint64_t,uint64_t> res;
		for(uint64_t it=2;it<=num/2;it++)
		{
			while(!(num%it))
			{

				res[it]++;
				num/=it;
			}
		}
		res[num]++;
		return res;
	}

	void itmcm(std::map<uint64_t,uint64_t>& input,std::map<uint64_t,uint64_t>& result)
	{
		for(auto&v:input)
		{
			if(result.count(v.first))
			{
				if(result[v.first]<v.second)
					result[v.first]=v.second;
			}
			else
				result[v.first]=v.second;
		}
	}

	static uint64_t ipow(uint64_t n,uint64_t exp)
	{
		if(exp==1)
			return n;
		if(exp==0)
			return 1;

		if(!(exp%2))
			return ipow(n*n,exp/2);

		return n*ipow(n,exp-1);
	}

	uint64_t mcm(uint64_t n1,uint64_t n2,uint64_t n3){
		auto f1=factor(n1);
		auto f2=factor(n2);
		auto f3=factor(n3);
		std::map<uint64_t,uint64_t> res;
		itmcm(f1,res);
		itmcm(f2,res);
		itmcm(f3,res);
		return std::accumulate(res.begin(),res.end(),1l,[](uint64_t t,auto k){
			return t*ipow(k.first,k.second);
		});

	}

	void getResult2(){

		cycle c1=iterateIndex(0);
		cycle c2=iterateIndex(1);
		cycle c3=iterateIndex(2);
		cout<<"First finished cycle for the whole thing at:"<<mcm(c1.second,c2.second,c3.second)<<endl;
	}

	std::vector<moon> moons;

};

#endif
