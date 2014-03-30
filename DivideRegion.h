#include "vol_math_Raw3D_Independt.h"
#include "vol_math_RawImage.h"
#include<iostream>
#include <queue>
#include <vector>
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
};
void DivideRegion(Raw *colonskeleton,Raw *data)
{
	int count=0;
	vector<Point> skeleton;
	for (int i = 0; i < colonskeleton->getXsize(); i++)
	{
		for (int j=0 ; j < colonskeleton->getYsize() ; j++)
		{
			for (int k=0; k<colonskeleton->getZsize();k++)
			{
				if (colonskeleton->get(i,j,k)== 255)
				{
					Point point(i,j,k,255);
					skeleton.push_back(point);
					count++;
				}
			}
		}
		

	}
	int histgram[4][255]={0};
	for (int i = 0; i < skeleton.size(); i++)
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
			FILE *p=fopen("histgram.txt","w");
			fwrite((void *)histgram[i][255],sizeof(int),255,p);
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
void DivideRegionthickness(Raw *colonskeleton,Raw *thickness)
{
	int count=0;
	vector<Point> skeleton;
	for (int i = 0; i < colonskeleton->getXsize(); i++)
	{
		for (int j=0 ; j < colonskeleton->getYsize() ; j++)
		{
			for (int k=0; k<colonskeleton->getZsize();k++)
			{
				if (colonskeleton->get(i,j,k)== 255)
				{
					Point point(i,j,k,255);
					skeleton.push_back(point);
					count++;
				}
			}
		}


	}
	int histgram[4][20]={0};
	for (int i = 0; i < skeleton.size(); i++)
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
	for (int i = 0; i < 4; i++)
	{
		//for (int j = 0; j < 255; j++)
		//{
		FILE *p=fopen("thicknesshistgram.txt","w");
		fwrite((void *)histgram[i][20],sizeof(int),20,p);
		//}
		//1\thickness 4
		// 2
		//\thickness new3 = num3-2*num4
		// \thickness new2 = num2-2*num4-2*new3
		// \thickneww new1= num1-2*new2-new3
		
	}

}