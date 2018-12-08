#include<iostream>
#include<cstdio>
#include<algorithm>
#include<cstring>
using namespace std;
inline int read(){
	int x=0,f=1;char ch;
	do{ch=getchar();if(ch=='-')f=-1;}while(ch<'0'||ch>'9');
	do{x=x*10+ch-'0';ch=getchar();}while(ch>='0'&&ch<='9');
	return x*f;
}
double Alpha;
const int N=200010,LOG=6,LOG2=20,MOD=1048576;
int ch[N*LOG*LOG2][2],lc,cnt[N*LOG*LOG2],s[N*LOG*LOG2],ls;
void tnewnode(int &o){
	if (ls) o=s[ls--];
	else o=++lc;
	ch[o][0]=ch[o][1]=0,cnt[o]=0;
}
void tinsert(int &rt,int x){
	if (!rt) tnewnode(rt);
	int now=rt;++cnt[rt];
	for (int i=LOG2-1;~i;--i){
		int v=(x>>i)&1;
		if (!ch[now][v]) tnewnode(ch[now][v]);
		now=ch[now][v];
		++cnt[now];
	}
}
void tdel(int &rt,int x){
	int now=rt,lnow=now;--cnt[rt];
	if (!cnt[rt]) s[++ls]=rt,rt=0;
	for (int i=LOG2-1;~i;--i){
		int v=(x>>i)&1;
		now=ch[now][v],--cnt[now];
		if (!cnt[now]) ch[lnow][v]=0,s[++ls]=now;
		lnow=now;
	}
}
int task(int rt,int x){
	int now=rt,ans=0;
	for (int i=LOG2-1;~i;--i){
		int v=(x>>i)&1;
		if (ch[now][v^1]) now=ch[now][v^1],ans|=(1<<i);
		else now=ch[now][v];
	}
	return ans;
}
struct Node{
	int ch[2],sz,s,del,x,mx,mx2,rt;
}T[N];
int root,lt,tmp[5],*lst;
void newnode(int &o,int x){
	o=++lt;
	T[o].ch[0]=T[o].ch[1]=T[o].del=0;
	T[o].x=T[o].mx=x,T[o].mx2=0;
	T[o].s=T[o].sz=1;
	tinsert(T[o].rt,x);
}
inline void pushup(int o){
	T[o].sz=T[T[o].ch[0]].sz+T[T[o].ch[1]].sz+1;
	T[o].s=T[T[o].ch[0]].s+T[T[o].ch[1]].s+1-T[o].del;
	if (!T[o].del) tmp[0]=T[o].x;
	else tmp[0]=0;
	tmp[1]=T[T[o].ch[0]].mx,tmp[2]=T[T[o].ch[0]].mx2;
	tmp[3]=T[T[o].ch[1]].mx,tmp[4]=T[T[o].ch[1]].mx2;
	sort(tmp,tmp+5);
	T[o].mx=tmp[4],T[o].mx2=tmp[3];
}
inline bool balance(int o){
	if ((double)T[T[o].ch[0]].sz>Alpha*(double)T[o].sz) return 0;
	if ((double)T[T[o].ch[1]].sz>Alpha*(double)T[o].sz) return 0;
	return 1;
}
void insert(int &o,int k,int x){
	if (!o){
		newnode(o,x);
		return;
	}
	if (k<=T[T[o].ch[0]].s) insert(T[o].ch[0],k,x);
	else insert(T[o].ch[1],k-T[T[o].ch[0]].s-1+T[o].del,x);
	pushup(o);
	tinsert(T[o].rt,x);
	if (!balance(o)) lst=&o;
}
int del(int o,int k){
	int num;
	if (k<=T[T[o].ch[0]].s) num=del(T[o].ch[0],k);
	else if (!T[o].del&&k==T[T[o].ch[0]].s+1) T[o].del=1,num=T[o].x;
	else num=del(T[o].ch[1],k-T[T[o].ch[0]].s-1+T[o].del);
	pushup(o);
	tdel(T[o].rt,num);
	return num;
}
int change(int o,int k,int to){
	int num;
	if (k<=T[T[o].ch[0]].s) num=change(T[o].ch[0],k,to);
	else if (!T[o].del&&k==T[T[o].ch[0]].s+1) num=T[o].x,T[o].x=to;
	else num=change(T[o].ch[1],k-T[T[o].ch[0]].s-1+T[o].del,to);
	pushup(o);
	tdel(T[o].rt,num),tinsert(T[o].rt,to);
	return num;
}
void dfs2(int o){
	if (!o) return;
	s[++ls]=o;
	dfs2(ch[o][0]),dfs2(ch[o][1]);
}
int nowtmp[N],ln,cnt1[N],cnt2[N],nownum;
void dfs1(int o){
	if (!o) return;
	dfs1(T[o].ch[0]);
	if (!T[o].del) nowtmp[++ln]=o;
	dfs1(T[o].ch[1]);
	dfs2(T[o].rt);
}
int build(int l,int r){
	if (l>r) return 0;
	int mid=(l+r)>>1,o=nowtmp[mid];
	T[o].rt=0;
	T[o].ch[0]=build(l,mid-1);
	T[o].ch[1]=build(mid+1,r);
	pushup(o);
	for (int i=l;i<=r;++i) tinsert(T[o].rt,T[nowtmp[i]].x);
	return o;
}
void rebuild(int &o){
	ln=0,dfs1(o);
	o=build(1,ln);
}
void qask(int o,int l,int r,int x,int y){
	if (x<=l&&r<=y){
		cnt1[++cnt1[0]]=o;
		return;
	}
	int mid=l+T[T[o].ch[0]].s-T[o].del;
	if (!T[o].del&&x<=mid&&mid<=y) cnt2[++cnt2[0]]=o;
	if (x<=mid-(T[o].del^1)) qask(T[o].ch[0],l,mid-(T[o].del^1),x,y);
	if (y>mid) qask(T[o].ch[1],mid+1,r,x,y);
}
int solve(int x,int y){
	cnt1[0]=cnt2[0]=0;
	qask(root,1,nownum,x,y);
	int mx=0,mx2=0,ans=0;
	for (int i=1;i<=cnt1[0];++i){
		int o=cnt1[i];
		if (T[o].mx>=mx) mx2=mx,mx=T[o].mx;
		else if (T[o].mx>mx2) mx2=T[o].mx;
	}
	for (int i=1;i<=cnt1[0];++i){
		int o=cnt1[i];
		if (T[o].mx2>=mx) mx2=mx,mx=T[o].mx2;
		else if (T[o].mx2>mx2) mx2=T[o].mx2;
	}
	for (int i=1;i<=cnt2[0];++i){
		int o=cnt2[i];
		if (T[o].x>=mx) mx2=mx,mx=T[o].x;
		else if (T[o].x>mx2) mx2=T[o].x;
	}
	for (int i=1;i<=cnt1[0];++i){
		int o=cnt1[i];
		ans=max(ans,task(T[o].rt,mx2));
	}
	for (int i=1;i<=cnt2[0];++i){
		int o=cnt2[i];
		ans=max(ans,mx2^T[o].x);
	}
	return ans;
}
char opt[5];
char Filename[100];
int main(){
	freopen("alpha2","r",stdin);
	scanf("%lf",&Alpha);
	scanf("%s",Filename);
	freopen(Filename,"r",stdin);
	freopen("tmp2.out","w",stdout);
	int n=read(),m=read(),ans=0;nownum=n;
	for (int i=1;i<=n;++i){
		int x=read();
		newnode(i,x);
		nowtmp[i]=i;
	}
	root=build(1,n);
	while (m--){
		scanf("%s",opt);
		if (opt[0]=='I'){
			int x=(read()+ans)%nownum,y=(read()+ans)%MOD;
			lst=0;++nownum;
			insert(root,x,y);
			if (lst) rebuild(*lst);
		}
		else if (opt[0]=='D'){
			int x=(read()+ans)%nownum+1;
			del(root,x);--nownum;
		}
		else if (opt[0]=='C'){
			int x=(read()+ans)%nownum+1,y=(read()+ans)%MOD;
			change(root,x,y);
		}
		else{
			int l=(read()+ans)%nownum+1,r=(read()+ans)%nownum+1;
			printf("%d\n",ans=solve(l,r));
		}
	}
	fclose(stdin);
	fclose(stdout);
	return 0;
}
