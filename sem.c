/*
 * CN_Script: The easier way to code in C.
 *
 * Description:
 *     CN_Script is a subset of the C language which integrates tools that make
 *     coding in C a bit more managable, including data structures and other
 *     tools to enforce good coding behaviour. The CN_Script language will
 *     "transpile" into valid C Code (assuming you didn't mess up at all), and
 *     can be compiled with a C compiler afterwards.
 *
 * Author:
 *     Clara Nguyen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "cns.h"

#define MAX_PATH 4096

void import(char* type, char* bounds, char* val) {
	if (strcmp(type, "import") == 0) {
		printf("//%s %c%s%c\n", type, bounds[0], val, bounds[1]);
		char path    [MAX_PATH];
		char exe_path[MAX_PATH];
		int plen;

		//Get the path of the executable. This'll be handy later.
		plen = readlink("/proc/self/exe", exe_path, MAX_PATH);
		if (plen < MAX_PATH && plen != -1)
			exe_path[plen] = '\0';

		if (bounds[0] == '\"' && bounds[1] == bounds[0]) {
			//getcwd(path, MAX_PATH);
			strcpy(path, CUR_FILE);

			//Get rid of executable name
			size_t buf = strlen(path) - 1;
			char t;
			while (1) {
				t = path[buf];
				path[buf] = 0;
				if (t == '/' || buf == 0)
					break;
				buf--;
			}
			strcat(path, "/");
			strcat(path, val);
		}
		else
		if (bounds[0] == '<' && bounds[1] == '>') {
			readlink("/proc/self/exe", path, MAX_PATH);

			//Get rid of executable name
			size_t buf = strlen(path) - 1;
			char t;
			while (1) {
				t = path[buf];
				path[buf] = 0;
				if (t == '/' || buf == 0)
					break;
				buf--;
			}
			strcat(path, "/lib/");
			strcat(path, val);
			strcat(path, ".cns");
		}
		printf(
			"// ---\n"
			"// Importing from:\n// %s\n"
			"// ---\n",
			path
		);

		//Now for the fun part... let's put the data in here.
		char* argv[3];
		argv[0] = exe_path;
		argv[1] = path;
		argv[2] = NULL;

		int pipes[2][2];
		pipe(pipes[1]);
		pipe(pipes[0]);

		int fk = fork();
		if (fk == -1) {
			//Failure
			fprintf(stderr, "[FATAL ERROR] Failed to fork process.\n");
			exit(2);
		}
		else
		if (fk == 0) {
			//We are the child.
			dup2(pipes[0][0], STDIN_FILENO);
			dup2(pipes[1][1], STDOUT_FILENO);

			close(pipes[0][0]);
			close(pipes[1][1]);
			close(pipes[0][1]);
			close(pipes[1][0]);

			execvp(exe_path, argv);	
			fprintf(stderr, "[FATAL ERROR] Failed to execvp child process. (%d)\n", errno);
		}
		else {
			close(pipes[0][0]);
			close(pipes[1][1]);
			int c;
			FILE* ya = fdopen(pipes[1][0], "r");
			while (1) {
				c = fgetc(ya);
				if (c == EOF)
					break;
				putchar(c);
			}
			fclose(ya);

			//Hi Dr. Plank
			int status;
			waitpid(fk, &status, 0);
		}

		//[SCRAPPED] Cheap-ass way: Copy file to stdout.
		/*FILE* fp = fopen(path, "rb");
		if (!fp) {
			fprintf(stderr, "[FATAL ERROR] %s doesn't exist!\n", val);
			exit(1);
		}
		int c;
		while (c = fgetc(fp)) {
			if (c == -1)
				break;
			putchar(c);
		}
		fclose(fp);*/

		printf(
			"// ---\n"
			"// End import from:\n// %s\n"
			"// ---\n",
			path
		);
	}
	else
		printf("#%s %c%s%c", type, bounds[0], val, bounds[1]);
}

/* Functions */
void fend() {
	printf("}");
}

void append_pair(char* name, char* type) {
	VAR_PAIR tmp;
	tmp.name = strdup(name);
	tmp.type = strdup(type);
	cn_vec_push_back(VAR_PAIRS, &tmp);
}

void clear_pairs() {
	unsigned int i = 0;
	for (; i < cn_vec_size(VAR_PAIRS); i++) {
		VAR_PAIR* ii = (VAR_PAIR*) cn_vec_at(VAR_PAIRS, i);
		free(ii->name);
		free(ii->type);
	}
	cn_vec_clear(VAR_PAIRS);
}

/* Functions you shouldn't really use... */
char* malloc_concat(char* s1, char* s2) {
	size_t l1, l2;
	char* s3;
	l1 = strlen(s1);
	l2 = strlen(s2);

	//You MUST call free on this.
	s3 = (char *) malloc(l1 + l2 + 1);
	s3[l1 + l2] = 0;
	memcpy(s3     , s1, l1);
	memcpy(&s3[l1], s2, l2);

	return s3;
}
char* realloc_concat(char* s1, char* s2) {
	size_t l1, l2;
	char* s3;
	l1 = strlen(s1);
	l2 = strlen(s2);

	//You MUST call free on this.
	s1 = (char *) realloc(s1, l1 + l2 + 1);
	s1[l1 + l2] = 0;
	memcpy(&s1[l1], s2, l2);

	return s1;
}

char* realloc_rconcat(char* s1, char* s2) {
	size_t l1, l2;
	char* s3;
	l1 = strlen(s1);
	l2 = strlen(s2);

	//You MUST call free on this.
	s2 = (char *) realloc(s2, l1 + l2 + 1);
	s2[l1 + l2] = 0;
	memcpy(&s2[l2], s1, l1);

	return s2;
}

void freeifnull(void* ptr) {
	if ((char*)ptr != NULL)
		free(ptr);
}
