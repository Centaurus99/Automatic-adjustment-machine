#include<iostream>
#include<cstdio>
#include<algorithm>
#include<cstring>
#include<windows.h>
#include<ctime>
#include<thread>
using namespace std;
inline int read(){
	int x=0,f=1;char ch;
	do{ch=getchar();if(ch=='-')f=-1;}while(ch<'0'||ch>'9');
	do{x=x*10+ch-'0';ch=getchar();}while(ch>='0'&&ch<='9');
	return x*f;
}
//��ʹ�ñ������� -std=c++11 
//����ʹ��O2 
//�������쳣���������� 
//�뱣��ϵͳ�ȶ� 
//˫�̵߳��Σ��������ڴ��������ϴ���ע���ڴ�ʹ�� 
const int OUTNUM=5,INNUM=10,SleepTime=100,DEBUG=0;
// OUTNUM->���ݵ���� (���Ϊ9)
// INNUM->ÿ�����ݵ����д��� 
// SleepTime->�Ƿ����н�������� (ms)
const double EPS=1e-3,S=0.5,T=1;
// EPS->���ξ���
// S->������Сֵ  T->�������ֵ 
bool flag1,flag2;
double sum1,sum2;
void run1(double mid){
	char Filename[10];
	sum1=0;
	int tmp[INNUM+5];
	for (int i=1;i<=OUTNUM;++i){
		sprintf(Filename,"%d",i);
		int len=strlen(Filename);
		Filename[len]='.',Filename[len+1]='i',Filename[len+2]='n',Filename[len+3]=0;
		FILE *fout;
		fout=fopen("alpha1","w");
		fprintf(fout,"%.8lf\n",mid);
		fprintf(fout,"%s\n",Filename);
		fclose(fout);
		if (DEBUG) cerr<<"\t1:"<<Filename<<endl;
		for (int j=1;j<=INNUM+2;++j){
			int t1=clock();
			system("std1.exe");
			tmp[j]=clock()-t1;
		}
		sort(tmp+1,tmp+INNUM+3);
		double avg=0;
		for (int j=2;j<=INNUM+1;++j) avg+=tmp[j];
		avg/=(double)INNUM;
		sum1+=avg;
	}
	flag1=0;
}
void run2(double mid){
	char Filename[10];
	sum2=0;
	int tmp[INNUM+5];
	for (int i=1;i<=OUTNUM;++i){
		sprintf(Filename,"%d",i);
		int len=strlen(Filename);
		Filename[len]='.',Filename[len+1]='i',Filename[len+2]='n',Filename[len+3]=0;
		FILE *fout;
		fout=fopen("alpha2","w");
		fprintf(fout,"%.8lf\n",mid);
		fprintf(fout,"%s\n",Filename);
		fclose(fout);
		if (DEBUG) cerr<<"\t2:"<<Filename<<endl;
		for (int j=1;j<=INNUM+2;++j){
			int t1=clock();
			system("std2.exe");
			tmp[j]=clock()-t1;
		}
		sort(tmp+1,tmp+INNUM+3);
		double avg=0;
		for (int j=2;j<=INNUM+1;++j) avg+=tmp[j];
		avg/=(double)INNUM;
		sum2+=avg;
	}
	flag2=0;
}
int main(){
	double l=S,r=T;
	while (l+EPS<r){
		if (!DEBUG) system("cls");
		double m1=l+(r-l)/3.,m2=r-(r-l)/3.;
		printf("%.4lf %.4lf:\n",m1,m2);
		//�������еĲ��� 
		flag1=1,flag2=1;
		thread t1(run1,m1);t1.detach();
		thread t2(run2,m2);t2.detach();
		while (flag1||flag2) Sleep(SleepTime);
		printf("\t%.2lf %.2lf:\n\n",sum1,sum2);
		//���еĻ���ʱ�� 
		if (sum1<sum2) r=m2;
		else l=m1;
	}
	FILE *fout;
	fout=fopen("ans","w");
	fprintf(fout,"%.3lf\n",l);
	//������ν���� ans �ļ��� 
	fclose(fout);
	return 0;
}
