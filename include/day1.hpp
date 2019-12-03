#ifndef INCLUDE_DAY1_HPP_
#define INCLUDE_DAY1_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>

using namespace std;

struct day1{

	day1(const string& filename):result(0),fuel4fuel(0){

		ifstream in(filename);
		int64_t input;
		in>>input;
		while(!in.fail())
		{
			result+=input/3-2;
			fuel4fuel+=recurseFuel(input/3-2);
			in>>input;
		}

	}

	void getResult(){
		cout<<"To launch the modules a total of "<<result<<" fuel is required"<<endl;
	}

	int64_t recurseFuel(int64_t fuel){
		int64_t result=fuel/3-2;

		if(result<1)
			return 0;
		else
			return result+recurseFuel(result);
	}

	void getResult2(){
		cout<<"To launch the modules and the fuel itself a total of "<<(result+fuel4fuel)<<" fuel is required"<<endl;
	}

	int64_t result;
	int64_t fuel4fuel;
};



#endif
