#include "client.h"

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int parse_endpoint(int argc, char *argv[], endpoint_t *out)
{
    if (!out) return 1;
    if (argc !=3) return 2; // need program + hostname + port

    if (argv[1] == NULL || argv[2] == NULL) return 3;
    if (strlen(argv[1]) == 0 || strlen(argv[2]) == 0) return 4;

    // access and assign the members of the endpoint_t struct
    out->hostname = argv[1];
    out->port = argv[2];
    return 0;
}


int client_connect(const char *hostname, const char *port)
{
    if (!hostname || !port) return -1;

    /* Use addrinfo linked-list from netdb header for
        protocol independent host information. Iterate through
        the candidates, testing each address till one works. */
    struct addrinfo hints;
    struct addrinfo *res = NULL; // pointer to head of linked-list

    // zero-initialize hints so all unspecified fields are 0/NULL
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;     // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream socket

    int gai = getaddrinfo(hostname, port, &hints, &res);
    if (gai != 0) return -1;

    int fd = -1;
    // Try each candidate address until one connect succeeds
    for (struct addrinfo *p = res; p != NULL; p = p->ai_next)
    {
        fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (fd < 0) continue;

        if (connect(fd, p->ai_addr, p->ai_addrlen) == 0)
        {
            freeaddrinfo(res);
            return fd; // Success!
        }

        close(fd);
        fd = -1; // this candidate address didn't work. keep trying
    }

    freeaddrinfo(res);
    return -1;
}
