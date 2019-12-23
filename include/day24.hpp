#ifndef INCLUDE_DAY24_HPP_
#define INCLUDE_DAY24_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
#include<map>
#include<queue>
#include<memory>

using namespace std;


struct day24{

	struct bugLand{

		bugLand(){
			id=0;
			buffer=0;
			above=0;
			below=0;
		}

		bool isBug(uint32_t xx,uint32_t yy){
			if(below&&xx==2&&yy==2)
				return 0;
			return id&(1<<(xx+yy*x));
		}

		void expand(){
			bugLand *a=this;
			while(a->above)
				a=a->above;
			a->above=new bugLand();
			a->above->below=a;
			a=this;
			while(a->below)
				a=a->below;
			a->below=new bugLand();
			a->below->above=a;

		}

		uint32_t around(uint32_t xx,uint32_t yy){
			uint32_t  res=0;

			if(xx>0&&isBug(xx-1,yy))
				res++;

			if(above&&xx==0)
				res+=above->isBug(1,2);

			if(xx<x-1&&isBug(xx+1,yy))
				res++;

			if(above&&xx==4)
				res+=above->isBug(3,2);

			if(yy>0&&isBug(xx,yy-1))
				res++;

			if(above&&yy==0)
				res+=above->isBug(2,1);

			if(yy<y-1&&isBug(xx,yy+1))
				res++;

			if(above&&yy==4)
				res+=above->isBug(2,3);

			if(below&&xx==2&&yy==1)
				res+=below->isBug(0,0)+below->isBug(1,0)
						+below->isBug(2,0)+below->isBug(3,0)+below->isBug(4,0);

			if(below&&xx==3&&yy==2)
				res+=below->isBug(4,0)+below->isBug(4,1)
						+below->isBug(4,2)+below->isBug(4,3)+below->isBug(4,4);

			if(below&&xx==1&&yy==2)
				res+=below->isBug(0,0)+below->isBug(0,1)
						+below->isBug(0,2)+below->isBug(0,3)+below->isBug(0,4);

			if(below&&xx==2&&yy==3)
				res+=below->isBug(0,4)+below->isBug(1,4)
						+below->isBug(2,4)+below->isBug(3,4)+below->isBug(4,4);

			return res;
		}

		bugLand* top(){
			if(above)
				return above->top();
			return this;
		}

		bugLand* bot(){
			if(below)
				return below->top();
			return this;
		}

		void update(){
			buffer=0;
			for(uint32_t yy=0;yy<y;yy++)
					for(uint32_t xx=0;xx<x;xx++)
					{
						auto ret=around(xx,yy);
						if(isBug(xx,yy)&&ret==1)
							buffer+=1<<(xx+yy*x);
						else if(!isBug(xx,yy)&&(ret==1||ret==2))
							buffer+=1<<(xx+yy*x);

					}
		}

		uint32_t count(){
			uint32_t res=0;
			for(uint32_t yy=0;yy<y;yy++)
				for(uint32_t xx=0;xx<x;xx++)
					if(isBug(xx,yy))
						res++;
			return res;
		}


		void updateAll(){
			update();
			auto t=this;
			while(t->above)
			{
				t->above->update();
				t=t->above;
			}
			t=this;
			while(t->below)
			{
				t->below->update();
				t=t->below;
			}
		}

		void draw(){
			for(uint32_t yy=0;yy<y;yy++)
			{
					for(uint32_t xx=0;xx<x;xx++)
						if(isBug(xx,yy))
							cout<<'#';
						else
							cout<<'.';
					cout<<endl;
			}
		}
		void swapUp(){
			if(above)
			{
				above->id=above->buffer;
				above->swapUp();
			}
		}
		void swapDown(){
			if(below)
			{
				below->id=below->buffer;
				below->swapDown();
			}
		}
		void swap(){
			id=buffer;
			swapUp();
			swapDown();
		}

		uint32_t id,buffer;
		bugLand *above,*below;
		static const uint32_t x=5,y=5;
	};

	day24(const string& filename){
		ifstream in(filename);
		char input;
		in>>input;
		initialState=0;
		uint32_t index=0;
		while(in.good())
		{
			if(input=='#'||input=='.')
			{
				if(input=='#')
					initialState+=1<<index;
				index++;
			}
			in>>input;
		}

	}

	void getResult(){
		bugLand b;
		set<uint32_t> ct;
		b.id=initialState;
		while(!ct.count(b.id))
		{
			ct.emplace(b.id);
			b.update();
			b.swap();
		}
		cout<<"Repeated ratting "<<b.id<<endl;
	}

	void getResult2(){
		bugLand c;
		c.id=initialState;
		for(uint32_t i=0;i<200;i++){
			c.expand();
			c.updateAll();
			c.swap();
		}
		uint32_t res=0;
		auto t=c.top();
		while(t){
			res+=t->count();
			t=t->below;
		}
		cout<<"Recursive Bugland count 200 min:"<<res<<endl;
	}

	uint32_t initialState;

};
#endif
