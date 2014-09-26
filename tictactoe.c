#include <stdio.h>
#include <malloc.h>

const char 	X = 'x',
			O = 'o',
			NONE = '-'; 

#define getOpponentPiece(piece) (piece == X) ? O : X
#define PLAYER 		1
#define COMPUTER 	2
#define TRUE 		1
#define FALSE 		0

int nodes_expanded = 0;

char *newBoard() {
	int i;
	char *board = (char *)malloc(sizeof(char) * 9);
	for(i = 0; i < 9; i ++) {
		board[i] = NONE;
	}
	return board;
}		

/*
	scan a board using scanf
*/
void readBoard(char *board) {
	int i, j;
	for(i = 0; i < 9; i += 3) {
		char row[4];
		scanf("%s", row);

		for(j = 0; j < 3; j ++)
			board[i + j] = row[j];
	}
}

/*
	print the board
*/
void printBoard(char *board) {
	int i, j;

	printf("\n  ______\n");
	printf(" /      \\\n");

	for(i = 0; i < 9; i += 3) {
		printf("| ");
		for(j = 0; j < 3; j ++)
			printf("%c ", board[i + j]);
		printf(" |\n");
	}
	printf(" \\______/\n");
}

/*
	Returns the character 'X', 'O' or '-' that has won.
*/
char whoWon(char *board) {
	int i;

	//chech rows.
	for(i = 0; i < 3; i ++) {
		if(board[i] != NONE && board[i] == board[i + 3] && board[i + 3] == board[i + 6]) {
			return board[i];
		}
	}

	//check columns.
	for(i = 0; i < 9; i += 3) {
		if(board[i] != NONE && board[i] == board[i + 1] && board[i + 1] == board[i + 2]) {
			return board[i];
		}
	}

	//check principal diagonal.
	if(board[0] != NONE && board[0] == board[4] && board[4] == board[8]) {
		return board[0];
	}

	//check other diagonal.
	if(board[2] != NONE && board[2] == board[4] && board[4] == board[6]) {
		return board[2];
	}

	return NONE;
}

int isFilled(char *board) {
	int i, j;
	for(i = 0; i < 9; i ++) {
		if(board[i] == NONE)
			return FALSE;
	}
	return TRUE;
}

void makePlayerMove(char *board, char playerPiece) {
	char move;
	printBoard(board);
	do {
		printf("\nEnter your move (1 to 9): ");
		move = getch();
	}while(! (move >= '1' && move <= '9' && board[move - '1'] == NONE) );
	board[move - '1'] = playerPiece;
}

char askUser(char *ques, char ch1, char ch2) {
	char input, dump;

	printf("%s", ques);
	input = getch();

	while(input != ch1 && input != ch2) {
		printf("\nInvalid input please enter '%c' or '%c': ", ch1, ch2);
		//scanf("%c", &input);
		//getchar();
		input = getch();
	}

	return input;
}

/*
	Minimax algorithm.
*/

int minimax(char *board, char playerPiece, char computerPiece, int isMaximizer, int alpha, int beta) {

	nodes_expanded ++;

	char winner = whoWon(board);

	/* Handle leaf node cases */

	//if i won.
	if(winner == playerPiece) {
		return 1;
	}
	//if my opponent won.
	else if(winner == computerPiece) {
		return -1;
	}
	//if it is a draw.
	else if(isFilled(board)) {
		return 0;
	}

	/* Minmax backtracking */

	int i, move, max, min;

	max = -2;
	min = 2;
	move = -1;

	for(i = 0; i < 9; i ++) {	
		if(board[i] == NONE) {			/* for each empty cell */

			if(isMaximizer)
				board[i] = playerPiece;
			else
				board[i] = computerPiece;

			int val = minimax(board, playerPiece, computerPiece, 1 - isMaximizer, alpha, beta);

			board[i] = NONE;

			if(isMaximizer) {

				if(val > max) {
					max = val;
					beta = max;
				}

				// 1 is the maximum possible value so break the search.
				// or max maybe greater than alpha.
				if(val == 1 || max >= alpha) {
					break;
				}
			}
			else {
				if(val < min) {
					min = val;
					alpha = min;
				}

				// -1 is the manimum possible value so break the search.
				if(val == -1 || min <= beta) {
					break;
				}
			}
		}
	}	
	if(isMaximizer)
		return max;
	else
		return min;
}

void makeComputerMove(char *board, char computerPiece) {
	int i, min, move;

	char playerPiece = getOpponentPiece(computerPiece);

	min = 2;
	move = -1;

	for(i = 0; i < 9; i ++) {
		if(board[i] == NONE) {
			board[i] = computerPiece;
			int val = minimax(board, playerPiece, computerPiece, 1, 2, -2);//maxAgent(board, playerPiece, computerPiece);
			board[i] = NONE;

			if(min > val) {
				min = val;
				move = i;
			}

			//-1 is the least possible value that a maxAgent can return so no need to check further.
			if(val == -1) {
				break;
			}
		}
	}

	//make the move.
	if(move != -1) {
		board[move] = computerPiece;
	}
}

int main() {
	char playAgain;

	do {
		nodes_expanded = 0;
		//initialize the board.
		char *board = newBoard();

		//get player and computer characters.
		char playerPiece = askUser("\nPlease choose your character (x or o): ", X, O);
		char computerPiece = getOpponentPiece(playerPiece);

		//check if player wants to play first.
		char ch = askUser("\nDo you want to play first ? (y or n): ", 'y', 'n');
		if(ch == 'y') {
			makePlayerMove(board, playerPiece);			
		}
		else if(ch == 'n') {
		}

		char winner;
		int turn = COMPUTER;

		//game loop.
		while(!isFilled(board) && (winner = whoWon(board)) == NONE) {

			if(turn == COMPUTER) {
				makeComputerMove(board, computerPiece);
				turn = PLAYER;
			}
			else {
				makePlayerMove(board, playerPiece);
				turn = COMPUTER;
			}
		}

		printBoard(board);

		//print results.
		if(winner == playerPiece) {
			printf("\n$ Congratulations! you have won. $\n");
		}
		else if(winner == NONE) {
			printf("\n* The game is a draw. *\n");
		}
		else {
			printf("\n@ You Lose!! @\n");
		}

		printf("\nNumber of nodes expanded: %d", nodes_expanded);

		//ask if user wants to play again.
		playAgain = askUser("\n Do you want to play again? (y or n): ", 'y', 'n');
	}while(playAgain == 'y');

	return 0;
}