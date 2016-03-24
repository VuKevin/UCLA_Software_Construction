readme.txt 
Assign 8

============
Premise & Reqs
============

	- Modify the simple ray tracer code in Brian Allen's SRT implementation so that
		- the code is multithreaded
		- runs several times faster on a multicore machine, such as the SEASnet Linux servers
	- Simple Ray Tracer SRT is made of several components
		- Modify only main.c and the Makefile in order to multithread it
		- Your implementation should use POSIX threads 
			- #include <pthread.h>
			- link with the -lpthread libary
		- Modify main function so that it does something useful with the nthreads variable that computes from the leading digit string in the first operand of the program
			- (Currently, it errors out unless nthreads is equal to 1)
		- New code should
			- Invoke pthread_create & pthread_join to create your threads and wait for them to finish
			- It need not use any other part of the POSIX threads interface
			- The rest of this interface is not needed for this application
			- You CAN allocate additional memory via malloc if needed
		- Benchmark and test your program with 
			- make clean check 
				- Should output a file 1-test.pmm that is byte4byte identical with the similarly-named file that is outputted by the unmodified SRT code
					- A copy of which is in the file baseline.ppm
				- This file is in standard Netpbm format and can be displayed as an image
=============
Action Report
=============
main.c is annotated so these steps can be reproduced 

Changes to main.c

00. tar -xvf srt.tgz
00. #include <pthread.h> into main.c
00. I analysed main.c, looking for nested loops to apply multithreading.
	- The region with 4 nested loops seems promising.
01. I added/hoisted variables from the main function
	- int nthreads
	- scene_t scene
	- float scaled_color[h][w][3] // modified with height and width
02. I declared a function prototype of a multithreading method
	- void *execture_threads(void *thread_num)
03. I implemented the same function
	- Took portions from main and transferred it over.
	03a: Modified the outside for loop 
		- Set px to number of threads
		- Incremented px by the number of threads
	03b: Modified scaled_color to align with the global variable scaled_color[h][w][3]
	03c: Return null pointer upon successful run of function
04. I made changes to main
	- Changed if (nthreads != 1) to print to cerr Multithreading will be supported
	04a: Initialize global variable scene 
	04b: Created an array of nthreads size, called thread_arr
	04c: Created and joined threads
		- Create a return code integer container
		- Loop through creating
		- Loop through joining
		- Included error handling with if (return code), as shown from Wed lecture slides
	04d: Modified writing pixels to disk
		- Create a row and column integer containers for the pixels
		- Nested while loops to print, use already written code in main()

Changes to Makefile
- I included lpthread

	- make clean check (DO THIS ON SEASNET!! I did it on Ubuntu originally, but the increase in threads did not make it any faster..)

	1 Thread:
	time ./srt 1-test.ppm >1-test.ppm.tmp
	mv 1-test.ppm.tmp 1-test.ppm
		real	0m52.111s
		user	0m52.105s
		sys		0m0.001s

	2 Threads:
	time ./srt 2-test.ppm >2-test.ppm.tmp
	mv 2-test.ppm.tmp 2-test.ppm
		real	0m27.137s
		user	0m54.193s
		sys		0m0.002s

	4 Threads:
	time ./srt 4-test.ppm >4-test.ppm.tmp
	mv 4-test.ppm.tmp 4-test.ppm
		real	0m14.059s
		user	0m55.090s
		sys		0m0.004s

	8 Threads:
	time ./srt 8-test.ppm >8-test.ppm.tmp
	mv 8-test.ppm.tmp 8-test.ppm
		real	0m7.278s
		user	0m56.311s
		sys		0m0.003s

From this, we can observe the increase in performance due to multithreading is exceptional.
