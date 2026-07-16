# ST5039CMD - CW1 (Programming and Operating Systems)

Coursework repo for Task 1 (Privilege Separation) and Task 2 (Malware Sandbox).

## Status
- [x] Repo scaffolded, design planned
- [ ] Task 1: frontend.c — socket connect + send (in progress)
- [ ] Task 1: backend.c — socket listen + validate + privilege drop (in progress)
- [ ] Task 1: investigation questions written up
- [ ] Task 2: sandbox.c
- [ ] Task 2: test binaries + logs
- [ ] Task 2: investigation questions written up
- [ ] Final report (references + formatting)

## Structure
- `task1-privsep/` — frontend.c, backend.c (see file headers for design notes)
- `task2-sandbox/` — sandbox controller (coming next)

## How to build (Task 1, once filled in)
```
gcc -o frontend frontend.c
gcc -o backend backend.c
```

## Notes to self
Started: 16 Jul 2026. Learning C alongside implementation — see TODOs in
each file for what's left to implement and why each piece matters.