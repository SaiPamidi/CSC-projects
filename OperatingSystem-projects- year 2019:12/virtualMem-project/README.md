### Virtual memory simulator
+ It is a program to replicate how memory is fetched from disk. (here we use a file instead with memory instead actual disk drive)
+ the virtual memory manager consists the following - 
	+ TLB - cache
	+ Page Table
	+ Backing store - disk (file)
	+ Physical memory
+ replacing frames in physical memory can be done through FIFO, LRU, and optimal replacement. (optimal knows the next adresses to be accesed)
+ usage :
	+ make
	+ memSim <reference-sequence-file.txt> <FRAMES> <PRA>
+ the reference file consisits a list of logical addresses
+ Frames is the no. of frames in physical memory
+ PRA is the replacement method - fifo,lru or opt