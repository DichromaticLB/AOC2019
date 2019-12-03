#ifndef INCLUDE_DAY2_HPP_
#define INCLUDE_DAY2_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
using namespace std;

struct day2{

	day2(const string& filename){

		ifstream in(filename);
		uint64_t input;
		in>>input;

		while(!in.fail())
		{
			if (in.peek() == ',')
					in.ignore();

			instructions.push_back(input);
			in>>input;

		}
	}

	uint64_t executeProgram(uint64_t noun,uint64_t verb){
		vector<uint64_t> instructions(this->instructions);
		instructions[1]=noun;
		instructions[2]=verb;

		uint64_t index=0;
		while(index<=instructions.size()-4)
		{

			if(instructions[index+1]>instructions.size()||instructions[index+2]>instructions.size()||instructions[index+3]>instructions.size())
				return 0;

			switch(instructions[index])
			{
				case 99:
					return instructions[0];
				case 1:
					instructions[instructions[index+3]]=instructions[instructions[index+1]]+instructions[instructions[index+2]];
					break;
				case 2:
					instructions[instructions[index+3]]=instructions[instructions[index+1]]*instructions[instructions[index+2]];
					break;
				default:
					return 0;
			}
			index+=4;
		}

		return 0;
	}

	void getResult(){
		cout<<"Value at pos 0="<<executeProgram(12,2)<<endl;
	}

	void getResult2(uint64_t target){
		for(uint64_t noun=0;noun<instructions.size();noun++)
			for(uint64_t verb=0;verb<instructions.size();verb++)
				if(executeProgram(noun,verb)==target)
				{
					cout<<"To generate the value "<<target<<" use noun:"<<noun<<" verb: "<<verb<<" result->"<<(100*noun+verb)<<endl;
					return;
				}
	}


	vector<uint64_t> instructions;
};



#endif
