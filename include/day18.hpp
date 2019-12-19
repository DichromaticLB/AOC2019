#ifndef INCLUDE_DAY18_HPP_
#define INCLUDE_DAY18_HPP_

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

namespace d18{

#define START '`'

	struct world{

		struct pos:public pair<int64_t,int64_t>{
			pos(int64_t a,int64_t b):pair<int64_t,int64_t>(a,b){}
			pos():pair<int64_t,int64_t>(0,0){}
			pos top(){return pos(this->first,this->second-1);}
			pos bot(){return pos(this->first,this->second+1);}
			pos right(){return pos(this->first+1,this->second);}
			pos left(){return pos(this->first-1,this->second);}
			bool good(){return first!=-1;}
		};

		struct cmp{
			bool operator()(const pos& a,const pos&b)const{
				if(a.first==b.first)
					return a.second<b.second;
				return a.first<b.first;
			}
		};
		world(){
			x=0;
			y=0;
		}
		world(ifstream&i){
			string s;
			i>>s;
			while(i.good())
			{
				c.push_back(s);
				i>>s;
			}

			pos p=find('@');
			x=p.first;
			y=p.second;
			get(x,y)='.';

		}

		pos find(char cc){
			for(unsigned i=0;i<c.size();i++)
				for(unsigned j=0;j<c[i].size();j++)
				{
					if(c[i][j]==cc)
						return pos(j,i);
				}
			return pos(-1,-1);
		}


		void createDependencies(){
			clear();
			for(auto&block:keys){
				if(isupper(block.first))
				{
					get(keys[block.first])=block.first;
					vector<pos> v;
					v.push_back(pos(x,y));
					__costs.clear();
					while(v.size())
					{
						pos p=v.back();
						v.pop_back();
						lookAround(p,v);
					}
					get(keys[toupper(block.first)])='.';
					for(auto&access:keys)
						if(islower(access.first)&&!__costs.count(access.second))
						{
							unlock[tolower(block.first)].push_back(access.first);
							getsUnlock[access.first].push_back(tolower(block.first));
						}

				}
			}

			for(auto&access:keys)
					if(islower(access.first)&&!getsUnlock.count(access.first))
					{
						unlock[START].push_back(access.first);
						getsUnlock[access.first].push_back(START);
					}

		}

		void clear(){
			for(auto& vec:c)
				for(auto&c:vec)
					if(c!='#')
						c='.';
		}

		void explore(){


			vector<pos> v;
			for(uint64_t i=0;i<c.size();i++)
				for(uint64_t j=0;j<c[i].size();j++)
					if(isalpha(get(j,i)))
						keys[get(j,i)]=pos(j,i);

			keys[START]=pos(x,y);
			createDependencies();

			for(auto& ch:keys){
				if(islower(ch.first)||ch.first==START)
				{
					__costs.clear();
					v.clear();
					v.push_back(ch.second);
					while(v.size())
					{
						pos p=v.back();
						v.pop_back();
						lookAround(p,v);
					}

					for(auto& ch2:keys){
						if(islower(ch2.first)||ch2.first==START)
							cost[ch.first][ch2.first]=__costs[ch2.second];
					}
				}
			}

			for(auto&v:getsUnlock)
				if(!unlock.count(v.first))
					endPoints.push_back(v.first);

			return;


		}


		void lookAround(pos p,vector<pos>&v){
			addPos(p.top(),__costs[p],v);
			addPos(p.bot(),__costs[p],v);
			addPos(p.left(),__costs[p],v);
			addPos(p.right(),__costs[p],v);
		}

		void addPos(pos p,int64_t cc,vector<pos> &v){

			char tile=get(p);
			if(tile!='#')
			{
				if(!__costs.count(p)||__costs[p]>cc)
				{
					__costs[p]=cc+1;
					if(tile=='.'||islower(tile))
						v.push_back(p);
				}
			}
		}

		void draw(){
			char cc=get(x,y);
			get(x,y)='$';
			uint64_t xx=0,yy=0;
			for(auto&v:c){
				for(auto& cc:v)
				{
					if(cc=='.'&&__costs.count(pos(xx,yy)))
					{

						cout<<" ";

					}
					else
						cout<<cc;
					xx++;
				}
				cout<<endl;
				xx=0;
				yy++;
			}
			get(x,y)=cc;
		}

