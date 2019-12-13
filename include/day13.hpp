#ifndef INCLUDE_DAY13_HPP_
#define INCLUDE_DAY13_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
#include<map>
#include<queue>
#include<memory>

using namespace std;

struct day13{

	union pos{
		uint64_t sig;
		struct{
			int32_t x,y;
		}xy;
	};

	struct cabinet{

		cabinet(std::vector<int64_t> ins){
			instructions=ins;
			IP=0;
			RELATIVE_BASE=0;
			done=false;
			score=0;
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
			int64_t pad;
			int64_t ball;
			for(auto&v:screen)
			{
				if(v.second==3)
					pad=v.first;

				if(v.second==4)
					ball=v.first;
			}

			pos p,b;
			p.sig=pad;
			b.sig=ball;
			if(p.xy.x<b.xy.x)
				IN.emplace(1);
			else if (p.xy.x>b.xy.x)
				IN.emplace(-1);
			else
				IN.emplace(0);


			if(IN.size())
			{
				tgt=IN.front();
				IN.pop();
				return true;
			}
			return false;
		}

		void write(int64_t out){
			buffer.push_back(out);
			if(buffer.size()==3)
			{
				if(buffer[0]==-1&&buffer[1]==0)
					score=buffer[2];
				else
				{
					pos p;
					p.xy.x=buffer[0];
					p.xy.y=buffer[1];
					screen[p.sig]=buffer[2];
				}

				buffer.clear();
			}
		}

		const std::map<uint8_t,uint8_t> instructionIncrements(){
			static std::map<uint8_t,uint8_t> ii={{1,4},{2,4},{3,2},{4,2},{5,3},{6,3},{7,4},{8,4},{9,2},{99,0}};
			return ii;
		}

		void draw(){
			std::vector<std::vector<char>> dr;
			for(auto&v:screen)
			{
				pos p;
				p.sig=v.first;
				if((int64_t)dr.size()<=p.xy.y)
					dr.resize(p.xy.y+1);

				if((int64_t)dr[p.xy.y].size()<=p.xy.x)
					dr[p.xy.y].resize(p.xy.x+1,' ');

				dr[p.xy.y][p.xy.x]=v.second;
			}


			for(auto&v:dr)
			{
				for(auto&vv:v)
				{
					switch(vv){
					case 1:
						cout<<'|';
						break;
					case 2:
						cout<<'#';
						break;
					case 3:
						cout<<'=';
						break;
					case 4:
						cout<<'*';
						break;
					default:
						cout<<' ';
						break;
					}
				}
				cout<<endl;
			}

		}



		bool done;
		uint64_t IP;
		int64_t RELATIVE_BASE;
		queue<int64_t> IN,OUT;
		vector<int64_t> instructions;

		map<uint64_t,int64_t> screen;
		std::vector<int64_t> buffer;
		int64_t score;


	};

	day13(const string& filename){
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
		cabinet c(instructions);
		c.executeProgram();
		cout<<"Initial blocks:";
		cout<<std::accumulate(c.screen.begin(),c.screen.end(),0l,[](uint64_t b,auto&v){
			if(v.second==2)
				return b+1;
			return b;
		})<<endl;

	}

	void getResult2(){
		cabinet c(instructions);
		c.instructions[0]=2;
		c.executeProgram();
		cout<<"Final score:"<<c.score<<endl;

	}

	std::vector<int64_t> instructions;

};
#endif
