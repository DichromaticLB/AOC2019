#ifndef INCLUDE_DAY15_HPP_
#define INCLUDE_DAY15_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
#include<map>
#include<queue>
#include<memory>

using namespace std;

struct day15{

	static const int64_t TOP = 1;
	static const int64_t BOT = 2;
	static const int64_t LEFT= 3;
	static const int64_t RIGHT=4;

	struct pos{
		int32_t x,y;

		pos left(){return pos(x-1,y);}
		pos right(){return pos(x+1,y);}
		pos top(){return pos(x,y-1);}
		pos bot(){return pos(x,y+1);}

		int64_t transition(const pos&other){
			if(x==other.x)
			{
				if(y<other.y)
					return BOT;

				return TOP;
			}
			else
			{
				if(x<other.x)
					return RIGHT;

				return LEFT;
			}
		}

		pos(uint32_t x,uint32_t y){
			this->x=x;
			this->y=y;
		}

		bool operator<(const pos&other)const{
			if(x==other.x)
				return y<other.y;
			return x<other.x;
		}
	};

	struct node{
		node():p(0,0){
			explored=false;
			wall=false;
			distance=0;
		}

		node& left(){
			if(!claimed.count(p.left()))
			{
				auto& n=claimed[p.left()];
				n.p=p.left();
				n.distance=distance+1;
			}

			return claimed[p.left()];
		}

		node& right(){
			if(!claimed.count(p.right()))
			{
				auto& n=claimed[p.right()];
				n.p=p.right();
				n.distance=distance+1;
			}

			return claimed[p.right()];
		}

		node& top(){
			if(!claimed.count(p.top()))
			{
				auto& n=claimed[p.top()];
				n.p=p.top();
				n.distance=distance+1;
			}

			return claimed[p.top()];
		}

		node& bot(){
			if(!claimed.count(p.bot()))
			{
				auto& n=claimed[p.bot()];
				n.p=p.bot();
				n.distance=distance+1;
			}

			return claimed[p.bot()];
		}

		node& explore(){
			if(!top().explored)
				return top();
			if(!bot().explored)
				return bot();
			if(!left().explored)
				return left();
			if(!right().explored)
				return right();

			return *this;
		}

		node& back(){
			uint32_t min=distance;
			node& res=*this;

			if(top().distance==min-1)
			{
				res=top();
			}

			if(bot().distance==min-1)
			{
				res=bot();
			}

			if(left().distance==min-1)
			{
				res=left();
			}

			if(right().distance==min-1)
			{
				res=right();
			}

			if(this==&res)
				throw("Bad step back");

			return res;
		}

		pos p;
		uint32_t distance;
		bool explored,wall;
		static map<pos,node> claimed;

	};

	struct computer{

			computer(std::vector<int64_t> ins){
				instructions=ins;
				IP=0;
				COUT=0;
				RELATIVE_BASE=0;
				done=false;
				}


			uint64_t executeProgram(){

				while(IP<=instructions.size())
				{
					if(done)
						return 0;

					int64_t op=instructions[IP]%100;
					int64_t mode1=(instructions[IP]/100)%10;
					int64_t mode2=(instructions[IP]/1000)%10;
					int64_t mode3=(instructions[IP]/10000)%10;
					uint32_t ipInc=instructionIncrements().at(op);

					switch(op)
					{
						case 99:
							done=true;
							return 1;
						case 1:
							decodeAccess(mode3,IP+3)=decodeAccess(mode1,IP+1)+decodeAccess(mode2,IP+2);
							break;
						case 2:
							decodeAccess(mode3,IP+3)=decodeAccess(mode1,IP+1)*decodeAccess(mode2,IP+2);
							break;
						case 3:
							if(!read(decodeAccess(mode1,IP+1)))
								return 0;
							break;
						case 4:
							write(decodeAccess(mode1,IP+1));
							ipInc=2;
							break;
						case 5:
							if(decodeAccess(mode1,IP+1))
							{
								IP=decodeAccess(mode2,IP+2);
								ipInc=0;
							}
							break;
						case 6:
							if(!decodeAccess(mode1,IP+1))
							{
								IP=decodeAccess(mode2,IP+2);
								ipInc=0;
							}
							break;
						case 7:
							decodeAccess(mode3,IP+3)=(decodeAccess(mode1,IP+1)<decodeAccess(mode2,IP+2))?1:0;
							break;
						case 8:
							decodeAccess(mode3,IP+3)=(decodeAccess(mode1,IP+1)==decodeAccess(mode2,IP+2))?1:0;
							break;
						case 9:
							RELATIVE_BASE+=decodeAccess(mode1,IP+1);
							break;
						default:
							throw ("Illegal instruction "+std::to_string(instructions[IP])+" at index "+std::to_string(IP));
					}
					IP+=ipInc;
				}

				throw ("Memory corruption program finished without halt");
			}

