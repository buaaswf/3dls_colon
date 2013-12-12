#include "vol_math_LevelSet.h"

#include "test.h"
#include <iostream>
#include <crtdbg.h> 
#include "CImg.h" 
#include "ThreeDim_LevelSet.h"
#include "Filter.h"
#include "WipeNioisePde.h"
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

using namespace std;

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
	char dirhead[200]="K:\\sdf\\volume\\clean\\clean\\ep\\";
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
	char *outname1="inner.raw";
	char outdir[200]="D:\\sdfdata\\";
	strcat(outdir,dirbody);
	strcat(outdir,outname1);
	test.writeImageName(*initial,outdir);
	//Raw temp(*initial);
	ls->outerwall(*initial,*input,5.0,0.1,-3,1.5,1,5,pt);
	//*initial -=temp;
	char *outname2="outer.raw";
	char outdir2[200]="D:\\sdfdata\\";
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
	char dir2[200]="D:\\sdfdata\\";
	strcat(dir2,dst);
	char dir3[300];
	strcpy(dir3,dir2);
	strcat(dir3,"outer.raw");
	float * indata1=test.readStreamfloat(dir3,&l,&m,&n);
	char dir4[300];
	strcpy(dir4,dir2);
	strcat(dir4,"inner.raw");
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
	strcat(dir5,"thickness.raw");
	p=fopen(dir5,"wb");
	fwrite(indata1,sizeof(PIXTYPE),l*m*n,p);
	fclose(p);
	fflush(stdout);
	delete [] indata1;
	delete [] indata2;

}

void pvalue(int [])
{
	//int l=512,m=512,n=570;
	//RawImage test;
	//char dst[100];
	//strcpy(dst,dir.c_str());
	//char dir2[200]="D:\\sdfdata\\";
	//strcat(dir2,dst);
	//char dir3[300];
	//strcpy(dir3,dir2);
	//strcat(dir3,"outer.raw");
	//float * indata1=test.readStreamfloat(dir3,&l,&m,&n);

}
void ddcircle(string dir)
{
	RawImage test;
	char dst[100];
	strcpy(dst,dir.c_str());
	char dir2[200]="D:\\sdfdata\\";
	strcat(dir2,dst);
	char dir3[300];
	strcpy(dir3,dir2);
	strcat(dir3,"outer.raw");
	//float * indata1=test.readStreamfloat(dir3,&l,&m,&n);

}
int main(int argc,char **argv)
{
	string dir("K:\\sdf\\volume\\clean\\clean\\ep\\");//K:\sdf\volume\clean\clean\ep//

	vector<string> files;
	GetFileNameFromDir(dir,files);
	vector<string>::iterator iterFile;
	for (iterFile=files.begin();iterFile!=files.end();iterFile++)
	{
		iterFile->assign(iterFile->substr(dir.size()+1));
		cout<<*iterFile <<endl;
		//testcolon(argc,*iterFile);
	}
	cout<<endl;
	
	system("pause");
	return 0;
}
