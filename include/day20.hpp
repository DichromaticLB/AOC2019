#ifndef INCLUDE_DAY20_HPP_
#define INCLUDE_DAY20_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
#include<map>
#include<queue>
#include<memory>
#include<stack>

using namespace std;

	struct day20{

	using pos=pair<uint32_t,uint32_t>;


		day20(const string& filename){
			ifstream i(filename);
			string c;
			getline(i,c);
			while(i.good())
			{
				m.push_back(c);
				getline(i,c);
			}

			map<string,pos> cache;

			for(uint32_t y=1;y<m.size()-1;y++)
			{
				for(uint32_t x=1;x<m[y].size()-1;x++)
				{
					bool entrance=false;
					char other=0;
					uint32_t ex=x,ey=y,ox=x,oy=y;
					char cr=get(x,y);
					if(isalpha(cr))
					{
						vector<pos> ar={{x+1,y},{x-1,y},{x,y+1},{x,y-1}};
						for(auto&p:ar)
						{
							char c=get(p.first,p.second);
							if(c=='.')
							{
								entrance=true;
							}
							if(isalpha(c))
							{
								other=c;
								ox=p.first;
								oy=p.second;
							}
						}

						if(!entrance)
						{
							ex=ox;
							ey=oy;
						}


						string s;
						string s2;
						s+=cr;s+=other;
						s2+=other;s2+=cr;
						if(cache.count(s))
						{
							portals[pos(ex,ey)]=cache[s];
							portals[cache[s]]=pos(ex,ey);
						}
						else if (cache.count(s2))
						{
							portals[pos(ex,ey)]=cache[s2];
							portals[cache[s2]]=pos(ex,ey);
						}
						else
							cache[s]=pos(ex,ey);

						get(ex,ey)='*';
						if(entrance)
						{
							get(ox,oy)='#';
						}
						else
							get(x,y)='#';
					}
				}
			}
			entrance=cache["AA"];
			exit=cache["ZZ"];

			if(get(entrance)!='*')
				for(auto&v:around(entrance))
					if(isalpha(get(v)))
					{
						entrance=v;
						break;
					}

			if(get(exit)!='*')
				for(auto&v:around(exit))
					if(isalpha(get(v)))
					{
						exit=v;
						break;
					}

		}

		char& get(uint32_t x,uint32_t y){
			return m[y][x];
		}
		char& get(pos p){return get(p.first,p.second);}

		vector<pos> around(pos p){return around(p.first,p.second);}
		vector<pos> around(uint32_t x,uint32_t y)
		{
			vector<pos> res;
			if(x>0)
			{
				res.emplace_back(x-1,y);
			}

			if(x<m[0].size()-1)
			{
				res.emplace_back(x+1,y);
			}

			if(y>0)
			{
				res.emplace_back(x,y-1);
			}

			if(y<m.size()-1)
			{
				res.emplace_back(x,y+1);
			}


			return res;
		}

		void draw(){
			for(auto&s:m)
				cout<<s<<endl;
		}

		void getResult(){
			queue<pos> remaining;
			map<pos,uint64_t> distances;
			distances[entrance]=0;
			remaining.emplace(entrance);

			while(remaining.size())
			{
				auto ar=around(remaining.front());
				uint64_t cost=distances[remaining.front()]+1;
				uint64_t costPortal=distances[remaining.front()];
				remaining.pop();
				for(auto&v:ar)
				{
					char c=get(v);
					if(c=='.'||v==entrance||v==exit)
					{
						if(!distances.count(v)||distances[v]>cost)
						{
							distances[v]=cost;
							remaining.emplace(v);
						}
					}
					else if(c=='*')
					{
						pos other=portals[v];
						if(!distances.count(other)||distances[other]>costPortal)
						{
							distances[other]=costPortal;
							remaining.emplace(other);
						}
					}

				}

			}

			cout<<"Distance problem 1:"<<distances[exit]-2<<endl;

		}

		struct levelPos:public pos
		{
			uint32_t level;
			pos getp(){
				return pos(first,second);
			}
		};

		bool isOuter(pos p){
			if(p.first<5||p.first>m[0].size()-6)
				return true;

			if(p.second<5||p.second>m.size()-6)
				return true;

			return false;
		}

		void getResult2(){
			queue<levelPos> remaining;
			vector<map<pos,uint64_t>> distances;
			distances.resize(100);

			distances[0][entrance]=0;
			levelPos p;
			p.first=entrance.first;p.second=entrance.second;p.level=0;
			remaining.emplace(p);
			while(remaining.size())
			{
				auto currPos=remaining.front();
				auto ar=around(currPos.getp());
				remaining.pop();
				uint64_t cost=distances[currPos.level][currPos.getp()]+1;
				uint64_t costPortal=distances[currPos.level][currPos.getp()];

				for(auto&v:ar)
				{
					char c=get(v);
					if(c==' '||c=='#')
						continue;

					if(v==entrance||v==exit)
					{
						if(currPos.level==0)
							c='.';
						else
							c='#';
					}

					if(c=='.')
					{
						if(!distances[currPos.level].count(v)||distances[currPos.level][v]>cost)
						{
							distances[currPos.level][v]=cost;

							levelPos p2;
							p2.first=v.first;
							p2.second=v.second;
							p2.level=currPos.level;
							remaining.emplace(p2);
						}
					}
					else if(c=='*')
					{
						if(isOuter(v)&&currPos.level==0)
							continue;

						uint32_t nextLevel=currPos.level;
						if(isOuter(v))
							nextLevel--;
						else
							nextLevel++;

						if(nextLevel>=distances.size())
							continue;

						pos other=portals[v];

						if(!distances[nextLevel].count(other)||distances[nextLevel][other]>costPortal)
						{
							distances[nextLevel][other]=costPortal;
							levelPos p2;
							p2.first=other.first;
							p2.second=other.second;
							p2.level=nextLevel;
							remaining.emplace(p2);
						}
					}

				}

			}

			cout<<"Distance problem 2:"<<distances[0][exit]-2<<endl;

		}
		pos entrance,exit;
		map<pos,pos> portals;
		vector<string> m;
};

#endif
