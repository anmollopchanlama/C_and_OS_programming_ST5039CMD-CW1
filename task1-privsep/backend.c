/*
 * backend.c
 * ----------
 * Role: PRIVILEGED validation process.
 * - Starts with elevated privileges (run this as setuid-root, or launched
 *   by root, depending on how you demo it).
 * - Listens on a UNIX domain socket for credential-check requests from
 *   frontend.c.
 * - Performs the actual validation (e.g. checks against a stored
 *   hash — do NOT use real /etc/shadow for coursework, use your own
 *   dummy credentials file).
 * - CRITICAL: after any privileged operation is done, permanently drops
 *   privileges with setresuid() before doing anything else (like sending
 *   the response back), so a compromised backend can't be abused to do
 *   more privileged actions later.
 *
 * STATUS: skeleton — fill in the TODOs.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/authsock"
#define BUF_SIZE 256

// TODO: write a helper to print current uid/euid/suid so you can screenshot
// this for your report as evidence of the privilege drop, e.g.:
// void print_ids(const char *label) {
//     printf("[%s] uid=%d euid=%d\n", label, getuid(), geteuid());
// }

int validate_credentials(const char *user, const char *pass) {
    // TODO: replace with real check against a local dummy credential
    // store (do NOT hardcode a real system password).
    return 0; // placeholder: always fail for now
}

int main(void) {
    // TODO 0: print_ids("startup") — show we start privileged

    // TODO 1: create + bind() + listen() on a UNIX domain socket at
    // SOCKET_PATH (remember to unlink() any stale socket file first)

    // TODO 2: accept() a connection from frontend.c

    // TODO 3: recv() username + password

    // TODO 4: run validate_credentials() WHILE STILL PRIVILEGED
    // (this is the part that conceptually needs elevated access —
    // e.g. reading a protected credentials file)

    // TODO 5: DROP PRIVILEGES PERMANENTLY
    //   if (setresuid(getuid(), getuid(), getuid()) != 0) {
    //       perror("setresuid failed");
    //       // must treat this as fatal - never continue privileged
    //   }
    //   print_ids("after drop");

    // TODO 6: verify the drop actually happened and is irreversible
    //   e.g. try seteuid(0) again here and confirm it FAILS — log this,
    //   it's good evidence for your report

    // TODO 7: send pass/fail result back to frontend over the socket

    // TODO 8: clear password buffer from memory (see investigation Q10-12)

    // TODO 9: close socket, unlink SOCKET_PATH, exit

    return 0;
}