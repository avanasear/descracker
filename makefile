CC=gcc
CFLAGS="-lcrypto"

descracker: src/main.c src/create-crypt-file.c
	$(CC) -o descracker src/main.c $(CFLAGS) -pthread
	$(CC) -o create-crypt-file src/create-crypt-file.c $(CFLAGS)

install: descracker create-crypt-file
	cp descracker /usr/bin/descracker
	cp create-crypt-file /usr/bin/create-crypt-file

uninstall: /usr/bin/descracker /usr/bin/create-crypt-file
	rm -f /usr/bin/descracker
	rm -f /usr/bin/create-crypt-file
