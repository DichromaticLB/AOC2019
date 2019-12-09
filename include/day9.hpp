#ifndef INCLUDE_DAY9_HPP_
#define INCLUDE_DAY9_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
#include<map>
#include<queue>
#include<memory>

using namespace std;

struct day9{

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
			if(IN.size())
			{
				tgt=IN.front();
				IN.pop();
				return true;
			}
			return false;
		}

		void write(int64_t out){
			if(COUT)
				COUT->IN.emplace(out);

			OUT.emplace(out);
			std::cout<<out<<endl;
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

	day9(const string& filename){
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
		computer c(instructions);
		c.IN.emplace(1);
		c.executeProgram();

	}

	void getResult2(){
		computer c(instructions);
		c.IN.emplace(2);
		c.executeProgram();
	}

	std::vector<int64_t> instructions;

};
#endif
