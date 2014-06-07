#include "vol_math_LevelSet.h"
#include "vol_math_Raw3D_Independt.h"
Raw * initialsimplycolon(int l,int m,int n)
{
	int R=50;
	int yR=50;
	Raw * data= new Raw(l,m,n);

	//memset(data->getdata(),0,l*m*n);
	int zR=115;
	int maxr=10;
	int outerthickness= 10;
	int distance=10;
	for (int i = 0 ; i< l; i++)
	{
		for (int j = 0; j < m; j++)
		{
			for (int k = 0; k < n; k++)
			{
				for (int r=0; r<maxr; r++)
				{
					if (r<5)
					{
						if ((i-sqrt((double)m*r/maxr)-l/4)*(i-sqrt((double)m*r/maxr)-l/4) +(j-m*r/maxr)*(j-m*r/maxr) + (k-n/2)*(k-n/2)  < R*R||
							(i-l*2/4-distance)*(i-l*2/4-distance) + (j-m*r/maxr)*(j-m*r/maxr) + (k-n/2)*(k-n/2)  < R*R)			//the || represents two colons
						{
							data->put(i,j,k,1000);
						}
				
						else if (data->get(i,j,k)!=2000&& data->get(i,j,k)!=1000&&data->get(i,j,k)!=0)
						{
							data->put(i,j,k,-1000);
						}

					}
					else 
					{
						//r-=5;
						if ((i+sqrt((double)m*(r-5)/maxr)-l/4)*(i+sqrt((double)m*(r-5)/maxr)-l/4) +(j-m*r/maxr)*(j-m*r/maxr) + (k-n/2)*(k-n/2)  < R*R||
							(i-l*2/4-distance)*(i-l*2/4-distance) + (j-m*r/maxr)*(j-m*r/maxr) + (k-n/2)*(k-n/2)  < R*R)
						{
							data->put(i,j,k,1000);
						}

						else if (data->get(i,j,k)!=2000&& data->get(i,j,k)!=1000&&data->get(i,j,k)!=0)
						{
							data->put(i,j,k,-1000);
						}
						//r+=5;
					}
				}






			}
		}
	}

		return data;
}