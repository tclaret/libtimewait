.PHONY: all clean test

all: libtimewait libtimewait_listen libtimewait_socket

libtimewait:
	$(CC) -Wall -Wextra -pedantic -D_GNU_SOURCE -nostartfiles -shared -fpic -fPIC \
		-fvisibility=hidden \
		-Wconversion -Wshadow \
		-Wpointer-arith -Wcast-qual \
		-Wstrict-prototypes -Wmissing-prototypes \
		-o $@.so timewait.c $@.c -ldl \
		-Wl,-z,relro,-z,now -Wl,-z,noexecstack

libtimewait_listen:
	$(CC) -Wall -Wextra -pedantic -D_GNU_SOURCE -nostartfiles -shared -fpic -fPIC \
		-fvisibility=hidden \
		-Wconversion -Wshadow \
		-Wpointer-arith -Wcast-qual \
		-Wstrict-prototypes -Wmissing-prototypes \
		-o $@.so timewait.c $@.c -ldl \
		-Wl,-z,relro,-z,now -Wl,-z,noexecstack

libtimewait_socket:
	$(CC) -Wall -Wextra -pedantic -D_GNU_SOURCE -nostartfiles -shared -fpic -fPIC \
		-fvisibility=hidden \
		-Wconversion -Wshadow \
		-Wpointer-arith -Wcast-qual \
		-Wstrict-prototypes -Wmissing-prototypes \
		-o $@.so timewait.c $@.c -ldl \
		-Wl,-z,relro,-z,now -Wl,-z,noexecstack

clean:
	-@rm libtimewait.so libtimewait_listen.so libtimewait_socket.so

test:
	@bats test
