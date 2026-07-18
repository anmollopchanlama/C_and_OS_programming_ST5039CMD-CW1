#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

#define SOCKET_PATH "./authsock"
#define BUF_SIZE 256
#define UNPRIV_UID 1000
#define UNPRIV_GID 1000

static void drop_privileges(void) {
    if (setresgid(UNPRIV_GID, UNPRIV_GID, UNPRIV_GID) != 0) {
        perror("setresgid failed"); exit(1);
    }
    if (setresuid(UNPRIV_UID, UNPRIV_UID, UNPRIV_UID) != 0) {
        perror("setresuid failed"); exit(1);
    }
    uid_t ruid, euid, suid;
    getresuid(&ruid, &euid, &suid);
    printf("[Backend] post-drop UIDs -> real:%d effective:%d saved:%d\n", ruid, euid, suid);

    if (setuid(0) == 0) {
        fprintf(stderr, "[Backend] CRITICAL: privilege drop was reversible!\n");
        exit(1);
    } else {
        printf("[Backend] confirmed: cannot regain root\n");
    }
}

int main(void) {
    printf("[Backend] starting, euid=%d\n", geteuid());
    if (geteuid() != 0) {
        fprintf(stderr, "[Backend] warning: not root, drop test meaningless. Run via sudo.\n");
    }
    drop_privileges();

    unlink(SOCKET_PATH);
    int listen_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listen_sock == -1) { perror("socket"); return 1; }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (bind(listen_sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind"); return 1;
    }
    if (listen(listen_sock, 5) == -1) { perror("listen"); return 1; }

    printf("[Backend] listening on %s (unprivileged)\n", SOCKET_PATH);

    while (1) {
        int client = accept(listen_sock, NULL, NULL);
        if (client == -1) { perror("accept"); continue; }

        char buf[BUF_SIZE];
        ssize_t n = read(client, buf, sizeof(buf) - 1);
        if (n > 0) {
            buf[n] = '\0';
            printf("[Backend] received: %s\n", buf);
            const char *reply = "received (validation not yet implemented)";
            write(client, reply, strlen(reply));
        }
        close(client);
    }

    close(listen_sock);
    return 0;
}
