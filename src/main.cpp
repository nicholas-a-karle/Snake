#include <iostream>
#include <vector>
#include <winuser.h>
#include <windows.h>
#include <time.h>
using namespace std;

void print_display(vector<vector<char>> display) {
	system("cls");
	for (int k = 0; k < display[0].size() + 1; ++k) cout << '_';
	cout << endl;
	for (int i = 0; i < display.size(); ++i) {
		cout << '|';
		for (int k = 0; k < display[i].size(); ++k) {
			cout << display[i][k];
		}
		cout << '|' << endl;
	}
	for (int k = 0; k < display[0].size() + 1; ++k) cout << '/';
}

pair<int, int> randomLocation(pair<int, int> d) {
	pair<int, int> val;
	val.first = rand() % d.first;
	val.second = rand() % d.second;
	return val;
}

void game() {
	srand(time(0));
	vector<vector<char>> display;
	int score = 0;
	int growth_counter = 0;
	pair<int, int> d = {20, 40};
	pair<int, int> apple = randomLocation(d);
	bool snake_alive = true;
	bool apple_eaten = false;

	vector<pair<int, int>> snake = {
		{d.first/2 - 1, d.second/2},
		{d.first/2 - 2, d.second/2}
	};
	pair<int, int> snake_next = {d.first/2, d.second/2};

	const char blank = ' ';
	const char snake_sym = 'D';
	const char apple_sym = 'O';

	for (int i = 0; i < d.first; ++i) {
		display.push_back(vector<char>(d.second, blank));
	}

	int update_interval = 300; //about 3 every second
	int cycle_interval = 50; //40 every second
	int update_time = 0;
	bool up = false, down = false, right = false, left = true; //start facing left

	short msb_short = 1 << (sizeof(short) * 8 - 1);

	cout << "Press Space to begin..." << endl;
	while (true) if (GetAsyncKeyState(VK_SPACE) & msb_short) break;

	while (snake_alive) {
		//snake has "fake" head which turns when key is pressed at cycle start
		update_time += cycle_interval;

		/**
		 * L -> U
		 * U -> R
		 * R -> D
		 * D -> L
		*/

		bool nup = GetAsyncKeyState(VK_RIGHT) & msb_short;
		bool ndown = GetAsyncKeyState(VK_LEFT) & msb_short;
		bool nright = GetAsyncKeyState(VK_DOWN) & msb_short;
		bool nleft = GetAsyncKeyState(VK_UP) & msb_short;

		//precedence (new then old) (up,down,right, then left)
		up = (up || nup) && !(ndown || nright || nleft);
		down = (down || ndown) && !(nright || nleft);
		right = (right || nright) && !nleft;
		left = (left || nleft);


		if (up) {
			snake_next = {snake[0].first, snake[0].second + 1};
		} else if (down) {
			snake_next = {snake[0].first, snake[0].second - 1};
		} else if (right) {
			snake_next = {snake[0].first + 1, snake[0].second};
		} else if (left) {
			snake_next = {snake[0].first - 1, snake[0].second};
		}

		//check for apple
		if (snake[0] == apple) {
			growth_counter = 2;
			++score;
			apple_eaten = true;
			if (score % 10 == 0) update_interval -= cycle_interval;
		}

		//check for walls
		if (snake[0].first > d.first || snake[0].first < 0 || 
		snake[0].second > d.second || snake[0].second < 0) snake_alive = false;

		//check for snake eating snake
		for (int i = 1; i < snake.size(); ++i) {
			if (snake[i] == snake[0]) snake_alive = false;
		}

		//if apple is eaten, pop it up somewhere random
		if (apple_eaten) {
			apple_eaten = false;
			apple = randomLocation(d);
		}

		if (update_time >= update_interval) {
			update_time = 0;

			//update snake
			//	if growth_counter, decrement, duplicate tail and move all others forward
			//	else, move all forward (count snake_next as first element in practice)
			pair<int, int> new_segment;
			if (growth_counter > 0) new_segment = snake[snake.size() - 1];
			
			for (int i = snake.size() - 1; i >= 1; --i) snake[i] = snake[i-1];
			snake[0] = snake_next;

			if (up) {
				snake_next = {snake_next.first, snake_next.second + 1};
			} else if (down) {
				snake_next = {snake_next.first, snake_next.second - 1};
			} else if (right) {
				snake_next = {snake_next.first + 1, snake_next.second};
			} else if (left) {
				snake_next = {snake_next.first - 1, snake_next.second};
			}

			//adding the new segment
			if (growth_counter > 0) {
				--growth_counter; snake.push_back(new_segment);
			}
		}

		//setting the display
		//clear display
		for (int i = 0; i < display.size(); ++i) {
			for (int k = 0; k < display[i].size(); ++k) {
				display[i][k] = blank;
			}
		}

		display[apple.first][apple.second] = apple_sym;

		for (int i = 0; i < snake.size(); ++i) { 
			display[snake[i].first][snake[i].second] = snake_sym;
		}

		print_display(display);
		cout << endl;
		cout << "SCORE: " << score << endl;
		if (!snake_alive) break;

		Sleep(cycle_interval);
	}

	cout << "GAME OVER" << endl;
	cout << "Press ENTER to RESTART... Press SPACE to EXIT the program" << endl;
	while (true) {
		if (GetAsyncKeyState(VK_RETURN) & msb_short) return game();
		if (GetAsyncKeyState(VK_SPACE) & msb_short) return;
	}

	

}

int main(int argc, char *argv[]) {
	
	game();

}

