#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "timewait.h"

void _init(void);
static int (*sys_connect)(int sockfd, const struct sockaddr *addr,
                          socklen_t addrlen);
#pragma GCC diagnostic ignored "-Wpedantic"
int __attribute__((visibility("default")))
connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
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
  sys_connect = dlsym(RTLD_NEXT, "connect");
#pragma GCC diagnostic warning "-Wpedantic"
  err = dlerror();

  if (err != NULL)
    (void)fprintf(stderr, "libtimewait:dlsym (connect): %s\n", err);
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
  int oerrno = errno;

  (void)timewait(sockfd, &opt);
  errno = oerrno;
  return sys_connect(sockfd, addr, addrlen);
}
