#ifndef INCLUDE_DAY11_HPP_
#define INCLUDE_DAY11_HPP_

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


struct day11{

	struct robot{

		union place{
			struct{
				int x,y;
			} xy;
			uint64_t sign;
		};

		robot(std::vector<int64_t> ins){
			instructions=ins;
			IP=0;
			RELATIVE_BASE=0;
			done=false;
			m={'T','R','B','L'};
			}


		uint64_t executeProgram(){
			while(IP<=instructions.size())
			{
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
			place c;
			c.xy.x=X;
			c.xy.y=Y;
			IN.emplace(color[c.sign]);
			if(IN.size())
			{
				tgt=IN.front();
				IN.pop();
				return true;
			}
			return false;
		}

		void write(int64_t out){
			if(iter%2)
			{
				if(out==0)
					direction--;
					else
					direction++;

				switch(m[direction%4]){
					case 'T':
						Y++;
						break;
					case 'L':
						X--;
						break;
					case 'B':
						Y--;
						break;
					case 'R':
						X++;
						break;
				}
			}
			else
			{
				place c;
				c.xy.x=X;
				c.xy.y=Y;
				color[c.sign]=out;
			}
			iter++;
		}

		const std::map<uint8_t,uint8_t> instructionIncrements(){
		static std::map<uint8_t,uint8_t> ii={{1,4},{2,4},{3,2},{4,2},{5,3},{6,3},{7,4},{8,4},{9,2},{99,0}};
		return ii;
		}


		bool done;
		uint64_t IP;
		int64_t RELATIVE_BASE;
		queue<int64_t> IN,OUT;
		vector<int64_t> instructions;

		std::map<uint64_t,int64_t> color;
		unsigned direction=400000;
		vector<char> m;
		int64_t X=0,Y=0,iter=0;


	};

	day11(const string& filename){
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


	void getResult1()
	{
		robot c(instructions);
		c.color[0]=0;
		c.executeProgram();
		cout<<"A total of "<<c.color.size()<<" tiles were painted"<<endl;
	}

	void getResult2()
	{
		robot cc(instructions);
		cc.color[0]=1;
		cc.executeProgram();
		int64_t l=100,b=-100,t=100,r=-100;
		for(auto&c:cc.color)
		{
			robot::place cc;
			cc.sign=c.first;
			if(cc.xy.x<l)
				l=cc.xy.x;
			if(cc.xy.x>r)
				r=cc.xy.x;
			if(cc.xy.y<t)
				t=cc.xy.y;
			if(cc.xy.y>b)
				b=cc.xy.y;
		}
		cout<<endl;
		for(int64_t i=b+5;i>t-5;i--){


			for(int64_t j=l-5;j<r+5;j++)
			{
				robot::place pl;
				pl.xy.x=j;
				pl.xy.y=i;
				if(cc.color.count(pl.sign))
					std::cout<<(cc.color[pl.sign]?"#":" ");
				else
					std::cout<<'#';

			}
			std::cout<<endl;
		}


	}
	std::vector<int64_t> instructions;

};

#endif