		char& get(pos p){return get(p.first,p.second);}
		char& get(uint64_t x,uint64_t y){
			if(y>=c.size()||x>=c[y].size())
				return vd;
			return c[y][x];
		}

		map<char,map<char,int64_t>> cost;
		map<char,vector<char>> 	  unlock;
		map<char,vector<char>> 	  getsUnlock;
		int64_t x,y;
		std::vector<string> c;
		map<char,pos> keys;
		map<pos,int64_t> __costs;
		vector<char> endPoints;
		char vd='#';
	};

	world w;

	struct endPointWalk{

		endPointWalk(char pos,uint32_t cost){
			this->pos=pos;
			this->cost=cost;
			keys=0;
			addKey(pos);
		}

		endPointWalk(const endPointWalk& o, char c){
			this->pos=c;
			this->cost=w.cost[o.pos][c];
			this->keys=o.keys;
			addKey(c);
		}

		void addKey(char c){
			keys|=(1<<(c-START));
		}

		void removeKey(char c)
		{
			keys&=0xffffffff^(1<<(c-START));
		}

		bool hasKey(char c){
			return (1<<(c-START))&keys;
		}

		bool canMove(char c){
			for(auto&nc:w.getsUnlock[c])
				if(!hasKey(nc))
					return false;
			return true;
		}

		char pos;
		int32_t keys,cost;
	};

	void findKey(char c,std::vector<endPointWalk>& out){
	std::vector<endPointWalk>&res=out;
	stack<char> required;
	required.push(c);

	while(required.size())
	{
		if(res.back().hasKey(required.top()))
		{
			required.pop();
		}
		else if(res.back().canMove(required.top()))
		{
			res.emplace_back(res.back(),required.top());
			required.pop();
		}
		else
			for(auto&v:w.getsUnlock[required.top()])
				required.push(v);
	}

}

struct listVector:public std::vector<endPointWalk>
{
	listVector(std::vector<endPointWalk> v):std::vector<endPointWalk>(v){

	}

	listVector del(uint32_t index){
		std::vector<endPointWalk> copy;
		for(uint32_t i=0;i<size();i++)
			if(i!=index)
				copy.push_back((*this)[i]);

		return copy;
	}

	listVector ins(uint32_t index,endPointWalk& p){
		std::vector<endPointWalk> copy;
		for(uint32_t i=0;i<size();i++)
		{
			if(i==index)
					copy.push_back(p);

			copy.push_back((*this)[i]);
		}

		if(index>=size())
			copy.push_back(p);

		return copy;
	}

};

struct pathWalker{

	pathWalker(listVector p):path(p){

		recalculate();
	}

	void recalculate(){
		currentTotal=0;
		for(uint32_t i=1;i<path.size();i++)
		{
			path[i].cost=w.cost[path[i-1].pos][path[i].pos];
			currentTotal+=path[i].cost;
		}
	}

	bool consistent(){
		path[0].keys=0;
		path[0].addKey(path[0].pos);

		for(unsigned i=0;i<path.size()-1;i++)
		{
			if(!path[i].canMove(path[i+1].pos))
				return false;

			path[i+1].keys=path[i].keys;
			path[i+1].addKey(path[i+1].pos);
		}

		return true;
	}


	bool moveBack(){
		for(uint32_t i=path.size();i!=1;)
		{
			i--;
			int64_t validj=-1;
			uint64_t best=currentTotal;
			for(uint32_t j=i-1;j!=0;j--)
			{
				listVector cpy=path.del(i).ins(j,path[i]);
				pathWalker c(cpy);
				if(c.consistent()&&c.currentTotal<best)
				{
					validj=j;
					best=c.currentTotal;
				}

			}

			if(validj!=-1)
			{
				*this=pathWalker(path.del(i).ins(validj,path[i]));
				return true;
			}
		}

		return false;
	}

	bool moveFront(){
		for(uint32_t i=1;i<path.size()-1;i++)
		{
			int64_t validj=-1;
			uint64_t best=currentTotal;
			for(uint32_t j=i+1;j<path.size();j++)
			{
				listVector cpy=path.del(i).ins(j,path[i]);
				pathWalker c(cpy);
				if(c.consistent()&&c.currentTotal<best)
				{
					validj=j;
					best=c.currentTotal;
				}

			}

			if(validj!=-1)
			{
				*this=pathWalker(path.del(i).ins(validj,path[i]));
				return true;
			}
		}

		return false;
	}

