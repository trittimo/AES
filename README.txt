You can compile and run this program relatively simply using the provided Makefile.
The Makefile has five possible arguments:

make aes
  => Produces the aes binary, which you can use as follows
       > aes [input file] [output file]
     This binary will not output any debug printing, so it is ideal for large tests

make debug
  => Produces the aes_debug binary, which you can use exactly the same as aes
     This binary will output debug prints, so you can see how each step works

make run
  => Runs aes on the two provided input files (aesinput.txt and aesinput2.txt)
     Produces two output files: aesoutput_mine1.txt, and aesoutput_mine2.txt

make test
  => Runs aes_debug on aesinput.txt
     Produces one output file: aesoutput_mine.txt

make clean
  => Cleans the current directory of unnecssary files produced by this program