#include<iostream>

class TicTacToe
{
private:
	char board[9];
	const char* symbols = "XO";
	bool turn ;
	int scoreX, scoreO;
	enum State {WIN, NOTWIN, DRAW};
public:
	TicTacToe()
	{	
		ResetBoard();
		turn = 0;
		scoreX = 0;
		scoreO = 0;
	}

	void RunGame()
	{
		Draw();
		HandleInput();

		State state = CheckWin();
		switch (state)
		{
		case NOTWIN:
			return RunGame();
		case WIN:
			Draw();
			std::cout << symbols[(turn + 1) % 2] << " Won The Game!" << std::endl;;
			turn == 0 ? scoreO++ : scoreX++;
			break;
		case DRAW:
			Draw();
			std::cout << "Its a draw!" << std::endl;;
			break;
		}

		std::cout << "Press Enter to continue playing . . . ." << std::endl;
		std::cin.get();
		if (std::cin.get() == '\n')
		{
			ResetBoard();
			RunGame();
		}
	}
private:
	void HandleInput()
	{
		char c;
		std::cout << "Enter Position to place (" << symbols[turn] << "): ";
		std::cin >> c;

		if (c < '1' || c > '9' || board[(int)c - 49] != ' ')
		{
			Draw();
			std::cout << "\tInvalid Number!" << std::endl;
			return HandleInput();
		}

		board[(int)c - 49] = symbols[turn];
		turn = (turn + 1) % 2;
	}

	State CheckWin() const
	{
		if (board[0] != ' ' && board[0] == board[1] && board[1] == board[2]) return WIN;
		if (board[3] != ' ' && board[3] == board[4] && board[4] == board[5]) return WIN;
		if (board[6] != ' ' && board[6] == board[7] && board[7] == board[8]) return WIN;

		if (board[0] != ' ' && board[0] == board[3] && board[3] == board[6]) return WIN;
		if (board[1] != ' ' && board[1] == board[4] && board[4] == board[7]) return WIN;
		if (board[2] != ' ' && board[2] == board[5] && board[5] == board[8]) return WIN;

		if (board[0] != ' ' && board[0] == board[4] && board[4] == board[8]) return WIN;
		if (board[2] != ' ' && board[2] == board[4] && board[4] == board[6]) return WIN;

		for (int i = 0; i < 9; ++i)
		{
			if (board[i] == ' ') return NOTWIN;
		}

		return DRAW;
	}

	void Draw()
	{
		system("cls");
		system("color 0C");
		std::cout << "Score(X): " << scoreX << ", Score(O): " << scoreO << std::endl;
		std::cout << std::endl;
		std::cout << "\t     |     |" << std::endl;
		std::cout << "\t  " << board[0] << "  |  " << board[1] << "  |  " << board[2] << std::endl;
		std::cout << "\t     |     |" << std::endl;
		std::cout << "\t-----|-----|-----" << std::endl;
		std::cout << "\t     |     |" << std::endl;
		std::cout << "\t  " << board[3] << "  |  " << board[4] << "  |  " << board[5] << std::endl;
		std::cout << "\t     |     |" << std::endl;
		std::cout << "\t-----|-----|-----" << std::endl;
		std::cout << "\t     |     |" << std::endl;
		std::cout << "\t  " << board[6] << "  |  " << board[7] << "  |  " << board[8] << std::endl;
		std::cout << "\t     |     |" << std::endl;
	}

	void ResetBoard()
	{
		for (int i = 0; i < 9; i++)
		{
			board[i] = ' ';
		}
	}
};

int main()
{
	std::cout << "\t... Tic Tac Game by Raj ..." << std::endl;
	std::cout << "Press any key to start ..." << std::endl;
	std::cin.get();
	TicTacToe* game = new TicTacToe();
	game->RunGame();

	delete game;
	std::cin.get();
}