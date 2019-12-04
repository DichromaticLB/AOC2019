#ifndef INCLUDE_DAY4_HPP_
#define INCLUDE_DAY4_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
#include<limits>
using namespace std;

struct day4{

	day4(uint32_t left,uint32_t right){
		r1=0;
		r2=0;

		for(uint32_t i=left;i<=right;i++)
		{
			uint32_t k=i;
			uint32_t k1=k%10;
			k/=10;
			bool increasingCheck=true;
			while(k)
			{
				if(k%10>k1)
				{
					increasingCheck=false;
					break;
				}
				k1=k%10;
				k/=10;
			}


			if(increasingCheck&&(digitMatch1(i,1)||digitMatch1(i,10)||digitMatch1(i,100)
					||digitMatch1(i,1000)||digitMatch1(i,10000)||digitMatch1(i,100000)))
					r1++;

			if(increasingCheck&&(digitMatch2(i,1)||digitMatch2(i,10)||digitMatch2(i,100)
					||digitMatch2(i,1000)||digitMatch2(i,10000)||digitMatch2(i,100000)))
					r2++;

		}
	}

	bool digitMatch2(uint32_t a,uint32_t dex)
	{
		bool res=(a/dex)%10==(a/(dex*10))%10;
		if((a/(dex*10))>9)
		{
			uint32_t next=(a/(dex*100))%10;
			uint32_t prev=(a/(dex*10))%10;

			if(next==prev)
				res=false;
		}

		if(dex!=1)
		{
			uint32_t next=(a/(dex/10))%10;
			uint32_t prev=(a/(dex))%10;
			if(next==prev)
					res=false;
		}
		return res;
	}

	bool digitMatch1(uint32_t a,uint32_t dex)
	{
		return (a/dex)%10==(a/(dex*10))%10;
	}

	void getResult(){
		cout<<"First criteria matching:"<<r1<<endl;
	}

	void getResult2(){
		cout<<"Second criteria matching:"<<r2<<endl;
	}

	uint32_t r1,r2;

};



#endif
