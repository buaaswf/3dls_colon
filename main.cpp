#include "vol_math_LevelSet.h"
#include "statistics.h"
#include "test.h"
//#include <iostream>
//#include <crtdbg.h> 
//#include "CImg.h" 
//#include "ThreeDim_LevelSet.h"
//#include "Filter.h"
//#include "WipeNioisePde.h"
//#include "string.h"
////================
//#include <fstream>
//#include <vector>
//#include <string>
//#ifdef _WIN32
//#include <Windows.h>
//#include <strsafe.h>
//#else
//#include <dirent.h>
//#endif
//#define output "D:\\sdfdata\\" 
//#define input1  "D:\\sdfdata\\pvaluethickness\\" //"K:\\sdf\\volume\\clean\\clean\\ep\\""E:\\volume\\cleantestdata\\test2\\"
//#define input2  "K:\\sdf\\volume\\clean\\clean\\ep\\test3\\" 
//using namespace cimg_library;
//using namespace std;


void testcolon(int argc,string dir)
{
	
	char *pt="single_well";
	int l=0,m=0,n=0,l1=0,l2=0,iter_outer=10;
	RawImage test;
	char dirhead[200]=input2;  //K:\\sdf\\volume\\clean\\clean\\ep\\

	char dirbody[100];
	strcpy(dirbody,dir.c_str());
	cout <<"dirbody"<<dirbody<<endl;
	strcat(dirhead,dirbody);
	cout << "dirhead" << dirhead<< endl;
	short * indata=test.readStream(dirhead,&l,&m,&n);
	Raw *initial=new Raw(l,m,n);
	float *inputo=new float[l*m*n];
	for (int i = 0; i < l*m*n; i++)
	{
		inputo[i]=(float) indata[i];		
	}

	Raw *input=new Raw(l,m,n,inputo);
	Filter *f=new Filter();
	//input=f->guass3DFilter(input,3);
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
	char *outname1="inner5-8_2.raw";
	char outdir[200]=output;
	strcat(outdir,dirbody);
	strcat(outdir,outname1);
	test.writeImageName(*initial,outdir);
	//Raw temp(*initial);
	ls->outerwall(*initial,*input,5.0,0.1,-+3,1.5,1,10,pt);
	//*initial -=temp;
	char *outname2="outer5-8_2.raw";
	char outdir2[200]=output;
	strcat(outdir2,dirbody);
	strcat(outdir2,outname2);
	test.writeImageName(*initial,outdir2);
	evaluate(dir,l,m,n);
}


int main(int argc,char **argv)
{
	string dir2(input2);
	vector<string> files2;
	GetFileNameFromDir(dir2,files2);
	vector<string>::iterator iterFile2;
	for (iterFile2=files2.begin();iterFile2!=files2.end();iterFile2++)
	{

		
		iterFile2->assign(iterFile2->substr(dir2.size()+1));
		cout<<*iterFile2 <<endl;
		//ddcircle(*iterFile);
		testcolon(argc,*iterFile2);
		//float2uchar(512,512,700,*iterFile2);
		iterFile2++;
	}

	cout<<endl;
	
	system("pause");
	return 0;
}
