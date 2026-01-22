#ifndef CLIENT_H
#define CLIENT_H

typedef struct
{
    const char *hostname;
    const char *port; // will keep as a string i. e. "8080"
} endpoint_t;

// Returns 0 on success, nonzero on error
int parse_endpoint(int argc, char *argvp[], endpoint_t *out);

// Returns a connected socket file descriptor, or -1 on error
int client_connect(const char *hostname, const char *port);

#endif
