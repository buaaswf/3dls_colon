#include "vol_math_Raw3D_Independt.h"
#include "vol_math_RawImage.h"
#include<iostream>
#include <queue>
#include <vector>
#include <queue>
#include"statistics.h"
using namespace std;
/*
1\delete the tree 
2\compute the length of the center line
3\divide the region 
*/
struct Point
{
	long long x;
	long long y;
	long long z;
	PIXTYPE value;
	Point(long long x,long long y,long long z,PIXTYPE value)
	{
		this->x=x;
		this->y=y;
		this->z =z;
		this->value=value;
	}
	Point ()
	{
	
	}
};
PIXTYPE  biglittleedianv2(PIXTYPE  val)
{

	PIXTYPE *q=&val;
	unsigned char * p= (unsigned char*) q;
	std::swap(p[0],p[3]);
	std::swap(p[1],p[2]);
	return val;
}
//vector <Point> SeedGrow(Point seed)
//{
//
//}
//deserve for graph storage structure
typedef struct edge
{
	int adjvex;
	int weight;
	struct edge *next;

}ELink;
typedef struct ver
{
	Point vertex;
	ELink *link;
}VLink  ;
class DivideRegion
{

public:
	queue<Point> q;
	Raw *skeletondata;
	vector<Point> center;
	DivideRegion(queue<Point> q,Raw *s,vector<Point> center)
	{
		this->q=q;
		this->skeletondata=new Raw(*s);
		this->center=center;
	}
void checkadj(Point firstseed)
{
	for (int l = -1; l <=1 ;l++)
	{
		for (int m =-1; m<= 1; m++)
		{
			for(int n=-1; n<= 1;n++)
			{
				int posx = firstseed.x+l;
				int posy = firstseed.y+m;
				int posz = firstseed.z+n;
				if (skeletondata->get(posx,posy,posz)!=0)
				{
					//firstseed.value=0;
					Point pos(posx,posy,posz,0);
					skeletondata->put(posx,posy,posz,0);
					center.push_back(pos);
					q.push(pos);
				}


			}
		}
	}
}
//the BFS for store the skeleton in order add a queue store different vertexes
void  putskletoninorder()
{
	//int i=0;
	//vector <Point> center;
	int i,j,k;
	Point firstseed;
	//queue<Point> q;
	for ( i =0;i< skeletondata->getXsize();i++)
	{
		for ( j=0;j<skeletondata->getYsize();j++)
		{
			for ( k=0;k<skeletondata->getZsize();k++)
			{
				//int count=0;
				if (skeletondata->get(i,j,k)!=0)
				{
					//count++;
					//if (count ==1)
					//{
						firstseed.x=(long long)i;
						firstseed.y=(long long)j;
						firstseed.z=(long long)k;
						firstseed.value=0;
						break;
					//}
					

				}
			}
		}
	}
	center.push_back(firstseed);
	checkadj(firstseed);
	while(!q.empty())
	{
		Point pos=q.front();
		checkadj(pos);
		q.pop();
	}

}
//compute the HU statistics
void DivideRegionv1(Raw *colonskeleton,Raw *data)
{
	//skeletondata=colonskeleton;
	int count=0;
	vector<Point> skeleton;
	for (int i = 0; i < colonskeleton->getXsize(); i++)
	{
		for (int j=0 ; j < colonskeleton->getYsize() ; j++)
		{
			for (int k=0; k<colonskeleton->getZsize();k++)
			{
				//big --little edian
				PIXTYPE val =biglittleedianv2(colonskeleton->get(i,j,k));
				if (val!= 0)
				{
					Point point(i,j,k,255);
					skeleton.push_back(point);
					count++;
				}
			}
		}
		

	}
	short histgram[4][3000]={0};
	for (int i = 1; i < skeleton.size(); i++)
	{
		int box=10;
		if (i <= count/4 &&i >=1)
		{
			Point pos=skeleton[i];

			Point prepos=skeleton[i-1];


			for (int l=pos.x-box; l< pos.x+box ;l++)
			{
				for (int m= pos.y-box; m< pos.y+box ; m++)
				{
					for (int n= pos.z-box ; n< pos.z+box; n++)
					{
						if (data->get(l,m,n)!=0)
						{
							bool flag=(l-pos.x)*(pos.x-prepos.x)+(m-pos.y)*(pos.y-prepos.y)*(pos.z-prepos.z)==0;
							if (flag)
							{
								histgram[0][(int)data->get(l,m,n)]++;
							}
						}
					}
				}
			}

		}
		else if (i<= 2*count/4)
		{
			Point pos=skeleton[i];
			Point prepos=skeleton[i-1];
			for (int l=pos.x-box; l< pos.x+box ;l++)
			{
				for (int m= pos.y-box; m< pos.y+box ; m++)
				{
					for (int n= pos.z-box ; n< pos.z+box; n++)
					{
						if (data->get(l,m,n)!=0)
						{
							bool flag=(l-pos.x)*(pos.x-prepos.x)+(m-pos.y)*(pos.y-prepos.y)*(pos.z-prepos.z)==0;
							if (flag)
							{
								histgram[1][(int)data->get(l,m,n)]++;
							}
						}
					}
				}
			}
		} 
		else if (i<= 3*count/4)
		{
			Point pos=skeleton[i];
			Point prepos=skeleton[i-1];
			for (int l=pos.x-box; l< pos.x+box ;l++)
			{
				for (int m= pos.y-box; m< pos.y+box ; m++)
				{
					for (int n= pos.z-box ; n< pos.z+box; n++)
					{
						if (data->get(l,m,n)!=0)
						{
							bool flag=(l-pos.x)*(pos.x-prepos.x)+(m-pos.y)*(pos.y-prepos.y)*(pos.z-prepos.z)==0;
							if (flag)
							{
								histgram[2][(int)data->get(l,m,n)]++;
							}
						}
					}
				}
			}
		} 
		else
		{
			Point pos=skeleton[i];
			Point prepos=skeleton[i-1];
			for (int l=pos.x-box; l< pos.x+box ;l++)
			{
				for (int m= pos.y-box; m< pos.y+box ; m++)
				{
					for (int n= pos.z-box ; n< pos.z+box; n++)
					{
						if (data->get(l,m,n)!=0)
						{
							bool flag=(l-pos.x)*(pos.x-prepos.x)+(m-pos.y)*(pos.y-prepos.y)*(pos.z-prepos.z)==0;
							if (flag)
							{
								histgram[3][(int)data->get(l,m,n)]++;
							}
						}
					}
				}
			}
		}



	}
	for (int i=0;i<4;i++)
	{
		//for (int j = 0; j < 255; j++)
		//{
		ofstream os("histgram.txt",ios::app);
		if (os)
		{
			for (int j=0; j <1000; j++)
			{
				os << histgram[i][j] <<" ";

			}
			os << endl;
		} 
			/*FILE *p=fopen("histgram.txt","a+");
			fwrite((void *)histgram[i],sizeof(short),3000,p);*/
		//}
	}
	
}
void DivideRegionv2(Raw *colonskeleton,Raw *data)
{
	//skeletondata=colonskeleton;
	int count=center.size();
	//vector<Point> skeleton;
	//for (int i = 0; i < colonskeleton->getXsize(); i++)
	//{
	//	for (int j=0 ; j < colonskeleton->getYsize() ; j++)
	//	{
	//		for (int k=0; k<colonskeleton->getZsize();k++)
	//		{
	//			//big --little edian
	//			PIXTYPE val =biglittleedianv2(colonskeleton->get(i,j,k));
	//			if (val!= 0)
	//			{
	//				Point point(i,j,k,0);
	//				center.push_back(point);
	//			
	//				//count++;
	//			}
	//		}
	//	}
	//	

	//}
	short histgram[4][3000]={0};
	for (int i = 1; i < center.size(); i++)
	{
		int box=10;
		if (i <= count/4 &&i >=1)
		{
			Point pos=center[i];

			Point prepos=center[i-1];


			for (int l=pos.x-box; l< pos.x+box ;l++)
			{
				for (int m= pos.y-box; m< pos.y+box ; m++)
				{
					for (int n= pos.z-box ; n< pos.z+box; n++)
					{
						if (data->get(l,m,n)!=0)
						{
							bool flag=(l-pos.x)*(pos.x-prepos.x)+(m-pos.y)*(pos.y-prepos.y)*(pos.z-prepos.z)==0;
							if (flag)
							{
								histgram[0][(int)data->get(l,m,n)]++;
							}
						}
					}
				}
			}

		}
		else if (i<= 2*count/4)
		{
			Point pos=center[i];
			Point prepos=center[i-1];
			for (int l=pos.x-box; l< pos.x+box ;l++)
			{
				for (int m= pos.y-box; m< pos.y+box ; m++)
				{
					for (int n= pos.z-box ; n< pos.z+box; n++)
					{
						if (data->get(l,m,n)!=0)
						{
							bool flag=(l-pos.x)*(pos.x-prepos.x)+(m-pos.y)*(pos.y-prepos.y)*(pos.z-prepos.z)==0;
							if (flag)
							{
								histgram[1][(int)data->get(l,m,n)]++;
							}
						}
					}
				}
			}
		} 
		else if (i<= 3*count/4)
		{
			Point pos=center[i];
			Point prepos=center[i-1];
			for (int l=pos.x-box; l< pos.x+box ;l++)
			{
				for (int m= pos.y-box; m< pos.y+box ; m++)
				{
					for (int n= pos.z-box ; n< pos.z+box; n++)
					{
						if (data->get(l,m,n)!=0)
						{
							bool flag=(l-pos.x)*(pos.x-prepos.x)+(m-pos.y)*(pos.y-prepos.y)*(pos.z-prepos.z)==0;
							if (flag)
							{
								histgram[2][(int)data->get(l,m,n)]++;
							}
						}
					}
				}
			}
		} 
		else
		{
			Point pos=center[i];
			Point prepos=center[i-1];
			for (int l=pos.x-box; l< pos.x+box ;l++)
			{
				for (int m= pos.y-box; m< pos.y+box ; m++)
				{
					for (int n= pos.z-box ; n< pos.z+box; n++)
					{
						if (data->get(l,m,n)!=0)
						{
							bool flag=(l-pos.x)*(pos.x-prepos.x)+(m-pos.y)*(pos.y-prepos.y)*(pos.z-prepos.z)==0;
							if (flag)
							{
								histgram[3][(int)data->get(l,m,n)]++;
							}
						}
					}
				}
			}
		}



	}
	for (int i=0;i<4;i++)
	{
		//for (int j = 0; j < 255; j++)
		//{
		ofstream os("histgram.txt",ios::app);
		if (os)
		{
			for (int j=0; j <1000; j++)
			{
				os << histgram[i][j] <<" ";

			}
			os << endl;
		} 
			/*FILE *p=fopen("histgram.txt","a+");
			fwrite((void *)histgram[i],sizeof(short),3000,p);*/
		//}
	}
	
}
void ComputeLength()
{

}
void  HistHU(Raw *thickness,Raw *origion,Raw *hu)
{

	for (int i=0;i<thickness->size();i++)
	{
		if (thickness->getXYZ(i)!=0)
		{
			hu->putXYZ(i,origion->getXYZ(i));
		}
	}

}
void DivideRegionthickness(Raw *colonskeleton,Raw *data)
{
	int count=0;

	vector<Point> skeleton;
	for (int i = 0; i < colonskeleton->getXsize(); i++)
	{
		for (int j=0 ; j < colonskeleton->getYsize() ; j++)
		{
			for (int k=0; k<colonskeleton->getZsize();k++)
			{
				if (colonskeleton->get(i,j,k)!= 0)
				{
					Point point(i,j,k,255);
					skeleton.push_back(point);
					count++;
				}
			}
		}


	}
	int histgram[4][20]={0};
	for (int i = 1; i < skeleton.size(); i++)
	{
		int box=10;
		if (i <= count/4)
		{
			Point pos=skeleton[i];
			Point prepos=skeleton[i-1];
			for (int l=pos.x-box; l< pos.x+box ;l++)
			{
				for (int m= pos.y-box; m< pos.y+box ; m++)
				{
					for (int n= pos.z-box ; n< pos.z+box; n++)
					{
						PIXTYPE val = data->get(l,m,n);
						if (val !=0)
						{
							bool flag=(l-pos.x)*(pos.x-prepos.x)+(m-pos.y)*(pos.y-prepos.y)*(pos.z-prepos.z)==0;
							if (flag)
							{
								histgram[0][(int)val]++;
							}
						}
					}
				}
			}

		}
		else if (i<= 2*count/4)
		{
			Point pos=skeleton[i];
			Point prepos=skeleton[i-1];
			for (int l=pos.x-box; l< pos.x+box ;l++)
			{
				for (int m= pos.y-box; m< pos.y+box ; m++)
				{
					for (int n= pos.z-box ; n< pos.z+box; n++)
					{
						if (data->get(l,m,n)!=0)
						{
							bool flag=(l-pos.x)*(pos.x-prepos.x)+(m-pos.y)*(pos.y-prepos.y)*(pos.z-prepos.z)==0;
							if (flag)
							{
								histgram[1][(int)data->get(l,m,n)]++;
							}
						}
					}
				}
			}
		} 
		else if (i<= 3*count/4)
		{
			Point pos=skeleton[i];
			Point prepos=skeleton[i-1];
			for (int l=pos.x-box; l< pos.x+box ;l++)
			{
				for (int m= pos.y-box; m< pos.y+box ; m++)
				{
					for (int n= pos.z-box ; n< pos.z+box; n++)
					{
						if (data->get(l,m,n)!=0)
						{
							bool flag=(l-pos.x)*(pos.x-prepos.x)+(m-pos.y)*(pos.y-prepos.y)*(pos.z-prepos.z)==0;
							if (flag)
							{
								histgram[2][(int)data->get(l,m,n)]++;
							}
						}
					}
				}
			}
		} 
		else
		{
			Point pos=skeleton[i];
			Point prepos=skeleton[i-1];
			for (int l=pos.x-box; l< pos.x+box ;l++)
			{
				for (int m= pos.y-box; m< pos.y+box ; m++)
				{
					for (int n= pos.z-box ; n< pos.z+box; n++)
					{
						if (data->get(l,m,n)!=0)
						{
							bool flag=(l-pos.x)*(pos.x-prepos.x)+(m-pos.y)*(pos.y-prepos.y)*(pos.z-prepos.z)==0;
							if (flag)
							{
								histgram[3][(int)data->get(l,m,n)]++;
							}
						}
					}
				}
			}
		}



	}
	for (int i = 0; i < 4; i++)
	{
		//for (int j = 0; j < 255; j++)
		//{
		ofstream os("thicknesshistgram.txt",ios::app);
		for (int j=0;j<20;j++)
		{
			os<<histgram[i][j]<<" ";
		}
		os <<endl;
		/*FILE *p=fopen("thicknesshistgram.txt","a+");
		fwrite((void *)histgram[i],sizeof(int),20,p);*/
		//}
		//1\thickness 4
		// 2
		//\thickness new3 = num3-2*num4
		// \thickness new2 = num2-2*num4-2*new3
		// \thickneww new1= num1-2*new2-new3
		
	}

}
void DivideRegionthicknessv2(Raw *colonskeleton,Raw *data)
{
	//skeletondata=colonskeleton;
	int count=center.size();
	int histgram[4][20]={0};
	for (int i = 1; i < center.size(); i++)
	{
		int box=10;
		if (i <= count/4)
		{
			Point pos=center[i];
			Point prepos=center[i-1];
			for (int l=pos.x-box; l< pos.x+box ;l++)
			{
				for (int m= pos.y-box; m< pos.y+box ; m++)
				{
					for (int n= pos.z-box ; n< pos.z+box; n++)
					{
						PIXTYPE val = data->get(l,m,n);
						if (val !=0)
						{
							bool flag=(l-pos.x)*(pos.x-prepos.x)+(m-pos.y)*(pos.y-prepos.y)*(pos.z-prepos.z)==0;
							if (flag)
							{
								histgram[0][(int)val]++;
							}
						}
					}
				}
			}

		}
		else if (i<= 2*count/4)
		{
			Point pos=center[i];
			Point prepos=center[i-1];
			for (int l=pos.x-box; l< pos.x+box ;l++)
			{
				for (int m= pos.y-box; m< pos.y+box ; m++)
				{
					for (int n= pos.z-box ; n< pos.z+box; n++)
					{
						if (data->get(l,m,n)!=0)
						{
							bool flag=(l-pos.x)*(pos.x-prepos.x)+(m-pos.y)*(pos.y-prepos.y)*(pos.z-prepos.z)==0;
							if (flag)
							{
								histgram[1][(int)data->get(l,m,n)]++;
							}
						}
					}
				}
			}
		} 
		else if (i<= 3*count/4)
		{
			Point pos=center[i];
			Point prepos=center[i-1];
			for (int l=pos.x-box; l< pos.x+box ;l++)
			{
				for (int m= pos.y-box; m< pos.y+box ; m++)
				{
					for (int n= pos.z-box ; n< pos.z+box; n++)
					{
						if (data->get(l,m,n)!=0)
						{
							bool flag=(l-pos.x)*(pos.x-prepos.x)+(m-pos.y)*(pos.y-prepos.y)*(pos.z-prepos.z)==0;
							if (flag)
							{
								histgram[2][(int)data->get(l,m,n)]++;
							}
						}
					}
				}
			}
		} 
		else
		{
			Point pos=center[i];
			Point prepos=center[i-1];
			for (int l=pos.x-box; l< pos.x+box ;l++)
			{
				for (int m= pos.y-box; m< pos.y+box ; m++)
				{
					for (int n= pos.z-box ; n< pos.z+box; n++)
					{
						if (data->get(l,m,n)!=0)
						{
							bool flag=(l-pos.x)*(pos.x-prepos.x)+(m-pos.y)*(pos.y-prepos.y)*(pos.z-prepos.z)==0;
							if (flag)
							{
								histgram[3][(int)data->get(l,m,n)]++;
							}
						}
					}
				}
			}
		}



	}
	for (int i = 0; i < 4; i++)
	{
		//for (int j = 0; j < 255; j++)
		//{
		ofstream os("thicknesshistgram.txt",ios::app);
		for (int j=0;j<20;j++)
		{
			os<<histgram[i][j]<<" ";
		}
		os <<endl;
		/*FILE *p=fopen("thicknesshistgram.txt","a+");
		fwrite((void *)histgram[i],sizeof(int),20,p);*/
		//}
		//1\thickness 4
		// 2
		//\thickness new3 = num3-2*num4
		// \thickness new2 = num2-2*num4-2*new3
		// \thickneww new1= num1-2*new2-new3
		
	}

}
};