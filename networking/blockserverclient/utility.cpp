#include "utility.h"

void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(reinterpret_cast<sockaddr_in*>(sa)->sin_addr);
  }

  return &(reinterpret_cast<sockaddr_in6*>(sa)->sin6_addr);
}
