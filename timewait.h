#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

typedef struct {
  int debug;
} timewait_t;

void timewait_init(timewait_t *opt);
int timewait(int sockfd, timewait_t *opt);
