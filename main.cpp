#include "vol_math_LevelSet.h"

#include "test.h"
#include <iostream>
#include <crtdbg.h> 
#include "CImg.h" 
#include "ThreeDim_LevelSet.h"
#include "Filter.h"
#include "WipeNioisePde.h"

using namespace cimg_library;
using namespace std;

void testcolon(int argc,char **argv)
{
	
	char *pt="single_well";
	int l=0,m=0,n=0,l1=0,l2=0,iter_outer=10;
	RawImage test;
	short * indata=test.readStream("K:\\sdf\\volume\\clean\\WI_3110_S_iso_clean.raw",&l,&m,&n);
	Raw *initial=new Raw(l,m,n);
	float *inputo=new float[l*m*n];
	for (int i = 0; i < l*m*n; i++)
	{
		inputo[i]=(float) indata[i];		
	}

	Raw *input=new Raw(l,m,n,inputo);
	Filter *f=new Filter();
	input=f->guass3DFilter(input,3);
	RawImage *write=new RawImage();
	ThreeDim_LevelSet *ls=new ThreeDim_LevelSet();
	ls->initialg(*input);
	for (int i=0; i<input->getXsize(); i++)
	{
		for (int j=0; j<input->getYsize(); j++)
		{
			for (int k=0; k<input->getZsize(); k++)
			{
				if (input->get(i,j,k)>=1)
				{
					initial->put(i,j,k,-2);
				}
				else initial->put(i,j,k,2);

			}
		}

	}
	*initial=ls->minimal_surface(*initial,*input,5.0,0.1,-3,1.5,1,iter_outer,pt);
	Raw temp(*initial);
	ls->outerwallauto(*initial,*input,5.0,0.1,-3,1.5,1,20,pt);
	//*initial -=temp;
	//for (int i = 0; i < l * m * n; i++)
	//{
	//	//int count=0;
	//	//PIXTYPE * a=initial->getdata();
	//	//if (a[i]!=0)
	//	//{
	//	//	//cout << a[i] <<endl;
	//	//	
	//	//} 
	//	//else
	//	//{
	//	//}
	//}
	test.writeImage(*initial);


}

void evaluate()
{
	int l=512,m=512,n=570;
	RawImage test;
	float * indata1=test.readStreamfloat("K:\\3Dlevelcolonouterwall3119pphysiceqution.raw",&l,&m,&n);
	float * indata2=test.readStreamfloat("K:\\3Dlevelcolon3119p.raw",&l,&m,&n);
	for (int i = 0; i < l*m*n; i++)
	{
		indata1[i] -= indata2[i];
	}
	FILE *p;
	p=fopen("K:\\3Dlevelcolon3119thickness.raw","wb");
	fwrite(indata1,sizeof(PIXTYPE),l*m*n,p);
	fclose(p);
	fflush(stdout);
	delete [] indata1;
	delete [] indata2;

}
int main(int argc,char **argv)
{
	testcolon(argc,argv);
	//evaluate();
	system("pause");
	return 0;
}