	uint64_t currentTotal;
	listVector path;
};

	struct day18{

		day18(const string& filename){
			ifstream i(filename);
			w=world(i);
			wacopy=w;
		}

		int32_t explore(){
			endPointWalk starting('`',0);

			uint32_t best=numeric_limits<uint32_t>::max();

			for(unsigned j=0;j<w.endPoints.size();j++)
			{
				vector<endPointWalk> path;
				path.push_back(starting);
				for(unsigned i=0;i<w.endPoints.size();i++)
				{
					findKey(w.endPoints[(i+j)%w.endPoints.size()],path);
				}
				pathWalker w(path);
				while(w.moveBack()||w.moveFront());
				if(w.currentTotal<best)
				{
					best=w.currentTotal;

				}
			}

			return best;
		}

		void getResult(){
			w.explore();
			cout<<"Best bigmaze:"<<explore()<<endl;
			w=wacopy;
		}

		void getResult2(){
			uint32_t res[4];
			world::pos p1=world::pos(w.x,w.y).left().top();
			w.get(p1.bot())='#';
			w.get(p1.right())='#';
			w.get(p1.bot().right())='#';
			w.x=p1.first;
			w.y=p1.second;
			for(uint32_t i=0;i<w.c.size();i++)
				for(uint32_t j=0;j<w.c[i].size();j++)
				{
					if(i>p1.second||j>p1.first)
						w.get(j,i)='#';
				}
			for(char c='a';c<='z';c++)
			{
				auto p=w.find(c);
				auto p2=w.find(toupper(c));

				if(!p.good()&&p2.good())
					w.get(p2)='.';


			}
			w.explore();
			res[0]=explore();
			w=wacopy;

			world::pos p2=world::pos(w.x,w.y).right().top();
			w.get(p2.bot())='#';
			w.get(p2.left())='#';
			w.get(p2.bot().left())='#';
			w.x=p2.first;
			w.y=p2.second;
			for(uint32_t i=0;i<w.c.size();i++)
				for(uint32_t j=0;j<w.c[i].size();j++)
				{
					if(i>p2.second||j<p2.first)
						w.get(j,i)='#';
				}
			for(char c='a';c<='z';c++)
			{
				auto p=w.find(c);
				auto p2=w.find(toupper(c));

				if(!p.good()&&p2.good())
					w.get(p2)='.';
			}
			//w.draw();
			w.explore();
			res[1]=explore();
			w=wacopy;

			world::pos p3=world::pos(w.x,w.y).left().bot();
			w.get(p3.right())='#';
			w.get(p3.top())='#';
			w.get(p3.top().right())='#';
			w.x=p3.first;
			w.y=p3.second;
			for(uint32_t i=0;i<w.c.size();i++)
				for(uint32_t j=0;j<w.c[i].size();j++)
				{
					if(i<p3.second||j>p3.first)
						w.get(j,i)='#';
				}
			for(char c='a';c<='z';c++)
			{
				auto p=w.find(c);
				auto p2=w.find(toupper(c));

				if(!p.good()&&p2.good())
					w.get(p2)='.';
			}
			w.explore();
			res[2]=explore();
			w=wacopy;


			world::pos p4=world::pos(w.x,w.y).right().bot();
			w.get(p4.left())='#';
			w.get(p4.top())='#';
			w.get(p4.top().left())='#';
			w.x=p4.first;
			w.y=p4.second;
			for(uint32_t i=0;i<w.c.size();i++)
				for(uint32_t j=0;j<w.c[i].size();j++)
				{
					if(i<p4.second||j<p4.first)
						w.get(j,i)='#';
				}
			for(char c='a';c<='z';c++)
			{
				auto p=w.find(c);
				auto p2=w.find(toupper(c));

				if(!p.good()&&p2.good())
					w.get(p2)='.';
			}
			w.explore();
			res[3]=explore();
			cout<<"Small mazes:"<<(res[0]+res[1]+res[2]+res[3])<<endl;
		}

		world wacopy;
};
}

#endif
