#include "vol_math_LevelSet.h"

#include "test.h"
#include <iostream>
#include <crtdbg.h> 
#include "CImg.h" 
#include "ThreeDim_LevelSet.h"
#include "Filter.h"
#include "WipeNioisePde.h"
#include "string.h"
//================
#include <fstream>
#include <vector>
#include <string>
#ifdef _WIN32
#include <Windows.h>
#include <strsafe.h>
#else
#include <dirent.h>
#endif
#define output "D:\\sdfdata\\" 
#define input1  "D:\\sdfdata\\pvaluethickness\\" //"K:\\sdf\\volume\\clean\\clean\\ep\\""E:\\volume\\cleantestdata\\test2\\"
#define input2  "K:\\sdf\\volume\\clean\\clean\\ep\\test3\\" 
using namespace cimg_library;
using namespace std;

//////////////////////////////////////////////////////////////////////////
//获取指定目录下所有文件的文件名，不包括文件夹，在GetFileFromDir中使用
//strDir: 输入，目录路径
//vFileDirList： 输出，文件路径列表
//返回：空
//////////////////////////////////////////////////////////////////////////
void GetFileNameFromDir(string strDir,vector<string>& vFileDirList)
{
#ifdef _WIN32
	WIN32_FIND_DATAA ffd;
	LARGE_INTEGER filesize;
	string szDir;
	//size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;

	szDir=strDir+"\\*";
	hFind = FindFirstFileA(szDir.c_str(), &ffd);

	if (INVALID_HANDLE_VALUE == hFind) 
	{
		cout<<"get file name error"<<endl;
		return;
	}
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			string filename=ffd.cFileName;//(const char*)
			string filedir=strDir+"\\"+filename;
			vFileDirList.push_back(filedir);
		}
	}while (FindNextFileA(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) 
	{
		cout<<"FindFirstFile error"<<endl;
		return;
	}
	FindClose(hFind);
#else
	DIR *dir;
	struct dirent *ptr;
	dir = opendir(strDir.c_str());
	while( (ptr = readdir(dir)) != NULL)
	{
		string path = strDir + string("/") + string(ptr->d_name);
		vFileDirList.push_back(path);
	}
	closedir(dir);
	sort(vFileDirList.begin(), vFileDirList.end());
#endif
}

void evaluate(string dir,int l ,int m,int n);
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
	char *outname1="inner5-8_circle.raw";
	char outdir[200]=output;
	strcat(outdir,dirbody);
	strcat(outdir,outname1);
	test.writeImageName(*initial,outdir);
	//Raw temp(*initial);
	ls->outerwall(*initial,*input,5.0,0.1,-3,1.5,1,10,pt);
	//*initial -=temp;
	char *outname2="outer5-8_circle.raw";
	char outdir2[200]=output;
	strcat(outdir2,dirbody);
	strcat(outdir2,outname2);
	test.writeImageName(*initial,outdir2);
	evaluate(dir,l,m,n);
}

void evaluate(string dir,int l,int m,int n)
{
	//int l=512,m=512,n=570;
	RawImage test;
	char dst[100];
	strcpy(dst,dir.c_str());
	char dir2[200]=output;
	strcat(dir2,dst);
	char dir3[300];
	strcpy(dir3,dir2);
	strcat(dir3,"outer5-8_circle.raw");
	float * indata1=test.readStreamfloat(dir3,&l,&m,&n);
	char dir4[300];
	strcpy(dir4,dir2);
	strcat(dir4,"inner5-8_circle.raw");
	float * indata2=test.readStreamfloat(dir4,&l,&m,&n);
	for ( int k = 0; k < n; k++ )
	{
		for ( int j = 0; j < m; j++)
		{
			for ( int i = 0; i < l; i++)
			{
				PIXTYPE *val=&indata1[k*m*l+j*l+i];
				//if(i<409 &&i> 107 && j>156 &&j <390)
				//{
				//	if (*val>1)
				//	{
				//		*val=0;

				//	}
				//	else *val=100;
				//}
				//else *val=0;
				*val -= indata2[ k*m*l + j*l + i];
			}
		}
	}
	//for (int i = 0; i < l*m*n; i++)
	//{
	//	indata1[i] -= indata2[i];
	//}
	FILE *p;
	char dir5[300];
	strcpy(dir5,dir2);
	strcat(dir5,"thickness5-8_circle.raw");
	p=fopen(dir5,"wb");
	fwrite(indata1,sizeof(PIXTYPE),l*m*n,p);
	fclose(p);
	fflush(stdout);
	delete [] indata1;
	delete [] indata2;

}

