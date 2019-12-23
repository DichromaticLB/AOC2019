#ifndef INCLUDE_DAY23_HPP_
#define INCLUDE_DAY23_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
#include<map>
#include<queue>
#include<memory>

using namespace std;
struct {int64_t x,y;}NAT;

struct day23{

	struct computer{

			computer(std::vector<int64_t> ins){
				instructions=ins;
				IP=0;
				RELATIVE_BASE=0;
				done=false;
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
							{
								IP+=ipInc;
								return 0;
							}

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
					//lim--;
					//if(!lim)
					//	return true;
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
				if(!IN.size())
				{
					tgt=-1;
					return false;
				}
				tgt=IN.front();
				IN.pop();
				return true;
			}

			void write(int64_t out){
				netBuffer.emplace_back(out);
				sent++;
				if(netBuffer.size()==3)
				{
					if(netBuffer[0]==255)
					{
						NAT.x=netBuffer[1];
						NAT.y=netBuffer[2];
						netBuffer.clear();
					}
					else
					{
						net[netBuffer[0]].IN.emplace(netBuffer[1]);
						net[netBuffer[0]].IN.emplace(netBuffer[2]);

						netBuffer.clear();
					}
				}

				OUT.emplace(out);

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
			vector<int64_t> netBuffer;
			static vector<computer> net;
			static uint32_t sent;
		};

	day23(const string& filename){
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

		for(uint64_t i=0;i<50;i++)
		{
			computer::net.emplace_back(instructions);
			computer::net.back().IN.emplace(i);
		}
	}

	void getResult(){
		auto back=computer::net;
		NAT.x=-1;
		NAT.y=-1;
		while(NAT.x==-1&&NAT.y==-1)
			for(uint64_t i=0;i<50;i++)
				computer::net[i].executeProgram();
		cout<<NAT.y<<endl;
		computer::net=back;


	}

	void getResult2(){
		int64_t prev=0xffffffffffffffff;
		NAT.x=-1;
		NAT.y=-1;
		while(true)
		{
			computer::sent=0;
			for(uint64_t i=0;i<50;i++)
				computer::net[i].executeProgram();

			if(computer::sent==0)
			{
				if(NAT.y==prev&&prev!=-1)
				{
					cout<<"REPEATED "<<NAT.y<<endl;
					return;
				}

				computer::net[0].IN.emplace(NAT.x);
				computer::net[0].IN.emplace(NAT.y);
				prev=NAT.y;

			}
		}
	}

	std::vector<int64_t> instructions;

};
vector<day23::computer> day23::computer::net;
uint32_t day23::computer::sent;
#endif
