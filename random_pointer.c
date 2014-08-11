/*
 * Random Pointer
 *
 * This is a little implementation of a program that was suggested by
 * Dr. Robert Harper in a comment on an article called Six Points About
 * Type Safety (http://tel.github.io/2014/08/08/six_points_about_type_safety/)
 * Comment: http://www.pl-enthusiast.net/2014/08/05/type-safety/#comment-500
 *
 * The relevant part of the comment is:
 * > For example, C is perfectly type safe. It’s semantics is a mapping from
 * > 2^64 64-bit words to 2^64 64-bit words. It should be perfectly possible
 * > to call rnd(), cast the result as a word pointer, write to it, and read
 * > it back to get the same value. Unix never implemented the C dynamics properly,
 * > so we get absurdities like “Bus Error” that literally have no meaning whatsoever
 * > in terms of C code.
 *
 * So I decided, what *would* the likelihood of generating a random pointer,
 * dereferincing it, writing a value, and reading it back (verifying it was
 * the same value)
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <limits.h>

#define ITERATIONS 1000

void childTest(unsigned long rndValue) {
	/* Take the random value and convert it into a pointer */
	int * rndPtr = (int *) rndValue;
	
	/* Okay, now do stupid stuff */
	*rndPtr = rndValue;
	if (*rndPtr == rndValue) {
		exit(0);
	}
}

int main() {
	/* No crashes ... yet */
	unsigned int crashes = 0;

	/* Init the random number generator */
	srand(time(NULL));
	unsigned long randNum = 0;
	
	int cpid;
	int i;
	for (i = 0; i < ITERATIONS; ++i) {
		/*
		 * Generate the random number in the parent
		 * because allowing the child to do it just
		 * doesn't really work.
		 */
		randNum = random();

		/*
		 * Basically we do the real work in a child process because
		 * a) I'm not sure how to catch a SIGSEGV (and continue)
		 * b) Even if I did, it's possible the pointer write will
		 *    mess up the stack and so the program would crash
		 *    because of that.
		 * So rather than worry about any of that, do it as a
		 * child process
		 */
		if ((cpid = fork()) == 0) {
			childTest(randNum);
		} else {
			int status;
			wait(&status);
			if (status == SIGSEGV) {
				++crashes;
			} else if (status == 0) {
				printf("Success! (How?): 0x%lx\n", randNum);
			}
		}
		
		/* Just print the iteration so we can see some progress */
		printf("Iteration: %d\r", i);
		fflush(stdout);
	}
	
	/* Quickly calculate the percentage of crashes (vs runs) and print the results */
	float percent = ((float) crashes / ITERATIONS) * 100;
	printf("Crashed %d of %d runs - %f%%\n", crashes, ITERATIONS, percent);
}