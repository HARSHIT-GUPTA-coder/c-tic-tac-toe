/*
Made by:
Harshit Gupta*/

#include<iomanip.h>
#include<stdio.h>
#include<conio.h>
#include<math.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<fstream.h>
#include<time.h>

#define ENTER_KEY '\r'
#define ESCAPE_KEY 27
#define SPECIAL_KEY 0
#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define pos_cursor(x,y) gotoxy(x*4+36,y*2+7)
#define ORDER 3

// The following parameter can be changed as per needed
const int level=3;		// determines the computer level (1=easiest)

char board[ORDER][ORDER];
int computer=1;
int pos;
int t=0;
class save            //class to save the game
	{
	char s[ORDER][ORDER];
	int pw;
	time_t ti;
	char m_name[11];

	public:
	save()
	       {
		ti=NULL;
		pw=0;
		}
	void read();
	void write(int&);
	void ret_name();
	void ret_time()
		{
		cout<<asctime(localtime(&ti));
		}
	};

//The functions' purposes are evident from their names
void display();
void empty_board();
void input(int player);
int chkboard();	//checks if any player has won
void counter_move(int player, int *best_i, int *best_j, int *best_pos);
void best_move(int player);
void sav(int win);
void smenu();

void main()
{
	clrscr();
	if(t==0)
	{
		cout<<"\n\n\n\n\n\n\n\n";
		cout<<setw(55)<<"THE INTELLIGENT COMMON GAME\n\n";
		cout<<setw(52)<<"Made by: Harshit Gupta\n";
		getch();
	t=1;
	}
	main:
	clrscr();
	char opt;
	char str;
	cout<<"\n\n\n\n";
	cout<<" What Do you want to do?";
	cout<<"\n 1. Play new match.";
	cout<<"\n 2. See Previous matches.";
	cout<<"\n 3. Exit.\n\t";
	cin>>opt;
	if(opt=='1')
	{
		clrscr();
		cout<<"\t \t \t \tINSTRUCTIONS\n \n";
		ifstream fob;
		fob.open("i.txt");
		if(!fob)
		       cout<<"INSTRUCTIONS CAN NOT BE DISPLAYED";
		else
		while(!fob.eof())
			{
			fob.get(str);
			cout<<str;
			}
		fob.close();
		AGAIN:
		empty_board();
		computer=1;
		cout<<"\n\nDo you want to play versus COMPUTER ('n' for no) :";
		cin>>opt;

		if ( opt=='n' || opt=='N')
			computer=0;

		int player=1, win=0,turn=1;
		clrscr();
		cout<<"\nPlayer "<< player<< " move \n";
		display();
		do
		{
			cout<<"\nPress arrow keys to move, then press <Enter> to place your mark";
			input(player);
			clrscr();
			cout<<"\nPlayer "<< (player==1?2:1)<< " move \n";
			display();
			win=chkboard();
			player=(player==1)?2:1;
			turn++;
		}while ( win==0 && turn<=ORDER*ORDER );

		if (win)
			cout<<"\nPlayer "<<win<<" won";
		else
			cout<<"\nDraw";
		cout<<"\nDo you want to save the game?(y for yes): ";
		cin>>opt;
		if( opt=='y' || opt=='Y')
			sav(win);
		cout<<"Play again? (y for yes): ";
		cin>>opt;
		if ( opt=='y' || opt=='Y')
			goto AGAIN;
		else
			goto main;
	}
	else
	{
		if(opt=='2')
			{
			 smenu();
			 }
		else
			{
			if(opt=='3')
				exit(0);
			else
			   {
			   cout<<"Wrong choice. Enter again \n";
			   main;
			   }
		}
	}
}

void save::read()
{
	clrscr();
	cout<<"game name: ";
	puts(m_name);
	cout<<"date and time of play :"<<asctime(localtime(&ti));
	cout<<"\nThe board status was \n";
	for (int j=0;j<ORDER*4+3;j++)
	cout<<"Ä";
	for (int i=0;i<ORDER;i++)
	{
		cout<<"\n ";
		cout<<"| ";
		for (int j=0;j<ORDER;j++)
		{
		if(s[i][j]!=NULL)
			cout<<s[i][j];
		else
			cout<<" ";
		cout<<" | ";
		}
		cout<<"\b \n";
		for (j=0;j<ORDER*4+3;j++)
			cout<<"Ä";
	}
	if(pw==0)
		cout<<"\nStatus: Draw";
	else
		cout<<"\nStatus: Player "<<pw<<" won";
}

void save::write(int& win)
{
	for(int i=0;i<ORDER;i++)
	{
		for(int j=0;j<ORDER;j++)
			s[i][j]=board[i][j];
	}
	pw=win;
	cout<<"Enter the match name (max. 10 characters) :";
	gets(m_name);
	time(&ti);
}

