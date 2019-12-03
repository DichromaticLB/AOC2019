#ifndef INCLUDE_DAY3_HPP_
#define INCLUDE_DAY3_HPP_

#include<iostream>
#include<string>
#include<cstdint>
#include<fstream>
#include<vector>
#include<limits>
using namespace std;

struct day3{

	struct path{
		path(int64_t X,int64_t Y,int64_t moveX,int64_t moveY)
		{
			hasBeenXOriented=false;
			hasBeenYOriented=false;
			fromX=X;
			fromY=Y;
			toX=X+moveX;
			toY=Y+moveY;
		}

		int64_t length() const{
			return abs(fromX-toX)+abs(fromY-toY);
		}

		operator string(){
			return "X="+to_string(fromX)+"->"+to_string(toX)+" Y="+to_string(fromY)+"->"+to_string(toY);
		}

		void orient(){//always from - to +
			if(horizontalMovement()){
				if(fromX>toX)
				{
					toX^=fromX;
					fromX^=toX;
					toX^=fromX;
					hasBeenXOriented=true;
				}
			}
			else{
				if(fromY>toY)
				{
					toY^=fromY;
					fromY^=toY;
					toY^=fromY;
					hasBeenYOriented=true;
				}
			}
		}

		bool horizontalMovement() const{
			return fromX!=toX;
		}

		int64_t crossIntersectionDistance(const path& other)const{

			if(!(horizontalMovement()^other.horizontalMovement()))
				return numeric_limits<int64_t>::max();

			if(!horizontalMovement())
				return other.crossIntersectionDistance(*this);

			if(other.fromX<=fromX||other.fromX>=toX)
				return numeric_limits<int64_t>::max();

			if(fromY<=other.fromY||fromY>=other.toY)
				return numeric_limits<int64_t>::max();

			return std::abs(fromY)+std::abs(other.fromX);

		}

		int64_t fromX,toX,fromY,toY;
		bool hasBeenXOriented,hasBeenYOriented;
	};

	std::vector<path> parsePath(ifstream& i)
	{
		std::vector<path> result;
		int64_t x=0,y=0,val;

		char dir;
		while(!i.eof()&&!i.fail()&&i.peek()!='\n')
		{
			i>>dir;
			i>>val;
			switch(dir)
			{
				case 'L':
					result.emplace_back(x,y,-val,0);
					break;
				case 'R':
					result.emplace_back(x,y,val,0);
					break;
				case 'D':
					result.emplace_back(x,y,0,-val);
					break;
				case 'U':
					result.emplace_back(x,y,0,val);
					break;
			}
			x=result.back().toX;
			y=result.back().toY;
			result.back().orient();

			if (i.peek() == ',')
					i.ignore();
		}

		return result;
	}

	day3(const string& filename){

		ifstream in(filename);
		path1=parsePath(in);
		in.ignore();//eat newline
		path2=parsePath(in);
	}

	void getResult() const{
		int64_t closest=numeric_limits<int64_t>::max();
		path a(0,0,0,0),b(0,0,0,0);

		for(auto& p1:path1)
			for(auto&p2:path2)
			{
				int64_t crossDist=p1.crossIntersectionDistance(p2);
				if(crossDist<closest)
				{
					closest=crossDist;
					a=p1;
					b=p2;
				}

			}

		if(!a.horizontalMovement())
		{
			path c=a;
			a=b;
			b=c;
		}

		cout<<static_cast<string>(a)<<endl<<static_cast<string>(b)<<endl<<"With a distance of "<<closest<<endl
				<<"intersection at X:"<<b.fromX<<" Y:"<<a.fromY<<endl;

	}

	void getResult2() const{
		int64_t shortest=numeric_limits<int64_t>::max();
		path a(0,0,0,0),b(0,0,0,0);
		int64_t acumA=0;
		int64_t acumB=0;

		for(auto& p1:path1)
		{
			acumB=0;
			for(auto&p2:path2)
			{
				int64_t crossDist=p1.crossIntersectionDistance(p2);
				if(crossDist!=numeric_limits<int64_t>::max())
				{
					if(p1.horizontalMovement())
					{
						a=p1;
						b=p2;
					}
					else
					{
						a=p2;
						b=p1;
					}

					int64_t contender=acumA+acumB;

					if(a.hasBeenXOriented)
						contender+=abs(b.fromX-a.toX);
					else
						contender+=abs(b.fromX-a.fromX);

					if(b.hasBeenYOriented)
						contender+=abs(a.fromY-b.toY);
					else
						contender+=abs(b.fromY-b.fromY);

					if(contender<shortest)
						shortest=contender;
				}

				acumB+=p2.length();

			}
			acumA+=p1.length();
		}

		cout<<"The shortest distance stands at a length of "<<shortest<<" units"<<endl;

	}

	vector<path> path1;
	vector<path> path2;

};



#endif
