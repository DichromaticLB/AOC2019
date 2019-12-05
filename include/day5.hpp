#ifndef INCLUDE_DAY5_HPP_
#define INCLUDE_DAY5_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
using namespace std;

struct day5{

	day5(const string& filename){
		IDEX=0;
		ODEX=0;
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

	uint64_t executeProgram(){
		vector<int64_t> instructions(this->instructions);

		uint64_t index=0;

		while(index<=instructions.size()-4)
		{

			int64_t op=instructions[index]%100;
			int64_t mode1=(instructions[index]/100)%10;
			int64_t mode2=(instructions[index]/1000)%10;
			int64_t param1,param2;
			uint32_t ipInc=4;

			switch(op)
			{
				case 99:
					return instructions[0];
				case 1:
					if(mode1)
							param1=instructions[index+1];
						else
							param1=instructions[instructions[index+1]];
					if(mode2)
							param2=instructions[index+2];
						else
							param2=instructions[instructions[index+2]];
					instructions[instructions[index+3]]=param1+param2;
					break;
				case 2:
					if(mode1)
							param1=instructions[index+1];
					else
							param1=instructions[instructions[index+1]];
					if(mode2)
							param2=instructions[index+2];
						else
							param2=instructions[instructions[index+2]];
					instructions[instructions[index+3]]=param1*param2;
					break;
				case 3:
					instructions[instructions[index+1]]=read();
					ipInc=2;
					break;
				case 4:
					if(mode1)
						param1=instructions[index+1];
					else
						param1=instructions[instructions[index+1]];
					std::cout<<"OUTPUT: "<<param1<<endl;
					ipInc=2;
					break;
				case 5:
					if(mode1)
							param1=instructions[index+1];
					else
							param1=instructions[instructions[index+1]];
					if(mode2)
							param2=instructions[index+2];
						else
							param2=instructions[instructions[index+2]];
					if(param1)
					{
						index=param2;
						ipInc=0;
					}
					else
						ipInc=3;
					break;
				case 6:
					if(mode1)
							param1=instructions[index+1];
					else
							param1=instructions[instructions[index+1]];
					if(mode2)
							param2=instructions[index+2];
						else
							param2=instructions[instructions[index+2]];
					if(!param1)
					{
						index=param2;
						ipInc=0;
					}
					else
						ipInc=3;
					break;
				case 7:
					if(mode1)
							param1=instructions[index+1];
					else
							param1=instructions[instructions[index+1]];
					if(mode2)
							param2=instructions[index+2];
						else
							param2=instructions[instructions[index+2]];

					instructions[instructions[index+3]]=(param1<param2)?1:0;

					break;
				case 8:
					if(mode1)
							param1=instructions[index+1];
					else
							param1=instructions[instructions[index+1]];
					if(mode2)
							param2=instructions[index+2];
						else
							param2=instructions[instructions[index+2]];

					instructions[instructions[index+3]]=(param1==param2)?1:0;

					break;
				default:
					return 0;
			}
			index+=ipInc;
		}

		return 0;
	}

	uint64_t read(){
		return IN[IDEX++];
	}


	void getResult(){
		IN.push_back(1);
		executeProgram();
	}

	void getResult2(){
		IN.push_back(5);
		executeProgram();

	}

	uint64_t IDEX,ODEX;
	vector<int64_t> IN;
	vector<int64_t> instructions;
};



#endif
