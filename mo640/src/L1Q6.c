/*
 * global.c
 * 
 * Copyright 2017 A Gomes <agomes@lasca.ic.unicamp.br>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define smax 32	// Effective: smax-1
//#define DEBUG 1
//#define VERBOSE 1

int match =		-1;
int ssmatch =	-3;
int gap =		-1;
int memo[smax][smax];


int alignglob(int m, int n, char *s1, char *s2) {
	char align1[2*smax-1];
	char align2[2*smax-1];
	
	int j = m;
	int i = n;
	int idx1 = 0;
	int idx2 = 0;
	
	while (i > 0 || j > 0) {
		if (memo[i][j] - memo[i][j-1] == gap) {
			align1[idx1++] = s1[j-1];
			align2[idx2++] = '-';
			j--;
		}
		else {
			 if (memo[i][j] - memo[i-1][j] == gap) {
				 align1[idx1++] = '-';
				 align2[idx2++] = s2[i-1];
				 i--;
			 } else {
				align1[idx1++] = s1[j-1];
				align2[idx2++] = s2[i-1];
				i--;
				j--;
			}
			
		}
	}
	align1[idx1] = '\0';
	align2[idx2] = '\0';
	
	#ifdef DEBUG
	printf("align1: %s. \nalign2: %s. \n", align1, align2);
	#endif
	
	/* Reverse */
	int aux = strlen(align1);
	/* Custom superstring problem variable */
	char *superstring = calloc(aux, sizeof(char));
	
	printf("\t");
	for (i=1; i <= aux; i++){
		printf("%c", toupper(align1[aux-i]));
		superstring[i-1] = toupper(align1[aux-i]);
	}
	printf("\n\t");
	for (i=1; i <= aux; i++){
		printf("%c", toupper(align2[aux-i]));
		if (align2[aux-i] != '-') superstring[i-1] = toupper(align2[aux-i]);
	}
	superstring[i-1] = '\0';
	
	printf("\n%ld: %s\n\n", strlen(superstring), superstring);
	
	free(superstring);
}


int main(int argc, char **argv)
{
	char s1[smax-1];
	char s2[smax-1];
	
	/* Check */
	if (argc < 3) {
		printf("Use: %s string1 string2\n", argv[0]);
		return 1;
	}
	
	int m, n;
	m = (int) strlen(argv[1]); // Long int to int
	n = (int) strlen(argv[2]);
	
	if (m > sizeof(s1)) {
		printf("string1 is too big. (%d)\n", m);
		return 1;
	}
	
	if (n > sizeof(s1)) {
		printf("string2 is too big. (%d\n", n);
		return 1;
	}
	// Debug
	#ifdef DEBUG
	printf("########## ########## ##########\n");
	printf("Match: %d, Miss-match: %d, Gap: %d.\n", match, ssmatch, gap);
	printf("Max    : %3ld bytes\n", sizeof(s1));
	printf("argv[1]: %3ld bytes\n", strlen(argv[1]));
	printf("argv[2]: %3ld bytes\n", strlen(argv[2]));
	printf("########## ########## ##########\n\n");
	#endif
	
	/* Initialize */
	strcpy(s1, argv[1]);
	strcpy(s2, argv[2]);
	printf("[!] Starting GLOBAL:L1Q6 alignment.\n");
	printf("\tSequence 1: %s\n", s1);
	printf("\tSequence 2: %s\n", s2);
	int i, j;
	
	for (i=0; i<smax; i++) {
		memo[i][0] = i*gap;
		memo[0][i] = i*gap;
	}
	
	/* Start */
	int max;
	for (i=1; i<=n; i++) {
		for (j=1; j<=m; j++) {
			/* Start with diagonal */
			max = memo[i-1][j-1];
			max += (s1[j-1] == s2[i-1]) ? match : ssmatch;
			//printf("[%c - %c]\n", s1[j], s2[i]);
			/* Test gaps */
			if (memo[i][j-1] + gap > max) max = memo[i][j-1] + gap;
			if (memo[i-1][j] + gap > max) max = memo[i-1][j] + gap;
			/* Save value */
			memo[i][j] = max;
			#ifdef VERBOSE
			printf("%2d ", max);
			#endif
		}
		#ifdef VERBOSE
		printf("\n");
		#endif
	}
	
	printf("##### ##### ##### Results: \n\t");
	for (j=0; j<m; j++) {
		printf("%c\t", s1[j]);
	}
	printf("\n");
	
	for (i=1; i<=n; i++) {
		for (j=0; j<=m; j++) {
			if (j==0) {
				printf("%c\t", s2[i-1]);
			} else {
				printf("%2d\t", memo[i][j]);
			}
		}
		printf("\n");
	}
	
	printf("Optimal *GLOBAL:L1Q6* alignment value: %d.\n", memo[n][m]);
	alignglob(m, n, s1, s2);
	return 0;
}

