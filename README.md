# Beam-search-decoding-using-persistence-in-data-structures

The task for beam search decoding for natural
language generation has been done in the assignment
4 of the course, E0 251, IISc Bangalore. In general,
beam search decoding follows the method of
backtracking to generate the desired sentence.
I propose a persistent model for beam search
decoding in which there is no need to backtrack,
this, in turn, saves time. We get our sentence
at the end of the process. Another advantage
of this method is that the process can be
started from the previous versions if the probabilities
have changed for a certain number of
bi-gram pairs. It can also be used to save time
as well as space if the prefix of two sentences
are the same, because we have a version saved
at each iteration. This method can be very efficient
in time for sentences longer than 25-30
words.

## How to run?

Download all the files.
Run the follwing commands,

  make
  
  ./a.out
