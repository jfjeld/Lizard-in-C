# LIZARD
This is a C++ implementation of the Lightweight Stream Cipher LIZARD.

The Lizard protocol was proposed by Mattias Hamann, Matthias Krause and Willi Meier and can be found here:
https://eprint.iacr.org/2016/926.pdf

***

## How to use:
1: Copy the Lizard class to your code from Lizard.cpp  
2: Create an object of that class. Example: ``Lizard Object`` at line 199  
3: Call the ``getKeystream("IV", "Key", keystreamLength)`` function.  
IV represented in 16 HEX, Key in 32 HEX and the desired keystream length you will produce.  

See the Lizard.cpp for further examples and explanations. 
