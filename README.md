# descracker
1DES cracking utility written in C. Compares result with given plaintext.

##Installation instructions:
`git clone https://github.com/avanasear/descracker.git`\
`cd descracker`\
`make`

If you want to install systemwide, after running `make`, run:\
`sudo make install`\
If you want to uninstall, run:\
`sudo make uninstall`

##Usage instructions:\
First, create a plaintext and ciphertext file:\
`create-crypt-file`\
\
Next, run:\
`descracker ciphertext.txt plaintext.txt`

Note:\
\t	The DES key is currently hard-coded in `src/create-crypt-file.c`, but it can be changed by modifying this file and re-running `make`.
