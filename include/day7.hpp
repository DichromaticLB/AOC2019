#ifndef INCLUDE_DAY7_HPP_
#define INCLUDE_DAY7_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
#include<map>
using namespace std;


struct day7{

	struct computer{

		computer(std::vector<int64_t> ins){
			IDEX=0;
			ODEX=0;
			instructions=ins;
			IP=0;
			COUT=0;
			done=false;
			}


		uint64_t executeProgram(){
			uint64_t &index=IP;
			while(index<=instructions.size())
			{

				int64_t op=instructions[index]%100;
				int64_t mode1=(instructions[index]/100)%10;
				int64_t mode2=(instructions[index]/1000)%10;
				int64_t param1,param2;
				uint32_t ipInc=4;

				switch(op)
				{
					case 99:
						done=true;
						return 1;
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
						if(read(instructions[instructions[index+1]]))
							ipInc=2;
						else
						{
							return 0;
						}

						break;
					case 4:
						if(mode1)
							param1=instructions[index+1];
						else
							param1=instructions[instructions[index+1]];

						write(param1);
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

		bool read(int64_t& tgt){
			if(IDEX<IN.size())
			{
				tgt=IN[IDEX++];
				return true;
			}
			return false;
		}

		void write(uint64_t out){
			if(COUT)
				COUT->IN.push_back(out);

			return OUT.push_back(out);
		}

		computer* COUT;
		bool done;
		uint64_t IDEX,ODEX,IP;
		vector<int64_t> IN,OUT;
		vector<int64_t> instructions;

	};


	day7(const string& filename){
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

	int64_t test(int64_t i1,int64_t i2,int64_t i3,int64_t i4,int64_t i5){
		std::vector<computer> c(5,instructions);
		for(unsigned i=0;i<c.size()-1;i++)
			c[i].COUT=&c[i+1];
		c[0].IN.push_back(i1);
		c[0].IN.push_back(0);
		c[1].IN.push_back(i2);
		c[2].IN.push_back(i3);
		c[3].IN.push_back(i4);
		c[4].IN.push_back(i5);
		bool done=false;
		while(!done)
		{
			done=true;
			for(auto& comp:c)
				if(!comp.done)
					done=done&&comp.executeProgram();
		}

		return c[4].OUT[0];
	}

	int64_t test2(int64_t i1,int64_t i2,int64_t i3,int64_t i4,int64_t i5){
		std::vector<computer> c(5,instructions);
		for(unsigned i=0;i<c.size()-1;i++)
			c[i].COUT=&c[i+1];
		c[4].COUT=&c[0];

		c[0].IN.push_back(i1);
		c[0].IN.push_back(0);
		c[1].IN.push_back(i2);
		c[2].IN.push_back(i3);
		c[3].IN.push_back(i4);
		c[4].IN.push_back(i5);
		bool done=false;
		while(!done)
		{
			done=true;
			for(auto& comp:c)
				if(!comp.done)
					done=comp.executeProgram()&&done;
		}

		return c[4].OUT.back();
	}

#define test1(v) if(c[v])c[v]=false;else continue;
	void getResult(){
		int64_t max=0;
		for(unsigned i=0;i<5;i++)
			for(unsigned i2=0;i2<5;i2++)
				for(unsigned i3=0;i3<5;i3++)
					for(unsigned i4=0;i4<5;i4++)
						for(unsigned i5=0;i5<5;i5++)
						{
							std::vector<bool> c(5,true);
							test1(i);
							test1(i2);
							test1(i3);
							test1(i4);
							test1(i5);

							auto res=test(i,i2,i3,i4,i5);
							if(res>max)
								max=res;
						}
		cout<<max<<endl;
	}

	void getResult2(){
		int64_t max=0;
			for(unsigned i=0;i<5;i++)
				for(unsigned i2=0;i2<5;i2++)
					for(unsigned i3=0;i3<5;i3++)
						for(unsigned i4=0;i4<5;i4++)
							for(unsigned i5=0;i5<5;i5++)
							{
								std::vector<bool> c(5,true);
								test1(i);
								test1(i2);
								test1(i3);
								test1(i4);
								test1(i5);

								auto res=test2(i+5,i2+5,i3+5,i4+5,i5+5);
								if(res>max)
									max=res;
							}
			cout<<max<<endl;
		}


	vector<int64_t> instructions;

};
#endif