void save::ret_name()
       {
       for(int i=0;i<strlen(m_name);i++)
       cout<<m_name[i];
       }


void empty_board()
{
	for (int i=0;i<ORDER;i++)
		for (int j=0;j<ORDER;j++)
			board[i][j]=NULL;
}


void display()
{
	cout<<"\n\n\n\t\t\t\t";
	for (int j=0;j<ORDER*4+3;j++)
	cout<<"Ä";

	for (int i=0;i<ORDER;i++)
	{
		cout<<"\n\t\t\t\t ";
		cout<<"| ";
		for (int j=0;j<ORDER;j++)
		{
			if(board[i][j]!=NULL)
				cout<<board[i][j];
			else
				cout<<" ";
				cout<<" | ";
		}
		cout<<"\n";
		cout<<"\t\t\t\t";
			for (j=0;j<ORDER*4+3;j++)
				cout<<"Ä";
	}
	cout<<"\n\nPress Esc to exit\n";
}

void input(int player)
{
	cout<<"\nPress S for best move";

	if (player==2 && computer==1)	//if it is computer's turn
	{
		best_move(player);
		return;
	}

	int x=0,y=0;
	pos_cursor(x,y);		//macro to position cursor at (y,x)th element
	unsigned char key, key2;
	do
	{
		while( ( key=getch())!=ENTER_KEY )
		{
			if (key==ESCAPE_KEY)
				exit(0);
			else if (key=='S' || key=='s')
				{
					best_move(player);
					return;
				}
			else if (key==SPECIAL_KEY)
			{
				key2=getch();
				switch(key2)
				{
					case UP_ARROW: y--; break;
					case DOWN_ARROW: y++; break;
					case LEFT_ARROW: x--; break;
					case RIGHT_ARROW: x++; break;
				}
				if (x<0)
					 x=0;
				else if
					(x>ORDER-1) x=ORDER-1;
				else if
					(y<0) y=0;
				else if
					(y>ORDER-1) y=ORDER-1;
				pos_cursor(x,y);
			}
		}
	}
	  while(board[y][x]!=NULL);

	char mark=(player==1)?'X':'O';
	board[y][x]=mark;

}

int chkboard(void)
{
	int player2_position=0;
	int player1,player2;

	//rows and cols check
	for (int k=0;k<2;k++)
		for (int i=0;i<ORDER;i++)
		{
			player1=player2=0;

			for (int j=0;j<ORDER;j++)
			{
				if (k==0)		//then check rows
				{
					if (board[i][j]=='X')
						player1++;
					else if (board[i][j]=='O')
						player2++;
				}
				else		// else check cols
				{
					if (board[j][i]=='X')
						player1++;
					else if (board[j][i]=='O')
						player2++;
				}
			}		//end loop j
			if (player1==0)
				player2_position+=player2;
			if (player2==0)
				player2_position-=player1;

			if (player1==ORDER)
			{
				pos=-ORDER*ORDER;
				return 1;
			}
			else if (player2==ORDER)
			{
				pos=ORDER*ORDER;
				return 2;
			}


		}	//end loop i

	//diagonal check
	for (k=0;k<2;k++)
	{
		player1=player2=0;
		for (i=0;i<ORDER;i++)
		{
			if (k==0)		//then check left diagonal
			{
					if (board[i][i]=='X')
						player1++;
					else if (board[i][i]=='O')
						player2++;
			}
			else            //else check right diagonal
			{
					if (board[i][ORDER-1-i]=='X')
						player1++;
					else if (board[i][ORDER-1-i]=='O')
						player2++;
			}
		}		// end loop i
		if (player1==0)
				player2_position+=player2;
		if (player2==0)
				player2_position-=player1;
		pos=player2_position;

		if (player1==ORDER)
		{
			pos=-ORDER*ORDER;
			return 1;
		}
		else if (player2==ORDER)
		{
			pos=ORDER*ORDER;
			return 2;
		}

	}
//	cout<<"\nPlayer 2 Postion %d", player2_position;
	::pos=player2_position;
	return 0;
}


