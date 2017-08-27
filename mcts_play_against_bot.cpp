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

#define TIME_LIMIT 2.0

vector< vector<int> > board(7,vector<int>(8));

int turn_sum=0;

char get_char(int val)
{
	if(!val)
		return ' ';
	return 'A'+val-1;
}
void print_game(Board &b)
{
	int i,j;
	for(j=0;j<8;++j)
		cout<<'-'<<j;
	cout<<'\n';
	for(i=0;i<7;++i,cout<<'\n')
		for(j=0;j<8;++j)
			cout<<' '<<get_char(b.board[i][j]);
	for(j=0;j<8;++j)
		cout<<'-'<<j;
	cout<<'\n';	
}

int main()
{
	srand(time(NULL));
	int i,j,p,user,ans;
	for(i=0;i<7;++i)
		for(j=0;j<8;++j)
			board[i][j]=0;
	Board b(board);
	int out_pos=-1;
	cout<<"Which player do you want to be (enter 1/2): ";
	sd(user);
	p=1;
	while(true)
	{
		print_game(b);
		if(out_pos!=-1&&p==user)
		{
			for(j=0;j<8;++j)
			{
				cout<<' ';
				if(j==out_pos)
					cout<<'^';
				else
					cout<<' ';
			}
			cout<<"\nBot played at column "<<out_pos<<'\n';
		}
		if(turn_sum==56)
		{
			cout<<"It's a tie. Well played!\n";
			return 0;
		}
		turn_sum++;
		if(p==user)
		{
			cout<<"Your turn. ";
			while(true)
			{
				cout<<"Enter column number: ";
				sd(j);
				if(j<0||j>=8)
					cout<<"Invalid Choice. Enter again.\n";
				else if(b.board[0][j])
					cout<<"Invalid Choice. Enter again.\n";
				else
				{
					b.play(j);
					break;
				}
			}
			if(b.ended())
			{
				print_game(b);
				cout<<"Congrats, you win! Well played.\n";
				return 0;
			}
		}
		else
		{
			clock_t start = clock();
			states.clear();
			states[b]=MP(0,0);

			int cnt=0;

			while(true)
			{
				cnt++;
				
				if(cnt%100==0)
				{
					clock_t end = clock();
					if((double)(end-start)/CLOCKS_PER_SEC > .9*TIME_LIMIT)	break;
				}
				mcts(b);
				
			}

			Move chosen_move=select_move(b,1);
			out_pos=chosen_move;
			b.play(chosen_move);
			
			cout<<cnt<<'\n';

			if(b.ended())
			{
				print_game(b);
				cout<<"Bot Wins! It took "<<turn_sum<<" steps to defeat you.";
				if(turn_sum<=30)
					cout<<"You can play better than that!\n";
				else
					cout<<"Well played, it was a close one!\n";
				return 0;
			}
		}
		p=3-p;
	}
}