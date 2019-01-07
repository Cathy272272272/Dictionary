I use 2 ways to implement a dictionary:
1. Naive BST Dictionary
2. Self Balancing BST Dictionary(Red-Black Tree)

To compare the performance of two implmentations, I use a alphabetized dictionary(alphabetized_dictionary.txt) and record the total time to import the whole dictionary and the average time to find each word in the dictionary. 

Naive BST Dictionary
Time to import (sec):2.72051  
Avg. time per find (us): 47.52343

Self Balancing BST Dictionary
Time to import (sec): 0.11404,
Avg. time per find (us): 0.89738
 
The difference between the two implementations is because the dictionary is in alphabetical order, so the Naive tree has much more levels, so the time is much more longer.

