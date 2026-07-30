#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

#define SOCKET_PATH "/tmp/authsock"
#define BUF_SIZE 256
#define UNPRIV_UID 1000
#define UNPRIV_GID 1000

#define DUMMY_USER "admin"
#define DUMMY_PASS "SecureOS2026!"

static void drop_privileges(void) {
    if (setresgid(UNPRIV_GID, UNPRIV_GID, UNPRIV_GID) != 0) {
        perror("setresgid failed");
        exit(1);
    }
    if (setresuid(UNPRIV_UID, UNPRIV_UID, UNPRIV_UID) != 0) {
        perror("setresuid failed");
        exit(1);
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
    int listen_sock;
    struct sockaddr_un addr;

    printf("[Backend] starting, euid=%d\n", geteuid());

    unlink(SOCKET_PATH);
    listen_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listen_sock == -1) {
        perror("socket");
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (bind(listen_sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        return 1;
    }

    if (listen(listen_sock, 5) == -1) {
        perror("listen");
        return 1;
    }

    printf("[Backend] listening on %s\n", SOCKET_PATH);

    while (1) {
        int client = accept(listen_sock, NULL, NULL);
        if (client == -1) {
            perror("accept");
            continue;
        }

        struct ucred peer_cred;
        socklen_t opt_len = sizeof(struct ucred);
        if (getsockopt(client, SOL_SOCKET, SO_PEERCRED, &peer_cred, &opt_len) == 0) {
            printf("[Backend Security Log] Connected PID: %d, UID: %d\n", peer_cred.pid, peer_cred.uid);
        }

        char buf[BUF_SIZE];
        memset(buf, 0, sizeof(buf));
        ssize_t n = read(client, buf, sizeof(buf) - 1);
        
        int validated = 0;
        if (n > 0) {
            buf[n] = '\0';
            char *username = strtok(buf, ":");
            char *password = strtok(NULL, ":");

            if (username && password) {
                if (strcmp(username, DUMMY_USER) == 0 && strcmp(password, DUMMY_PASS) == 0) {
                    validated = 1;
                }
            }
        }

        memset(buf, 0, sizeof(buf));
        
        // Drop privileges inside the connection execution thread sequence
        drop_privileges();

        if (validated) {
            write(client, "SUCCESS", 7);
        } else {
            write(client, "FAILED", 6);
        }

        close(client);
    }

    close(listen_sock);
    unlink(SOCKET_PATH);
    return 0;
}
