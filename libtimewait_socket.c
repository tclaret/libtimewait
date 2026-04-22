#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include "timewait.h"

void _init(void);
static int (*sys_socket)(int domain, int type, int protocol);
#pragma GCC diagnostic ignored "-Wpedantic"
int __attribute__((visibility("default")))
socket(int domain, int type, int protocol);
#pragma GCC diagnostic warning "-Wpedantic"

static timewait_t opt = {0};

void _init(void) {
  const char *err;

  char *env_debug;

  env_debug = getenv("LIBTIMEWAIT_DEBUG");

  timewait_init(&opt);

  if (env_debug)
    opt.debug = 1;

#pragma GCC diagnostic ignored "-Wpedantic"
  sys_socket = dlsym(RTLD_NEXT, "socket");
#pragma GCC diagnostic warning "-Wpedantic"
  err = dlerror();

  if (err != NULL)
    (void)fprintf(stderr, "libtimewait:dlsym (socket): %s\n", err);
}

int socket(int domain, int type, int protocol) {
  int fd;

  fd = sys_socket(domain, type, protocol);

  switch (domain) {
  case AF_INET:
    break;
  case AF_INET6:
    break;
  default:
    return fd;
  }

  if (fd == -1) {
    return fd;
  }

  (void)timewait(fd, &opt);
  errno = 0;

  return fd;
}
