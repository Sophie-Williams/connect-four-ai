#include<bits/stdc++.h>
using namespace std;
 
#define sd(mark) scanf("%d",&mark)
#define ss(mark) scanf("%s",&mark)
#define sl(mark) scanf("%lld",&mark)
#define debug(mark) printf("check%d\n",mark)
#define clr(mark) memset(mark,0,sizeof(mark))
#define F first
#define S second
#define MP make_pair
#define PB push_back
#define ll long long
#define INF 100
int MAX_LEVEL = 5;
int board[10][10];
int cnt[10]={0},cnt_sum=0;
int di[]={-1,0,1,1};
int dj[]={1,1,1,0};
int out_pos=0;
int cols=0;
clock_t start;
void play(int j,int p)
{
	cnt_sum++;
	cnt[j]++;
	board[7-cnt[j]][j]=p;
}
void unplay(int j)
{
	board[7-cnt[j]][j]=0;
	cnt[j]--;
	cnt_sum--;
}
 
bool check(int i,int j,int p)
{
	if(i<0||i>=7||j<0||j>=8)
		return 0;
	return board[i][j]==p;
}
int isWinning(int p)
{
	int i,j,k,l,curi,curj;
	for(i=0;i<7;++i)
		for(j=0;j<8;++j)
			for(k=0;k<4;++k)
			{
				for(l=0;l<4;++l)
					if(!check(i+l*di[k],j+l*dj[k],p))
						break;
				if(l==4)
					return true;
			}
	return false;
}
int eval(int p)
{
	int cnt1=0,cnt2=0,i,j,k,l,curi,curj;
	for(j=0;j<8;++j)
	{
		if(cnt[j]==7)
			continue;
		play(j,p);
		if(isWinning(p))
			cnt1++;
		unplay(j);
		play(j,3-p);
		if(isWinning(3-p))
		{
			cnt2++;
			unplay(j);
			play(j,p);
			if(cnt[j]<7)
			{
				play(j,3-p);
				if(isWinning(3-p))
					cnt2+=2;
				unplay(j);
			}
			unplay(j);
		}
		else
			unplay(j);
	}
	if(cnt1>0)
		return INF;
	if(cnt2>1)
		return -INF;
	int ret=0;
	for(j=0;j<8;++j)
	{
		if(cnt[j]>=6)	continue;
		play(j,p);
		play(j,3-p);
		if(isWinning(3-p))
			ret-=10;
		unplay(j);
		unplay(j);
	}
	for(j=0;j<8;++j)
	{
		if(cnt[j]>=6)	continue;
		play(j,3-p);
		play(j,p);
		if(isWinning(p))
			ret+=10;
		unplay(j);
		unplay(j);
	}
	
	if(cnt2==1)
		ret-=5;
 
	for(i=1;i<7;++i)
	for(j=0;j<8;++j)
		if(board[i][j]==0)
		{
			board[i][j]=p;
			if(isWinning(p))
			{
				board[i][j]=3-p;
				board[i-1][j]=p;
				if(isWinning(p))
					ret+=10;
				board[i-1][j]=board[i][j]=0;
			}
			
			board[i][j]=3-p;
			if(isWinning(3-p))
			{
				board[i][j]=p;
				board[i-1][j]=3-p;
				if(isWinning(3-p))
					ret-=10;
				board[i-1][j]=board[i][j]=0;
			}
			board[i][j]=0;
		}
	for(i=0;i<7;++i)
		for(j=0;j<8;++j)
			for(k=0;k<4;++k)
				if(board[i][j])
					if(check(i+di[k],j+dj[k],board[i][j]))
					{
						if(board[i][j]==p)
							ret+=2;
						else
							ret-=2;
					}
	for(i=0;i<7;++i)
	for(j=0;j<8;++j)
	{
		if(!board[i][j])
			continue;
		bool f=0;
		if(j)
			if(6-i>=cnt[j-1])
				f=1;
		if(j+1<8)
			if(6-i>=cnt[j+1])
				f=1;
		if(7-i==cnt[j])
			f=1;
		if(f)
		{
			if(board[i][j]==p)
				ret+=2;
			else
				ret-=2;
		}
	}
	return ret;
}
int negamax(int alpha,int beta,int level)
{
	int p=(level&1)+1,maxx=-2000000*INF,max_pos=0;
	if(isWinning(3-p))
		return pow(10,max(6-level,0))*INF;
	if(level==MAX_LEVEL)
		return -eval(p);
	if(cnt_sum==56)
		return 0;
	int i,j;
	for(j=0;j<8;++j)
	{
		if(cnt[j]==7)
			continue;
		play(j,p);
		int val=negamax(-beta,-max(maxx,alpha),level+1);
		if(val>maxx)
		{
			maxx=val;
			max_pos=j;
		}
		if(val==maxx)
			if(rand()%3 == 0)
				max_pos=j;
		unplay(j);
		clock_t end = clock();
		if(val>beta||(double)(end-start)/CLOCKS_PER_SEC>=.9)
		{
			out_pos=max_pos;
			return -maxx;
		}
	}
	out_pos=max_pos;
	return -maxx;
}
int main()
{
	srand(time(NULL));
	start = clock();
	int i,j,p;
	for(i=0;i<7;++i)
		for(j=0;j<8;++j)
			sd(board[i][j]);
	sd(p);
	if(p==2)
		for(i=0;i<7;++i)
			for(j=0;j<8;++j)
				if(board[i][j])
					board[i][j]=3-board[i][j];
	for(i=0;i<7;++i)
		for(j=0;j<8;++j)
			if(board[i][j])
				cnt[j]=max(cnt[j],7-i);
	for(i=0;i<8;++i)
	{
		cnt_sum+=cnt[i];
		if(cnt[i]<7)
			cols++;
	}
	if(cols<=3)			MAX_LEVEL=13;
	else if(cols<=4)	MAX_LEVEL=11;
	else if(cols==5)	MAX_LEVEL=10;
	else if(cols==6)	MAX_LEVEL=7;
	else if(cols==7)	MAX_LEVEL=6;
	else				MAX_LEVEL=5;
	for(i=0;i<8;++i)
	{
		if(cnt[i]==7)
			continue;
		play(i,1);
		if(isWinning(1))
			return 0*printf("%d\n",i);
		unplay(i);
	}
	for(i=0;i<8;++i)
	{
		if(cnt[i]==7)
			continue;
		play(i,2);
		if(isWinning(2))
			return 0*printf("%d\n",i);
		unplay(i);
	}
	int ans=negamax(-2000000*INF,2000000*INF,0);
	if(cnt_sum==0)
		out_pos=3;
	printf("%d\n%d\n",out_pos,-ans);
}