File explainations:

`typescript`: The script showing the execution of program on test cases
`main.c`: Main driver program
`mailbox.c`: All the utility functions related to how each thread handles receiving and sending
`queue.c`: All the utility functions related to queue undelivered messages.

To compile:
Run `make`

To remove executable and object files from previous compilation
Run `make clean`


To run after compilation:
Run `./mailbox [numThreads]` or run `./mailbox [numThreads] nb`