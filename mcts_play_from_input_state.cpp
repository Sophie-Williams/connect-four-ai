#include <bits/stdc++.h>
#include "monte_carlo_tree_search.cpp"

using namespace std;

#define sd(a) scanf("%d",&a)
#define ss(a) scanf("%s",&a)
#define sl(a) scanf("%lld",&a)
#define clr(a) memset(a,0,sizeof(a))
#define debug(a) printf("check%d\n",a)
#define F first
#define S second
#define MP make_pair
#define PB push_back
#define ll long long

#define TIME_LIMIT .9

vector< vector<int> > board(7,vector<int>(8));

int main()
{
	clock_t start = clock();
	
	int i,j,p;
	for(i=0;i<7;i++)
		for(j=0;j<8;j++)
			cin>>board[i][j];
	sd(p);
	Board b(board);
	states[b]=MP(0,0);

	int cnt=0;

	while(true)
	{
		cnt++;
		
		if(cnt%10==0)
		{
			clock_t end = clock();
			if((double)(end-start)/CLOCKS_PER_SEC > .9*TIME_LIMIT)	break;
		}
		mcts(b);
		// for(int i=0;i<b.board.size();i++,cout<<'\n')
		// 	for(int j=0;j<b.board[0].size();j++)
		// 		cout<<b.board[i][j]<<' ';
		// sd(p);
	}

	cout<<select_move(b,1)<<'\n';
	cout<<cnt<<'\n';
}