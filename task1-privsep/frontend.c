/*
 * frontend.c
 * -----------
 * Role: UNTRUSTED, UNPRIVILEGED process.
 * - Talks to the user (reads a username/password).
 * - Has NO access to sensitive validation data.
 * - Sends the credentials to backend.c over a UNIX domain socket.
 * - Waits for a pass/fail result and reports it to the user.
 *
 * This is the "attack surface" process. If this process is compromised,
 * the attacker should gain nothing more than the ability to submit
 * login attempts.
 *
 * STATUS: skeleton — fill in the TODOs.
 */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "./authsock"   // must match backend.c
#define BUF_SIZE 256

int main(void) {
    char username[BUF_SIZE];
    char password[BUF_SIZE];

    printf("Username: ");
    // TODO: read username safely with fgets(), strip newline

    printf("Password: ");
    // TODO: read password safely with fgets(), strip newline
    // (later: consider disabling terminal echo for the password using termios)

    // TODO 1: create a UNIX domain socket
    //   int sock = socket(AF_UNIX, SOCK_STREAM, 0);

    // TODO 2: set up sockaddr_un with sun_path = SOCKET_PATH
    //   struct sockaddr_un addr;
    //   memset(&addr, 0, sizeof(addr));
    //   addr.sun_family = AF_UNIX;
    //   strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // TODO 3: connect() to backend.c's listening socket

    // TODO 4: send username + password to backend
    //   (think about a simple framed message: "user\npass\n")

    // TODO 5: recv() the backend's pass/fail response

    // TODO 6: clear password from memory before exiting
    //   (research why memset() alone may not be reliable — see
    //   investigation question 11 in the brief, and look up memset_s /
    //   explicit_bzero)

    // TODO 7: print result to user, close socket

    return 0;
}