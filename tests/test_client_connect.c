#include "unity.h"
#include "../src/client.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

/* Test client.c using a tiny local TCP server independent of the internet */
typedef struct
{
    int server_fd;
} server_ctx_t;

static void *server_thread(void *arg)
{
    server_ctx_t *ctx = (server_ctx_t *)arg;

    // Accept one connection then close
    int client_fd = accept(ctx->server_fd, NULL, NULL);
    if (client_fd >= 0)
    {
        close(client_fd);
    }

    return NULL;
}

// Start a server on localhost with an ephemeral port. Returns allocated port
static char *start_local_server(int *out_server_fd, pthread_t *out_tid)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    TEST_ASSERT_TRUE(fd >= 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(0); // ask OS for any free port

    int yes = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    TEST_ASSERT_EQUAL(0, bind(fd, (struct sockaddr *)&addr, sizeof(addr)));
    TEST_ASSERT_EQUAL(0, listen(fd, 1));

    // Read back the chosen port
    struct sockaddr_in bound;
    socklen_t len = sizeof(bound);
    TEST_ASSERT_EQUAL(0, getsockname(fd, (struct sockaddr *)&bound, &len));

    int port = ntohs(bound.sin_port);

    server_ctx_t *ctx = malloc(sizeof(server_ctx_t));
    TEST_ASSERT_NOT_NULL(ctx);
    ctx->server_fd = fd;

    TEST_ASSERT_EQUAL(0, pthread_create(out_tid, NULL, server_thread, ctx));

    // Return port as string
    char *port_str = malloc(16);
    TEST_ASSERT_NOT_NULL(port_str);
    snprintf(port_str, 16, "%d", port);

    *out_server_fd = fd;
    return port_str;
}

void setUp(void) {}
void tearDown(void) {}

void test_client_connects_to_localhost_port(void)
{
    pthread_t tid;
    int server_fd = -1;

    char *port = start_local_server(&server_fd, &tid);

    int client_fd = client_connect("127.0.0.1", port);
    TEST_ASSERT_TRUE(client_fd >= 0);

    close(client_fd);

    // Wait for server thread to accept + exit
    pthread_join(tid, NULL);
    close(server_fd);

    free(port);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_client_connects_to_localhost_port);
    return UNITY_END();
}
