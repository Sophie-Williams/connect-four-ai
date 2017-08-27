#include<bits/stdc++.h>
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

#define WIN_LEN 4
#define UCB_PARAM 2.828427125

typedef int Move;

int di[4]={-1,0,1,1};
int dj[4]={1,1,1,0};

inline bool check(int x,int y)
{
	return (x>=0 && x<7 && y>=0 && y<8);
}

struct Board
{
	vector< vector<int> > board;

	int move_count;

	Board() {}
	Board(vector< vector<int> > b) : board(b)
	{
		move_count=0;
		for(int i=0;i<b.size();i++)
			for(auto x:b[i])
				if(x)
					move_count++;
	}
	Board(const Board &b) : board(b.board), move_count(b.move_count) {}

	int current_player()
	{
		return (move_count&1)+1;
	}
	vector<Move> get_moves()	//vector of valid moves
	{
		vector<Move> ret;
		for(int i=0;i<board[0].size();i++)
			if(!board[0][i])
				ret.push_back(i);
		return ret;
	}

	void play(Move m)
	{
		int p=current_player(),i;

		if(board[0][m])
		{
			for(int i=0;i<board.size();i++,cout<<'\n')
				for(int j=0;j<board[0].size();j++)
					cout<<board[i][j]<<' ';
			assert(0);
		}

		assert(!board[0][m]);	//assert validity
		
		for(i=0;i<board.size();i++)
			if(board[i][m])
				break;
		board[--i][m]=p;
		move_count++;
	}
	void revert(Move m)
	{
		move_count--;
		for(int i=0;i<board.size();i++)
			if(board[i][m])
			{
				board[i][m]=0;
				return;
			}
		assert(0);	//assert validity
	}
	bool ended()
	{
		int p=3 - current_player();
		for(int i=0;i<board.size();i++)
		for(int j=0;j<board[i].size();j++)
		{
			if(board[i][j]!=p)	continue;
			for(int d=0;d<4;d++)
			{
				int k;
				for(k=1;k<WIN_LEN;k++)
					if(check(i+k*di[d],j+k*dj[d])==false || board[i+k*di[d]][j+k*dj[d]]!=p)
						break;
				if(k==WIN_LEN)	return true;
			}
		}
		return false;
	}

	bool operator==(const Board& b) const { return board==b.board; }

};

inline void hash_combine(size_t& seed, int const& v)	//c++ boost hash_combine
{
	seed ^= std::hash<int>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
struct Hash_Board
{	
	size_t operator()(const Board &b ) const
	{
		size_t seed=0;
		for(int i=0;i<b.board.size();i++)
			for(auto x:b.board[i])
				hash_combine(seed, x);
		return seed;
	}
};

unordered_map<Board,pair<int,int>,Hash_Board> states;

inline double UCB(int wins,int total_child,double log_total_parent,int Final=0)
{
	return ((double)(total_child-wins))/total_child + (1-Final)*UCB_PARAM*sqrt(log_total_parent/total_child);
}

Move select_move(Board &b,int Final=0)
{
	vector<Move> moves=b.get_moves();
	random_shuffle(moves.begin(),moves.end());

	double max_ucb=-1;
	Move chosen_move=-1;

	double log_total_parent=log(states[b].second);
	for(auto x:moves)
	{
		b.play(x);
		if(states.find(b)==states.end())
		{
			states[b]=MP(0,0);
			chosen_move=x;
			b.revert(x);
			break;
		}
		pair<int,int> val=states[b];
		int wins=val.first,total_child=val.second;
	
		b.revert(x);
		
		double ucb=UCB(wins,total_child,log_total_parent,Final);

		// if(Final)
		// 	cout<<ucb<<' '<<x<<'\n';
		
		if(ucb>max_ucb)
		{
			max_ucb=ucb;
			chosen_move=x;
		}
	}
	return chosen_move;
}
int level=0;
int mcts(Board &b)
{
	level++;
	states[b].second+=2;
	if(b.ended())
	{
		level--;
		return 2;
	}
	Move chosen_move = select_move(b);
	if(chosen_move == -1)
	{
		states[b].first++;
		return 1;
	}
	b.play(chosen_move);
	int res=mcts(b);
	b.revert(chosen_move);
	if(res)	states[b].first+=res;
	level--;
	return 2-res;
}

