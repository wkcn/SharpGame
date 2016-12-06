#include "sharpgame.h"
#include <set>
using namespace std;

int get_id(const ChessBoard &b){
	int id = 0;
	for (int r = 0;r < 3;++r){
		for (int c = 0;c < 3;++c){
			id = id * 3 +b.data[r][c];
		}
	}
	return id;
}

//先手为1
int win_count = 0;
int tied_count = 0;
int total_count = 0;
set<int> used;
void search(ChessBoard b, int turn){
	int id = get_id(b);
	//if (used.count(id))return;
	used.insert(id);
	int w = b.get_winner();
	if (w == 1){
		win_count += 1;
		total_count += 1;
		return;
	}
	if (w == 2){
		total_count += 1;
		return;
	}
	bool full = true;
	for (int r = 0;r < 3;++r){
		for (int c = 0;c < 3;++c){
			if (!b.data[r][c]){
				full = false;
				break;
			}
		}
	}
	if (full){
		tied_count += 1;
		total_count += 1;
		return;
	}
	for (int r = 0;r < 3;++r){
		for (int c = 0;c < 3;++c){
			if (!b.data[r][c]){
				b.data[r][c] = turn;
				search(b, 3 - turn);
				b.data[r][c] = 0;
			}
		}	
	}
}

int main(){
	int data[3][3][3]; // [r][c][(win,tied,tot)]
	for (int r = 0;r < 3;++r){
		for (int c = 0;c < 3;++c){
			ChessBoard b;
			b.data[r][c] = 1;
			win_count = 0;
			tied_count = 0;
			total_count = 0;
			used.clear();
			search(b, 2);
			data[r][c][0] = win_count;
			data[r][c][1] = tied_count;
			data[r][c][2] = total_count;
		}
	}


	cout << "win_count" << endl;
	for (int r = 0;r < 3;++r){
		for (int c = 0;c < 3;++c){
			cout << data[r][c][0] << " ";
		}
		cout << endl;
	}

	cout << "tied_count" << endl;
	for (int r = 0;r < 3;++r){
		for (int c = 0;c < 3;++c){
			cout << data[r][c][1] << " ";
		}
		cout << endl;
	}

	cout << "tot_count" << endl;
	for (int r = 0;r < 3;++r){
		for (int c = 0;c < 3;++c){
			cout << data[r][c][2] << " ";
		}
		cout << endl;
	}

	cout << "p_win" << endl;
	for (int r = 0;r < 3;++r){
		for (int c = 0;c < 3;++c){
			cout << data[r][c][0] * 1.0 / (data[r][c][2] - data[r][c][1]) << " ";
		}
		cout << endl;
	}
	return 0;
}
