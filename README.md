# UNIX System Programming Architecture: Sandbox & Privilege Separation Suite

An automated, defensive software framework implementing low-level Linux security primitives for privilege separation, identity verification, and multi-threaded sandbox containment. This suite satisfies the strict engineering and evaluation criteria for the Programming and Operating Systems module (STP503CA9).

##  Repository Architecture
```text
.
├── task1-privsep/
│   ├── frontend.c              # Client interface managing unprivileged token inputs
│   └── backend.c               # Decoupled SetUID root daemon executing validation
├── task2-sandbox/
│   └── sandbox.c               # Multi-threaded supervisor tracking child runtime metrics
└── Makefile                    # Automated multi-task compilation engine
```

## 🛠️ Automated Compilation & Build Strategy
The project utilizes a centralized `Makefile` configuring strict warning profiles (`-Wall -Wextra`), high-level optimizations (`-O2`), and multi-threading options (`-pthread`):

```bash
# Compile all software components simultaneously
make
```

## 🧪 Execution & Verification Workflow

### Task 1: Privilege Separation Boundary Validation
1. **Provision Elevated Environment:** Allocate temporary root capabilities to the backend daemon to test permanent identity transitions:
   ```bash
   make provision_privileges
   ```
2. **Execute Interface:** Launch the unprivileged client landing application:
   ```bash
   ./task1-privsep/frontend
   ```
3. **Evaluation Scenarios:**
   * **Test Case A (Valid Input):** Submit `SystemOS_SecuredP@ss` to trigger dynamic identity matching via `SO_PEERCRED`, authenticate successfully, and permanently drop privileges to a non-zero ID across all kernel slots using `setresuid()`.
   * **Test Case B (Invalid Input):** Submit any alternate token to verify robust, runtime access-denied state handling.

### Task 2: Multi-Threaded Sandbox Monitoring
1. **Launch Sandbox Container:** Execute the runtime monitor thread loop against a system execution target:
   ```bash
   ./task2-sandbox/sandbox /usr/bin/sleep 10
   ```
2. **Telemetry Evaluation:** Observe the concurrent tracking thread continuously poll the target via `/proc/[PID]/statm`. The engine handles real-time resource violations and execution timeouts independently by issuing uncatchable kernel containment signaling flags (`SIGKILL`).

##  Core Security Implementations
* **Identity Auditing:** Dynamic credential checking via `SO_PEERCRED` on low-level `AF_UNIX` local sockets.
* **Privilege Shedding:** Irreversible identity mitigation stripping Real, Effective, and Saved UIDs simultaneously via `setresuid()`.
* **Memory Cleansing:** Forced compiler-optimization bypasses utilizing specialized `volatile` tracking loops for secure password buffer scrubbing.
