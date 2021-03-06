#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/syscall.h>
#include<linux/random.h>

int getRandom() {
	unsigned long r;
	syscall(SYS_getrandom, &r, 4, 1);
	r = r % 9000 + 1000;
	if (r < 0) {
		r *= -1;
	}
	// Only use it so I know the answer.
	printf("randomNumber:%ld\n", r);
	return r;
}

void intToChar(int number, char *arr) {
	int i;
	for (i = 3; i >= 0; i--) {
		arr[i] = number % 10 + '0';
		number /= 10;
	}
}

int charToInt(char *arr) {
	int i;
	int number = 0;
	for (i = 0; i < 4; i++) {
		number += arr[i];
		number *= 10;
	}
	return number;
}


int check(char *result, char *code, char *guess) {
	int win = 0;
	int count = 0;
	int i;
	int j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (i == j && code[i] == guess[j]) {
				result[i] = 'X';
				count++;
				break;
			}
			else if (i != j && code[i] == guess[j]) {
				result[i] = 'Z';
				break;
			}
		}
	}
	if (count == 4) {
		return 0;
	}
	else {
		return 1;
	}
}

int checkGuess(char *result, char *code, char *guess) {
	int win = 0;
	int j;
	for (j = 0; j < 4; j++) {
		if (code[j] == guess[j]) {
			result[j] = 'X';
		}
		else {
			result[j] = 'O';
			win = 1;
		}
	}
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
	char instructionsMsg[] = "Instructions:\nThe computer generate a 4 digit number and you have 10 guesses to find it\nFor each guess you will receive a result indicating if you guessed a digit successfully\n(O = No, X = Yes)\n";
	char insertGuessMsg[] = "Type your guess:\n";
	char winMsg[] = "\nYou Won!\n";
	char loseMsg[] = "\nYou lose!\n";
	char result[] = "OOOO\n";
	char guess[5];
	char code[5];
	int i;
	int number;
	number = getRandom();
	intToChar(number, code);
	write(1, welcomeMsg, sizeof(welcomeMsg) -1);
	write(1, instructionsMsg, sizeof(instructionsMsg) -1);
	write(1, insertGuessMsg, sizeof(insertGuessMsg) -1);
	read(0, &guess, 5);
	win = check(result, code, guess);
	write(1, "result: ", 7);
	write(1, result, sizeof(result) -1);
	for (i = 0; i < 9 && win == 1; i++) {
		initResult(result);
		write(1, insertGuessMsg, sizeof(insertGuessMsg) -1);
		read(0, &guess, 5);
		win = check(result, code, guess);;
		write(1, result, sizeof(result) -1);
	}
	if (win == 0) {
		write(1, winMsg, sizeof(winMsg) -1);
	}
	else {
		write(1, loseMsg, sizeof(loseMsg) -1);
	}
	return 0;
}
