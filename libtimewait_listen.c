#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "timewait.h"

void _init(void);
static int (*sys_listen)(int sockfd, int backlog);
#pragma GCC diagnostic ignored "-Wpedantic"
int __attribute__((visibility("default"))) listen(int sockfd, int backlog);
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
  sys_listen = dlsym(RTLD_NEXT, "listen");
#pragma GCC diagnostic warning "-Wpedantic"
  err = dlerror();

  if (err != NULL)
    (void)fprintf(stderr, "libtimewait:dlsym (listen): %s\n", err);
}

int listen(int sockfd, int backlog) {
  int oerrno = errno;

  (void)timewait(sockfd, &opt);
  errno = oerrno;
  return sys_listen(sockfd, backlog);
}
