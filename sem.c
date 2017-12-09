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
#include <unistd.h>

#include "cns.h"

#define MAX_PATH 4096

void import(char* type, char* bounds, char* val) {
	if (strcmp(type, "import ") == 0) {
		printf("//%s%c%s%c\n", type, bounds[0], val, bounds[1]);
		char path    [MAX_PATH];
		char exe_path[MAX_PATH];

		//Get the path of the executable. This'll be handy later.
		readlink("/proc/self/exe", exe_path, MAX_PATH);

		if (bounds[0] == '\"' && bounds[1] == bounds[0]) {
			getcwd(path, MAX_PATH);
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
			strcat(path, ".c");
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
			fprintf(stderr, "[FATAL ERROR] Failed to execvp child process.\n");
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
		printf("#%s%c%s%c", type, bounds[0], val, bounds[1]);
}

/* Functions */
void fend() {
	printf("}");
}