void best_move(int player)
{
	int temp_pos=0, tot_pos;

	tot_pos=(player==1)?3276:-3276;

	int best_i,best_j, best_pos;
	int tmp_i[level],tmp_j[level];
	char mark[3]={0,'X','O'};
	int peak=ORDER*ORDER;
	peak=(player==1)?peak:-peak;
	int next_player=(player==1)?2:1;

	for (int i=0;i<ORDER;i++)
		for (int j=0;j<ORDER;j++)
			if ( board[i][j]==NULL )
			{
				board[i][j]=mark[player];
				temp_pos=0;
				chkboard();
				temp_pos+=pos;
				next_player=player;
				int cnt=0;

				do
				{
					next_player=(next_player==1)?2:1;
					counter_move(next_player, &tmp_i[cnt], &tmp_j[cnt], &best_pos);
					temp_pos+=best_pos;
					if (tmp_i[cnt]!=-1 && (best_pos)!=peak )
						board[ tmp_i[cnt] ] [ tmp_j[cnt] ] = mark[next_player];
					cnt++;

				}
					  while (tmp_i[cnt-1]!=-1 && best_pos!=peak && cnt<level);

				if (cnt==level || tmp_i[cnt-1]==-1 || best_pos==peak)
					cnt--;
				if (tmp_i[cnt]==-1)
					cnt--;

				while(cnt>=0 && tmp_i[cnt]!=-1)
				{
					board[tmp_i[cnt]][tmp_j[cnt]]=NULL;
					cnt--;
				}

				if (player==2)
				{
					if ( tot_pos<temp_pos )
					{
						best_i=i;
						best_j=j;
						tot_pos=temp_pos;
					}
				}
				else
					if ( tot_pos>temp_pos )
					{
						best_i=i;
						best_j=j;
						tot_pos=temp_pos;
					}

				board[i][j]=NULL;
			}  	// end if (board==NULL)

	board[best_i][best_j]=mark[player];
//	cout<<"i=%d,j=%d,pos=%d", best_i,best_j,best_pos);

}


void counter_move(int player, int *best_i, int *best_j, int *best_pos)
{
	*best_i=*best_j=-1;
	char mark=(player==1)?'X':'O';
	*best_pos=(player==1)?3276:-3276;

	for (int i=0;i<ORDER;i++)
		for (int j=0;j<ORDER;j++)
			if ( board[i][j]==NULL )
			{
				board[i][j]=mark;
				chkboard();
				if (player==2)
				{
					if ( *best_pos<pos )
					{
						*best_i=i;
						*best_j=j;
						*best_pos=pos;
					}
				}
				else
					if ( *best_pos>pos )
					{
						*best_i=i;
						*best_j=j;
						*best_pos=pos;
					}

				board[i][j]=NULL;
			}  	// end if (board==NULL)
	if (*best_i==-1)
		*best_pos=0;//(player==2)?3276:-3276;

	return;
}

void sav(int win)
	{
	fstream s("stat.dat",ios::binary | ios::app);
	save ob;
	ob.write(win);
	s.write((char *)& ob, sizeof(ob));
	cout<<"Match successfully stored \n\n";
	s.close();
	}

void smenu()
	{
	back:
	char opt;
	fstream s("stat.dat",ios::binary | ios::in);
	save ob;
	int sno=0;
	if(!s)
		{
		clrscr();
		cout<<"Sorry!! no saved game found! \n";
		cout<<"Press any key to go back to menu...";
		getch();
		main();
		}
	else
		{
		clrscr();
		int i=0;
		cout<<"S.No. \t\t\t\t Name \t\t\t\t Date \n";
		while(s.read((char*)& ob, sizeof(ob)))
			{
			i++;
			cout<<"  "<<i<<"\t\t\t\t";
			ob.ret_name();
			cout<<"\t\t";
			ob.ret_time();
			}
		cout<<"Enter the serial number for the game to be displayed :";
		cin>>sno;
		if(sno>i || sno<=0)
			{
			cout<<"Wrong Serial number...";
			cout<<"\nEnter again or press Esc to exit\n";
			opt=getch();
			if(opt==ESCAPE_KEY)
				exit(0);
			else
				goto back;
			}
		else
		{
		i=1;
		s.close();
		fstream s1("stat.dat",ios::binary | ios::in);
		s.open("s.dat",ios::binary | ios::app);
		while(i<sno)
			{
			s1.read((char*)&ob,sizeof(ob));
			s.write((char*)&ob,sizeof(ob));
			i++;
			}
		s1.read((char*)&ob,sizeof(ob));
		ob.read();
		cout<<"\n\n To delete the opened game press d";
		cout<<"\n Press b to go back or any other key to go menu \n";
		opt=getch();
		if(opt=='D' || opt=='d')
		{
			while(s1.read((char*)&ob,sizeof(ob)))
			{
				i++;
				s.write((char*)&ob,sizeof(ob));
			}
			s1.close();
			s.close();
			remove("stat.dat");
			rename("s.dat","stat.dat");
			cout<<"deleted successfully";
			if(i==1)
				remove("stat.dat");
			getch();
			goto back;
		}
		else
		{
			s1.close();
			s.close();
			if(opt=='B'|| opt=='b')
				goto back;
			else
				main();
		}
		}
		}
	}