void pvalue( string dir1,string dir2)
{
	int l=0,m=0,n=0;
	char dst1[200],dst2[200];
	strcpy(dst1,dir1.c_str()); //string-->char
	strcpy(dst2,dir2.c_str());
	RawImage test;
	char dir3[200]="D:\\sdfdata\\pvaluethickness\\" ;
	char dir4[200]="K:\\sdf\\volume\\clean\\clean\\ep\\";
	strcat(dir3,dst1);
	strcat(dir4,dst2);
	short * indata2=test.readStream(dir4,&l,&m,&n);
	cout<<m<<endl;
	float * indata1=test.readStreamfloat(dir3,&l,&m,&n);
	
	int TT=0,TF=0,FT=0,FF=0;
	double *res=new double[4];
	for (int i = 0; i < n; i++ )
	{
		for (int j = 0; j < l; j++)
		{
			for (int k =0; k < m; k++)
			{

					PIXTYPE val1 = indata1 [i*l*m+j*m+k];
					//cout<<val1<<endl;
					short val2 = indata2 [i*l*m+j*m+k];
					//if (val2 !=0)
					//{
					//	cout << val2 <<endl;
					//} 
					//else
					//{
					//}
				
					if ( val1 == 100)
					{

						if (val2 > -938 && val2 < -124)
						{
							TT++;
						} 
						else
						{
							FT++;
						}
					} 
					if (val2 > -938 && val2 <-124)
					{

						if (val1==0)
						{
							TF++;
						} 

					} 
					else if (val1==0)
					{
						FF++;
					}

	
				
			}
		}
	}
	res [0] = TT; 
	res [1] = FT;
	res [2] = TF;
	res [3] = FF;
	cout <<"TT:"<<TT<<"TF:"<<TF<<"FF:"<<FF<<"FT"<<FT<<endl;
	//FILE *p=fopen("K:\\sdf\\volume\\clean\\clean\\ep\\roc.txt","at+");
	//fwrite(res,sizeof(double),4,p);
	//fclose(p);
	//fflush(stdout);
	ofstream os("K:\\sdf\\volume\\clean\\clean\\ep\\roc.txt",ios::app);
	if (os)
	{
		for (int i=0; i <4; i++)
		{
			os << res[i] <<" ";
			
		}
		os << endl;
	} 
	else cerr <<"no file" <<endl;
	delete [] res;
	delete [] indata1;
	delete [] indata2;

}
void ddcircle(string dir)
{
	int l=512;
	int m=512; 
	int	n=700;
	RawImage test;
	char dst[100];
	strcpy(dst,dir.c_str());
	char dir2[200]="D:\\sdfdata\\";
	strcat(dir2,dst);
	char dir1[200] = "J:\\swfdata\\";
	strcat(dir1,dst);
	float * indata1=test.readStreamfloat(dir2,&l,&m,&n);
	for (int i = 0; i < n; i++)
	{
		for ( int j = 0; j < l; j++)
		{
			for ( int k = 0; k < m; k++)
			{
				PIXTYPE *val=&indata1[i*l*m+j*m+k];
				if ((( j - 256 ) * ( j - 256 ) + ( k - 256 ) * ( k-256 )) >= 40000)
				{

					*val = 0;

				} 
			}
		}
	}
	//Raw *indata=new Raw(l,m,n,indata1);
	FILE *p;
	p=fopen(dir1,"wb");
	fwrite(indata1,sizeof(PIXTYPE),l*m*n,p);
	fclose(p);
	fflush(stdout);
	delete [] indata1;
	//indata->~Raw();
}
void roc()
{
	string dir1(input1);//K:\sdf\volume\clean\clean\ep//
	string dir2(input2);
	//cout << dir1 <<endl;
	vector<string> files1;
	vector<string> files2;
	GetFileNameFromDir(dir1,files1);
	GetFileNameFromDir(dir2,files2);
	vector<string>::iterator iterFile1,iterFile2=files2.begin();;
	for (iterFile1=files1.begin();iterFile1!=files1.end();iterFile1++)
	{

		iterFile2++;
		iterFile1->assign(iterFile1->substr(dir1.size()+1));
		iterFile2->assign(iterFile2->substr(dir2.size()+1));
		cout<<*iterFile1 <<endl;
		cout<<*iterFile2 <<endl;
		pvalue(*iterFile1,*iterFile2);
		//ddcircle(*iterFile);
		//testcolon(argc,*iterFile);
	}

	cout<<endl;
}

int main(int argc,char **argv)
{
	string dir2(input2);
	vector<string> files2;
	GetFileNameFromDir(dir2,files2);
	vector<string>::iterator iterFile2;
	for (iterFile2=files2.begin();iterFile2!=files2.end();iterFile2++)
	{

		iterFile2++;
		iterFile2->assign(iterFile2->substr(dir2.size()+1));
		cout<<*iterFile2 <<endl;
		//ddcircle(*iterFile);
		testcolon(argc,*iterFile2);
	}

	cout<<endl;
	
	system("pause");
	return 0;
}
