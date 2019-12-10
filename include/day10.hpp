#ifndef INCLUDE_DAY10_HPP_
#define INCLUDE_DAY10_HPP_

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

using namespace std;

#define PI (atan(1)*4)
	struct tg2{

		int x,y,destroyed;
		double distance;
		double at2()const {return atan2(-y,x);}

		double angle()const{

			double res=at2()*(180/PI);
			if(res<0)
				res+=360;
			return res;
		}


		bool operator<(const tg2& other)const{

			if(angle()>other.angle())
				return !(other<*this);

			if(angle()<=90)
			{
				if(other.angle()<=90)
					return angle()>other.angle();
				else
					return true;
			}

			return angle()>other.angle();

		}

		double dist()const{
				return sqrt(x*x+y*y);
			}

	};



struct day10{



	day10(const string& filename){
		ifstream in(filename);
		std::string line;
		in>>line;

		while(in.good())
		{
			field.emplace_back();
			for(auto&c:line)
				field.back().push_back(c);

			in>>line;

		}
	}

	void getResult(int x,int y){

		std::map<tg2,std::vector<tg2>> ordered;
		for(int i=0;i<(int)field.size();i++)
			for(int j=0;j<(int)field[0].size();j++)
			{
				if(field[i][j]=='.')
					continue;
				tg2 t={j-x,i-y,false,0};
				ordered[t].push_back({j,i,false,t.dist()});
			}
		cout<<"A total of "<<ordered.size()<<" are in the line of sight"<<endl;

	}


	void getResult2(int x,int y)
	{
		unsigned count=0;
		std::map<tg2,std::vector<tg2>> ordered;
		for(int i=0;i<(int)field.size();i++)
				for(int j=0;j<(int)field[0].size();j++)
				{
					if(field[i][j]=='.')
						continue;
					tg2 t={j-x,i-y,false,0};
					ordered[t].push_back({j,i,false,t.dist()});
				}

		for(auto&v:ordered)
			std::sort(v.second.begin(),v.second.end(),[](const tg2&a,const tg2&b){return a.dist()>b.dist();});

		while(true)
			for(auto&v:ordered)
			{
				for(auto&vv:v.second)
				{
					if(!vv.destroyed)
					{
						vv.destroyed=true;
						count++;
						if(count==200)
						{
							std::cout<<"Number 200: "<<vv.x<<","<<vv.y<<endl;
							return;
						}
						break;
					}
				}
			}

	}

	std::vector<std::vector<char>> field;
};

#endif
