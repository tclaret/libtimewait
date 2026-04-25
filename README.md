Inspired by libtcpkeepalive, we're defaulting the "so_reuse*" socket options.

"libtcpkeepalive
 Michael Santos https://github.com/msantos/libkeepalive
""


# SYNOPSIS

# client only

LD_PRELOAD=libtimewait.so *COMMAND* *ARG* *...*

# server only

LD_PRELOAD=libtimewait_listen.so *COMMAND* *ARG* *...*

# client and server

LD_PRELOAD=libtimewait.so:libtimewait_listen.so *COMMAND* *ARG* *...*

# client and/or server

LD_PRELOAD=libtimewait_socket.so *COMMAND* *ARG* *...*

# DESCRIPTION

libtimewait: set SO_REUSEADDR and SO_REUSEPORT socket options

libtimewait is a small library for setting various socket options
required for enabling SO_REUSEADDR and SO_REUSEPORT. Inspired by:

* https://blog.cloudflare.com/when-tcp-sockets-refuse-to-die/
* https://blog.cloudflare.com/how-to-stop-running-out-of-ephemeral-ports-and-start-to-love-long-lived-connections/
* https://stackoverflow.com/questions/14388706/how-do-so-reuseaddr-and-so-reuseport-differ

golang reuseport
* https://pkg.go.dev/github.com/libp2p/go-reuseport

`libtimewait` works by intercepting calls to `connect(2)` using
`LD_PRELOAD`. Before `connect(2)`ing, `setsockopt(2)` is called using
the configured socket options.

`libtimewait_listen` works by intercepting calls to `listen(2)`
using `LD_PRELOAD`. Socket options are set when the application calls
`listen(2)`. Socket options for `accept`(2)'ed fd's are inherited from
the listener socket.

`libtimewait_socket` intercepts calls to `socket(2)` using
`LD_PRELOAD`. Socket options are set after the application calls
`socket(2)`.

libtimewait requires the program to be dynamically linked and will
not work with statically linked programs or programs that directly
make syscalls.

libtimewait is a small LD_PRELOAD library to enable SO_REUSEADDR and
SO_REUSEPORT on any sockets opened by dynamically linked applications,
either outbound (connect(2), using libtimewait.so), inbound (listen(2),
using libtimewait_listen.so) or any INET/INET6 socket (socket(2),
using libtimewait_socket.so).

The typical situation is that a server needs to quickly restart and
rebind to the same address and port without waiting for TIME_WAIT
connections to expire. Enabling SO_REUSEADDR allows reuse of local
addresses. Enabling SO_REUSEPORT allows multiple sockets to bind to
the same port.

# ENVIRONMENT VARIABLES

Setting options to 0 will use the system default.

## COMMON VARIABLES

`LIBTIMEWAIT_DEBUG`
: Write errors to stdout (default: disabled). Set to any value to enable.

```
LIBTIMEWAIT_DEBUG=1
```

## libtimewait, libtimewait_socket

`TCP_SYNCNT`
: Number of SYN packets sent on `connect(2)` (default: 0 (system default))

## libtimewait_listen, libtimewait_socket

`TCP_DEFER_ACCEPT`
: Avoid waking up the server process until data is sent by the
client. Number of seconds to wait (default: 0 (disabled))

# EXAMPLES

## netcat

```shell
## Use strace to verify setsockopt(2) is called

# run in a shell
LD_PRELOAD=libtimewait_listen.so strace -e trace=network nc -k -l 9090

# in another shell
LD_PRELOAD=libtimewait.so strace -e trace=network nc 127.0.0.1 9090

# in another shell
LD_PRELOAD=libtimewait_socket.so strace -e trace=network nc 127.0.0.1 9090
```

### `TCP_USER_TIMEOUT`

```shell
$ time LD_PRELOAD=libtimewait.so TCP_USER_TIMEOUT=5000 nc -vvv 8.8.8.8 22
nc: connect to 8.8.8.8 port 22 (tcp) failed: Connection timed out

real    0m5.038s
user    0m0.005s
sys     0m0.008s
```

Using the settings described in https://tech.instacart.com/the-vanishing-thread-and-postgresql-tcp-connection-parameters-93afc0e1208c:


# SEE ALSO

*socket*(7), *tcp*(7), *connect*(2), *listen*(2), *accept*(2), *setsockopt*(2)
