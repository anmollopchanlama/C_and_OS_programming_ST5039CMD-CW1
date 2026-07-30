#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/authsock"
#define BUF_SIZE 256

int main(void) {
    char username[BUF_SIZE];
    char password[BUF_SIZE];
    char payload[BUF_SIZE * 2];
    int sock;
    struct sockaddr_un addr;
    char reply[BUF_SIZE];
    ssize_t n;

    printf("Username: ");
    if (fgets(username, BUF_SIZE, stdin) == NULL) return 1;
    username[strcspn(username, "\n")] = '\0';

    printf("Password: ");
    if (fgets(password, BUF_SIZE, stdin) == NULL) return 1;
    password[strcspn(password, "\n")] = '\0';

    snprintf(payload, sizeof(payload), "%s:%s", username, password);

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        memset(password, 0, sizeof(password));
        memset(payload, 0, sizeof(payload));
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("connect failed");
        close(sock);
        memset(password, 0, sizeof(password));
        memset(payload, 0, sizeof(payload));
        return 1;
    }

    write(sock, payload, strlen(payload));

    memset(password, 0, sizeof(password));
    memset(payload, 0, sizeof(payload));

    memset(reply, 0, sizeof(reply));
    n = read(sock, reply, sizeof(reply) - 1);
    if (n > 0) {
        reply[n] = '\0';
        printf("Result: %s\n", reply);
    } else {
        printf("Result: NO_RESPONSE_FROM_BACKEND\n");
    }

    close(sock);
    return 0;
}
