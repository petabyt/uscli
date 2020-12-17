#include <stdio.h>
#include <ctype.h>
#include "header.h"

#define LIMIT 100

// Pretty print word wrap function I wrote for
// https://code.heb12.com/heb12/heb12cli/src/branch/master/app.c
void printBreak(char string[]) {
	int breakAt = LIMIT;
	for (int c = 0; string[c] != '\0'; c++) {
		putchar(string[c]);
		if (c > breakAt && string[c] == ' ') {
			putchar('\n');
			breakAt += LIMIT;
		}
	}

	putchar('\n');
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		puts("Usage: uscli [options]");
		puts("  -t\tGet Article");
		puts("  -p\tGet Paragraph (of article)");
		puts("  -a\tGet Amendment");
		puts("  -r\tGet Preamble");
		return 0;
	}

	size_t article = 0;
	size_t paragraph = 0;
	size_t amendment = 0;
	size_t *toset = &article;

	// Parse command line arguments
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-' && argv[i][1] != '\0') {
			// Set the pointer
			switch (argv[i][1]) {
			case 'a':
				toset = &amendment;
				break;
			case 'p':
				toset = &paragraph;
				break;
			case 't':
				toset = &article;
				break;
			case 'r':
				printBreak(constitution.preamble);
				return 0;
			}

			// Skip..
			continue;
		}

		// Next parameter is the number, parse it
		size_t c = 0;
		while (isdigit(argv[i][c])) {
			*toset *= 10;
			*toset += (size_t)argv[i][c] - '0';
			c++;
		}

		if (*toset == 0) {
			puts("Start at 1, not zero.");
			return 0;
		}
	}
	
	// Print article or amendment
	if (article != 0) {
		// Use pointer to select location of chapter
		char **articlep;
		switch (article) {
		case 1: articlep = constitution.article_1; break;
		case 2: articlep = constitution.article_2; break;
		case 3: articlep = constitution.article_3; break;
		case 4: articlep = constitution.article_4; break;
		case 5: articlep = constitution.article_5; break;
		case 6: articlep = constitution.article_6; break;
		}

		if (paragraph != 0) {
			printBreak(articlep[paragraph - 1]);
		} else {
			size_t length = sizeof(**articlep) / sizeof(articlep[0]);
			for (size_t i = 0; i < length; i++) {
				printBreak(articlep[i]);
			}
		}
	} else if (amendment != 0) {
		printBreak(constitution.amendments[paragraph - 1]);
	} else {
		puts("No text requested.");
	}

	return 0;
}

