#ifndef INCLUDE_DAY19_HPP_
#define INCLUDE_DAY19_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
#include<map>
#include<queue>
#include<memory>
#include<stack>

using namespace std;

	struct day19{

	using pos=pair<uint32_t,uint32_t>;
	struct computer{

			computer(std::vector<int64_t> ins,map<pos,char> &t):tractor(t){
				instructions=ins;
				IP=0;
				COUT=0;
				RELATIVE_BASE=0;
				done=false;
				gox=true;
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
				if(gox)
				{
					IN.emplace(x);
					gox=false;

				}
				else
				{
					IN.emplace(y);
					gox=true;

				}

				if(IN.size())
				{
					tgt=IN.front();
					IN.pop();
					return true;
				}
				return false;
			}

			void write(int64_t out){


				if(out)
				{

					tractor[pos(x,y)]=out;
				}

			}

			void request(uint32_t xx,uint32_t yy)
			{
				IP=0;
				computer c(instructions,tractor);
				c.x=xx;
				c.y=yy;
				c.executeProgram();

			}

			bool check(uint32_t xx,uint32_t yy){
				request(xx,yy);
				return tractor[pos(xx,yy)]!=0;
			}

			pos findClampX(uint32_t y, double slope){
				pos res;
				uint32_t x=round(y/slope);
				uint32_t lx=0,rx=x;

				while(rx-lx>1)
				{
					auto ch=((rx-lx)/2)+lx;

					if(check(ch,y))
						rx=ch;
					else
						lx=ch;
				}

				res.first=rx;

				lx=x,rx=x*4;

				while(rx-lx>1)
				{
					auto ch=((rx-lx)/2)+lx;

					if(check(ch,y))
						lx=ch;
					else
						rx=ch;
				}

				res.second=lx;


				res.second++;
				return res;

			}

			pos findClampY(uint32_t x, double slope){
				pos res;
				uint32_t y=std::round(x*slope);
				uint32_t ly=0,ry=y;

				while(ry-ly>1)
				{
					auto ch=((ry-ly)/2)+ly;

					if(check(x,ch))
						ry=ch;
					else
						ly=ch;
				}
				res.first=ry;

				ly=y,ry=y*4;

				while(ry-ly>1)
				{
					auto ch=((ry-ly)/2)+ly;

					if(check(x,ch))
						ly=ch;
					else
						ry=ch;
				}

				res.second=ly;
				res.second++;
				return res;

			}

			bool squareCheck(uint32_t y,double slope,uint32_t req=100)
			{
				auto xc=findClampX(y,slope);
				if(xc.second-xc.first<req)
					return false;

				auto yc=findClampY(xc.second-1,slope);
				if(yc.second-yc.first<req)
					return false;

				auto yc2=findClampY(xc.second-req,slope);
					if(yc2.second-yc.first<req)
						return false;

				return true;
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

			map<pos,char> &tractor;
			bool gox;
			int32_t x=0,y=0;
		};

		day19(const string& filename){
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
			computer c(instructions,tractor);
			for(uint32_t i=0;i<50;i++)
				for(uint32_t j=0;j<50;j++)
					c.request(j,i);
			int count=0;
			for(auto&v:c.tractor)
				if(v.second)
					count++;
			cout<<"total beam tiles "<<count<<endl;
		}

		void getResult2(){
			uint32_t lx=400,rx=300;
			computer c(instructions,tractor);
			for(unsigned i=300;i<301;i++)
			{
				for(unsigned j=300;j<400;j++)
				{
					c.request(j,i);
					if(tractor[pos(j,i)])
					{
						if(j<lx)
							lx=j;
						if(j>rx)
							rx=j;
					}
				}
			}

			double slope=300.0f/(lx+(rx-lx)/2);

			uint32_t target=100;
			uint32_t y=800;
			while(true)
			{
				if(!c.squareCheck(y,slope,target))
					y++;
				else
					break;

			}
			auto t1=c.findClampX(y,slope);
			cout<<(t1.second-target)<<","<<y<<endl;
		}

		vector<int64_t> instructions;
		map<pos,char> tractor;
};

#endif
