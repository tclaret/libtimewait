#include "timewait.h"

void timewait_init(timewait_t *opt) { opt->debug = 0; }

int timewait(int sockfd, timewait_t *opt) {
  int enable = 1;

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) <
      0) {
    if (opt->debug)
      (void)fprintf(stderr, "libtimewait:setsockopt(SO_REUSEADDR, 1): %s\n",
                    strerror(errno));
    return -1;
  }

#ifdef SO_REUSEPORT
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(enable)) <
      0) {
    if (opt->debug)
      (void)fprintf(stderr, "libtimewait:setsockopt(SO_REUSEPORT, 1): %s\n",
                    strerror(errno));
    return -1;
  }
#endif

  return 0;
}
