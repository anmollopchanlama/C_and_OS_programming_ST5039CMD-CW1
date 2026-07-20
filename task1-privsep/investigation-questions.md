# Task 1 - Investigation Questions

## Q1: Why is it insecure for a single process to receive user input and access sensitive authentication data?

If one process handles both user input and the actual password check, it means untrusted input and sensitive data live in the same memory space. If that process gets compromised through something like a buffer overflow, the attacker automatically gets access to everything that process could touch, including the authentication data. Splitting these into two processes means even if the input-handling side is attacked, there's nothing sensitive there to steal.

## Q2: How can the principle of least privilege be enforced at the OS level using multiple processes rather than threads?

To fill in once Backend/Frontend fully work - will reference actual UID output here.

## Q3: What risks remain if privilege dropping is incorrectly implemented?

To fill in once Backend/Frontend fully work - will reference actual UID output here.
