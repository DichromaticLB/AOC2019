#ifndef INCLUDE_DAY21_HPP_
#define INCLUDE_DAY21_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
#include<map>
#include<queue>
#include<memory>
#include<stack>
#include"day9.hpp"

using namespace std;

	struct day21{

	using pos=pair<uint32_t,uint32_t>;


		day21(const string& filename){
			ins=day9(filename).instructions;
		}

		void getResult(){
			day9::computer copy(ins);
			string program;
			program+="NOT C T\n";
			program+="OR  T J\n";
			program+="NOT B T\n";
			program+="OR  T J\n";
			program+="NOT A T\n";
			program+="OR  T J\n";
			program+="AND D J\n";
			program+="WALK\n";
			for(auto&c:program)
				copy.IN.push(c);
			copy.executeProgram();
		}

		void getResult2(){
			day9::computer copy(ins);
			string program;
			program+="NOT C T\n";
			program+="OR  T J\n";
			program+="NOT B T\n";
			program+="OR  T J\n";
			program+="NOT A T\n";
			program+="OR  T J\n";
			program+="AND D J\n";
			program+="NOT E T\n";
			program+="NOT T T\n";
			program+="OR  H T\n";
			program+="AND T J\n";
			program+="RUN\n";
			for(auto&c:program)
				copy.IN.push(c);
			copy.executeProgram();
		}
		vector<int64_t> ins;
};

#endif
