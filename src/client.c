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
