#ifndef INCLUDE_INTCODEDIS_HPP_
#define INCLUDE_INTCODEDIS_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
#include<map>
#include<queue>
#include<memory>

struct computer{

		computer(std::vector<int64_t> ins){
			instructions=ins;
			IP=0;
			COUT=0;
			RELATIVE_BASE=0;
			done=false;
			MAX=ins.size();
			}


		uint64_t disassemble(){
			while(IP<MAX)
			{
				cout<<"["<<std::to_string(IP)<<"]";
				int64_t op=instructions[IP]%100;
				int64_t mode1=(instructions[IP]/100)%10;
				int64_t mode2=(instructions[IP]/1000)%10;
				int64_t mode3=(instructions[IP]/10000)%10;
				if(instructionIncrements().count(op)==0)
				{
					IP++;
					cout<<"Bad ins inc+1"<<endl;
					continue;
				}
				uint32_t ipInc=instructionIncrements().at(op);

				switch(op)
				{
					case 99:
						cout<<"HALT"<<endl;
						done=true;
						break;
					case 1:
						cout<<"ADD"<<decodeAccessDebug(mode3,IP+3)<<"<="<<decodeAccessDebug(mode1,IP+1)<<","<<decodeAccessDebug(mode2,IP+2)<<endl;
						break;
					case 2:
						cout<<"MULT"<<decodeAccessDebug(mode3,IP+3)<<"<="<<decodeAccessDebug(mode1,IP+1)<<","<<decodeAccessDebug(mode2,IP+2)<<endl;
						break;
					case 3:
						cout<<"READ"<<decodeAccessDebug(mode1,IP+1)<<endl;
						break;
					case 4:
						cout<<"WRIT"<<decodeAccessDebug(mode1,IP+1)<<endl;
						break;
					case 5:
						cout<<"JNOZ "<<decodeAccessDebug(mode1,IP+1)<<","<<decodeAccessDebug(mode2,IP+2)<<endl;
						break;
					case 6:
						cout<<"JZ "<<decodeAccessDebug(mode1,IP+1)<<","<<decodeAccessDebug(mode2,IP+2)<<endl;
						break;
					case 7:
						cout<<"STLT"<<decodeAccessDebug(mode3,IP+3)<<","<<decodeAccessDebug(mode1,IP+1)<<"<"<<decodeAccessDebug(mode2,IP+2)<<endl;
						break;
					case 8:
						cout<<"STEQ"<<decodeAccessDebug(mode3,IP+3)<<","<<decodeAccessDebug(mode1,IP+1)<<"=="<<decodeAccessDebug(mode2,IP+2)<<endl;
						break;
					case 9:
						cout<<"REL "<<decodeAccessDebug(mode1,IP+1)<<endl;
						break;
					default:
						throw ("Illegal instruction "+std::to_string(instructions[IP])+" at index "+std::to_string(IP));
				}
				IP+=ipInc;
			}

			return 1;
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

		std::string decodeAccessDebug(int8_t mode,int64_t index)
		{
			if(index>=(int64_t)instructions.size())
				instructions.resize(instructions.size()*8,0);

			if(mode==1)
			{
				return "I"+std::to_string(instructions[index]);

			}
			else if(mode==2)
			{
				if(instructions[index]+RELATIVE_BASE>=(int64_t)instructions.size())
						instructions.resize(instructions.size()*8,0);
				return "(R+"+std::to_string(instructions[index])+")";
			}
			else
			{
				while(instructions[index]>=(int64_t)instructions.size()||instructions[instructions[index]]>(int64_t)instructions.size())
						instructions.resize(instructions.size()*8,0);
				return "("+std::to_string(instructions[index])+")";
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
		}

		const std::map<uint8_t,uint8_t> instructionIncrements(){
			static std::map<uint8_t,uint8_t> ii={{1,4},{2,4},{3,2},{4,2},{5,3},{6,3},{7,4},{8,4},{9,2},{99,1}};
			return ii;
		}

		unique_ptr<computer> COUT;
		bool done;
		uint64_t IP,MAX;
		int64_t RELATIVE_BASE;
		queue<int64_t> IN,OUT;
		vector<int64_t> instructions;


	};


#endif /* INCLUDE_INTCODEDIS_HPP_ */