			int64_t& decodeAccess(int8_t mode,int64_t index)
			{
				if(index>=(int64_t)instructions.size())
					instructions.resize(instructions.size()*8,0);

				if(mode==1)
					return instructions[index];
				else if(mode==2)
				{
					if(instructions[index]+RELATIVE_BASE>=(int64_t)instructions.size())
						instructions.resize(instructions.size()*8,0);
					return instructions[instructions[index]+RELATIVE_BASE];
				}
				else
				{
					if(instructions[index]>=(int64_t)instructions.size())
							instructions.resize(instructions.size()*8,0);
					return instructions[instructions[index]];
				}
			}

			bool read(int64_t& tgt){
				auto current=*st.back();
				auto& next=current.explore();

				if(&current==&next){
					st.pop_back();
				}else
					st.push_back(&next);

				if(st.size()==0)
					return false;

				st.back()->explored=true;
				IN.emplace(current.p.transition(st.back()->p));
				if(IN.size())
				{
					tgt=IN.front();
					IN.pop();
					return true;
				}
				return false;
			}

			void write(int64_t out){
				if(out==0){
					if(!st.size())
					{
						throw("Failed on empty vector?");
					}

					st.back()->wall=true;
					st.pop_back();
				}
				else if(out==2){
					n=st.back();
				}

			}

			const std::map<uint8_t,uint8_t> instructionIncrements(){
				static std::map<uint8_t,uint8_t> ii={{1,4},{2,4},{3,2},{4,2},{5,3},{6,3},{7,4},{8,4},{9,2},{99,0}};
				return ii;
			}

			unique_ptr<computer> COUT;
			bool done;
			uint64_t IP;
			int64_t RELATIVE_BASE;
			queue<int64_t> IN,OUT;
			vector<int64_t> instructions;


		};


	day15(const string& filename){
		ifstream in(filename);
		int64_t input;
		in>>input;

		while(!in.fail())
		{
			if (in.peek() == ',')
					in.ignore();

			instructions.push_back(input);
			in>>input;
		}
	}

	void getResult(){
		st.push_back(&node::claimed[pos(0,0)]);
		st.back()->explored=true;
		computer e(instructions);
		e.executeProgram();
		cout<<"Distance to reach the target:"<<n->distance<<endl;
	}

	void getResult2(){

		std::set<pos> done;
		std::vector<pos> oxygen;
		unsigned iteration=0;
		oxygen.push_back(n->p);
		while(oxygen.size())
		{
			std::vector<pos> next;
			for(auto&n:oxygen)
			{
				std::vector<node*> neigh={
						&node::claimed[n].left(),
						&node::claimed[n].right(),
						&node::claimed[n].top(),
						&node::claimed[n].bot(),
				};
				done.insert(n);
				for(auto&p:neigh)
				{
					if(!p->wall&&!done.count(p->p))
					{
						next.push_back(p->p);
						done.insert(p->p);
					}
				}

			}
			oxygen=next;
			iteration++;
		}

		cout<<"Took "<<iteration-1<<" iterations for the O2 to expand";

	}
	std::vector<int64_t> instructions;
	static node *n;
	static std::vector<node*> st;
};
map<day15::pos,day15::node> day15::node::claimed={{day15::pos(0,0),day15::node()}};
day15::node *day15::n;
std::vector<day15::node*> day15::st;

#endif
