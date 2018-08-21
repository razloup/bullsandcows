#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/syscall.h>
#include<linux/random.h>

void getRandom(char *code) {
	int i;
	int r;
	printf("code@:%s", code);
	syscall(SYS_getrandom, &r, 4, 1);
	r = r % 9000 + 1000;
	if (r < 0) {
		r *= -1;
	}
	for (i = 3; i >= 0; i--) {
		code[i] = r % 10;
		r /= 10;
	}
	printf("code!:%s", code);
}

int checkGuess(char *result, char *code, char *guess) {
	int win = 0;
	int j;
	printf("\n@@@@@@@\n");
	for (j = 0; j < 4; j++) {
		printf("code:%d, ", code[j]);
		printf("guess:%d\n", guess[j]);
		if (code[j] == guess[j]) {
			result[j] = 'X';
		}
		else {
			result[j] = 'O';
			win = 1;
		}
	}
	printf("@#@#@#\n");
	return win;
}

void initResult(char *result) {
	result[0] = 'O';
	result[1] = 'O';
	result[2] = 'O';
	result[3] = 'O';
}

int main() {
	int win = 1;
	char welcomeMsg[] = "\nWelcome to Bulls & Cows!\n";
	char instructionsMsg[] = "Instructions:\nThe computer generate a 4 digit number and you have 10 guesses to find it\nFor each guess you will receive a result\n";
	char insertGuessMsg[] = "Type your guess:\n";
	char winMsg[] = "\nYou Won!\n";
	char loseMsg[] = "\nYou lose!\n";
	char result[] = "OOOO\n";
	char guess[5];
	int i;
	char code[] = "0000\n";
	getRandom(code);
	write(1, welcomeMsg, sizeof(welcomeMsg) -1);
	write(1, instructionsMsg, sizeof(instructionsMsg) -1);
	write(1, insertGuessMsg, sizeof(insertGuessMsg) -1);
	read(0, guess, 4);
	printf("code:%s\n", code);
	printf("guess:%s\n", guess);
	win = checkGuess(result, code, guess);
	printf("****\n");
	write(1, result, sizeof(result) -1);
	for (i = 1; i < 10 && win == 1; i++) {
		//initResult(result);
		printf("####\n");
		write(1, insertGuessMsg, sizeof(insertGuessMsg) -1);
		printf("$$$$\n");
		read(0, guess, 4);
		printf("^^^^\n");
		win = checkGuess(result, code, guess);
		printf("&&&&\n");
		write(1, result, sizeof(result) -1);
		i++;
	}
	if (win == 0) {
		write(1, winMsg, sizeof(winMsg) -1);
	}
	else {
		write(1, loseMsg, sizeof(loseMsg) -1);
	}
	return 0;
}
