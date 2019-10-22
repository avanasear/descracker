# descracker
1DES cracking utility written in C. Compares result with given HMAC.

Compile create-crypt-file.c with  
`gcc create-crypt-file.c -o create-crypt-file -lcrypto`  
\
Compile main.c with  
`gcc main.c -o main -lcrypto -pthread`  