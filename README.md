<div align="center">

# 🏓 ft_ping

### *Recoding `ping` — a dive into ICMP and IPv4 networking*

[![42](https://img.shields.io/badge/42-Project-000000?style=for-the-badge&logo=42&logoColor=white)](https://42.fr)
[![Language](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![RFC](https://img.shields.io/badge/RFC-792-blue?style=for-the-badge)](https://www.rfc-editor.org/rfc/rfc792)
[![Reference](https://img.shields.io/badge/Reference-inetutils--2.0-green?style=for-the-badge)](https://www.gnu.org/software/inetutils/)

> 🌐 Test the reachability of a host on an IP network and measure the round-trip time (RTT).

</div>

---

## 📖 Table of contents

- [🎯 Goal](#-goal)
- [💡 What is `ping`?](#-what-is-ping)
- [📡 ICMP & RFC 792](#-icmp--rfc-792)
- [⏱️ RTT & statistics](#️-rtt--statistics)
- [🏗️ Project requirements](#️-project-requirements)
- [⚙️ Options](#️-options)
- [🚀 Usage](#-usage)
- [🔧 Compilation](#-compilation)
- [📋 Evaluation criteria](#-evaluation-criteria)
- [🎁 Bonus part](#-bonus-part)
- [🧩 Challenges & resolutions](#-challenges--resolutions)
- [📚 References](#-references)

---

## 🎯 Goal

**ft_ping** is a 42 project that consists in **recoding the `ping` command** in **C**, without calling the system binary or using its sources.

The goal is to deeply understand:

- 🔹 The **ICMP** protocol (Internet Control Message Protocol)
- 🔹 Sending and receiving **IPv4 packets**
- 🔹 Measuring **round-trip time** (Round-Trip Time)
- 🔹 Handling **network errors** and command-line options

---

## 💡 What is `ping`?

`ping` is a command used to **test the reachability** of another host over an IP network. It sends **ICMP Echo Request** packets and waits for **ICMP Echo Reply** packets in return.

```
  ┌──────────┐                              ┌──────────┐
  │  Host A  │  ──── ICMP Echo Request ───► │  Host B  │
  │ (ft_ping)│  ◄─── ICMP Echo Reply  ───── │          │
  └──────────┘                              └──────────┘
         │                                          │
         └──────────── RTT = round-trip time ───────┘
```

In practice, for each reply `ping` displays:

- 📦 The number of bytes received
- 🏷️ The source address of the reply
- 🔢 The ICMP sequence number
- ⏱️ The **TTL** (Time To Live) of the IP packet
- ⚡ The **response time** in milliseconds

---

## 📡 ICMP & RFC 792

**ICMP** (*Internet Control Message Protocol*) is defined in **[RFC 792](https://www.rfc-editor.org/rfc/rfc792)** (September 1981, J. Postel, ISI). It is an integral part of IP and **must be implemented by every IP module**.

### 🧩 Role of ICMP

ICMP provides **feedback** about problems encountered while processing IP datagrams:

| Type | Name | Description |
|:----:|:---|:---|
| `0` | Echo Reply | Reply to an echo request |
| `3` | Destination Unreachable | Destination unreachable |
| `4` | Source Quench | Congestion — slow down transmission |
| `5` | Redirect | A shorter route is available |
| `8` | **Echo** | **Echo request (used by `ping`)** |
| `11` | Time Exceeded | TTL expired in transit |
| `12` | Parameter Problem | Error in the IP header |
| `13` | Timestamp | Timestamp request |
| `14` | Timestamp Reply | Timestamp reply |

> ⚠️ No ICMP message is sent **about** other ICMP messages (avoids infinite regression).

### 🏓 Echo / Echo Reply message format

This is the core of `ping`. ICMP packet structure (RFC 792):

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     Type      |     Code      |          Checksum             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           Identifier          |        Sequence Number        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     Data ...
+-+-+-+-+-+-+-+-+
```

| Field | Echo Request | Echo Reply |
|:---|:---:|:---:|
| **Type** | `8` | `0` |
| **Code** | `0` | `0` |
| **Checksum** | 16-bit one's complement | same |
| **Identifier** | Identifies the session (like a TCP/UDP port) | returned as-is |
| **Sequence Number** | Incremented on each request | returned as-is |
| **Data** | Arbitrary data | **must be returned unchanged** |

### 🔐 ICMP checksum computation

The checksum is the **16-bit one's complement** of the one's complement sum of all 16-bit words of the ICMP message, starting from the **Type** field. The checksum field is set to **zero** while computing.

### 🌍 Associated IP header

Each ICMP message is encapsulated in an IP datagram with, among others:

| IP field | ICMP value |
|:---|:---|
| **Version** | `4` |
| **Protocol** | `1` (ICMP) |
| **TTL** | Decremented at each hop — expired → ICMP Time Exceeded (type 11) |
| **Header Checksum** | IP header checksum |

---

## ⏱️ RTT & statistics

### What is RTT?

**RTT** (*Round-Trip Time*) = round-trip time of an ICMP packet, in milliseconds.

```
  Send Echo Request            Receive Echo Reply
        |                              |
        t1                             t2
        |<-------- RTT = t2 - t1 ----->|
```

In `ft_ping`, the timestamp is stored in the packet on send (`gettimeofday`), then recomputed on receive via `get_ms()`.

---

### `rtt_t` structure

The `rtt_t` structure (in `src/core/rtt.c`) accumulates statistics **without storing every RTT in memory**:

| Field | Role |
|:---|:---|
| `min` | Smallest observed RTT |
| `max` | Largest observed RTT |
| `sum` | Sum of all RTTs → used to compute the **average** |
| `sumsq` | Sum of squares of all RTTs → used to compute the **mdev** |
| `count` | Number of recorded RTTs |
| `elapsed_ms` | Total ping duration (`time 1001ms` line) |
| `start` | Timestamp of the start of the ping |

---

### Formulas used

#### Average (avg)

```
avg = sum / count
```

#### Variance

```
variance = (sumsq / count) - (avg × avg)
```

Expanded:

```
variance = (sumsq / count) - (sum / count)²
```

#### mdev (mean deviation)

The **mdev** is the **standard deviation** of the RTTs. It measures network **stability**:

- **Low** mdev → stable response times
- **High** mdev → unstable response times (jitter, congestion…)

```
mdev = sqrt(variance)
```

#### Line printed at the end

```
rtt min/avg/max/mdev = 11.800/12.050/12.300/0.250 ms
```

---

### Numeric example

Received RTTs: `10 ms`, `12 ms`, `14 ms`, `50 ms`

**Step 1 — Accumulation on each packet**

```
sum   = 10 + 12 + 14 + 50       = 86
sumsq = 10² + 12² + 14² + 50²   = 100 + 144 + 196 + 2500 = 2940
count = 4
min   = 10
max   = 50
```

**Step 2 — Final computation**

```
avg      = 86 / 4           = 21.5 ms
variance = 2940/4 - 21.5²   = 735 - 462.25 = 272.75
mdev     = sqrt(272.75)     ≈ 16.516 ms
```

Result:

```
rtt min/avg/max/mdev = 10.000/21.500/50.000/16.516 ms
```

The mdev is high because the `50 ms` packet strongly skews the average.

---

### Why `sumsq` without storing everything in memory?

**Naive approach**: keep an array `[10, 12, 14, 50]` then compute the mean of deviations.

**Ping approach**: on each packet, only update `sum` and `sumsq`.

Both methods give the **same result** thanks to this mathematical identity:

```
variance = mean of (xi - avg)²
         = mean of (xi²) - avg²
         = (sumsq / n) - (sum / n)²
```

So `sum` + `sumsq` hold all the information needed for the average and mdev, in **constant O(1) memory**.

---

### Mapping to the code

```c
// On each echo reply received (rtt_add)
r->sum   += ms;
r->sumsq += ms * ms;
r->count++;

// At the end (rtt_show)
avg      = r->sum / r->count;
variance = (r->sumsq / r->count) - (avg * avg);
mdev     = sqrt(variance);
```

---

### RTT references

| Source | Link |
|:---|:---|
| GNU inetutils `ping.c` | [gnu.org/software/inetutils](https://www.gnu.org/software/inetutils/) |
| iputils `ping_common.c` | [github.com/iputils/iputils](https://github.com/iputils/iputils/blob/master/ping_common.c) |
| mdev explanation | [serverfault.com — What does mdev mean in ping](https://serverfault.com/questions/333116/what-does-mdev-mean-in-ping8) |

---

## 🏗️ Project requirements

### 📌 General rules

| Rule | Detail |
|:---|:---|
| 🗣️ Language | **C** only |
| 📦 Deliverable | A **Makefile** with the usual rules |
| 🏷️ Binary | Must be named **`ft_ping`** |
| 📚 libc | The entire **libc** is allowed |
| 🚫 Forbidden | Calling the system `ping` or using its sources |
| 🛡️ Robustness | No unexpected crash (segfault, bus error, double free…) |
| 📏 Reference | Behavior modeled on **inetutils-2.0** (`ping -V`) |

### ✅ Mandatory part

- [ ] Executable named `ft_ping`
- [ ] Handling of options **`-v`** and **`-?`**
- [ ] Support for an **IPv4** address or a **hostname** as argument
- [ ] Handling of **FQDNs** without DNS resolution on the returned packet
- [ ] Option **`-v`**: display results when a packet-related problem or error occurs (without forcing the program to stop — changing the TTL can help trigger an error)
- [ ] The **`printf`** family is allowed

---

## ⚙️ Options

### 🔴 Mandatory

| Option | Description |
|:---:|:---|
| `-v` | Verbose mode — shows details on packet errors or problems |
| `-?` | Shows help |

### 🟡 Bonus (if the mandatory part is perfect)

| Option | Description |
|:---:|:---|
| `-f` | Flood ping |
| `-l` | Preload |
| `-n` | No DNS resolution |
| `-w` | Stop after N seconds (`--timeout`) |
| `-W` | Timeout per reply |
| `-p` | Pattern to send in packets |
| `-r` | Record route |
| `-s` | Packet size |
| `-T` | IP options (TOS) |
| `--ttl` | Set the TTL |
| `--ip-timestamp` | IP timestamp option |

> ℹ️ Options `-V`, `--usage` and `--echo` **do not count** as bonus.  
> Two flags for the same feature (e.g. `-t` / `--type`) count only **once**.

---

## 🚀 Usage

```bash
# Basic ping to an IPv4 address
./ft_ping 8.8.8.8

# Ping to a hostname
./ft_ping google.com

# Verbose mode (also shows ICMP errors)
./ft_ping -v 127.0.0.1

# Force a Time Exceeded error to check the -v dump
./ft_ping -v -c 1 --ttl 1 8.8.8.8

# Several hosts, one statistics block each
./ft_ping -c 2 127.0.0.1 8.8.8.8

# Help (long list) and short synopsis
./ft_ping -?
./ft_ping --usage
```

### 📤 Expected output example (inetutils-2.0)

Note the inetutils specifics: `): 56 data bytes`, a sequence starting at
**0**, and `N packets received` (iputils, the other common `ping`, differs on
all three).

```
PING google.com (142.250.185.78): 56 data bytes
64 bytes from 142.250.185.78: icmp_seq=0 ttl=116 time=12.300 ms
64 bytes from 142.250.185.78: icmp_seq=1 ttl=116 time=11.800 ms
^C
--- google.com ping statistics ---
2 packets transmitted, 2 packets received, 0% packet loss
rtt min/avg/max/mdev = 11.800/12.050/12.300/0.250 ms
```

The last line is the one deviation the subject explicitly tolerates
(inetutils writes `round-trip min/avg/max/stddev`).

---

## 🔧 Compilation

```bash
make        # Compile ft_ping
make clean  # Clean object files
make fclean # Clean everything
make re     # Full rebuild
```

### 🖥️ Required environment

- 🐧 Virtual machine **Debian ≥ 7.0**
- 🐧 Linux kernel **> 3.14**
- 🛠️ C build tools (`gcc`, `make`, …)

---

## 📋 Evaluation criteria

| Criterion | Tolerance |
|:---|:---|
| 📝 Output indentation | **Identical** to inetutils-2.0 (except RTT line and reverse DNS resolution) |
| ⏱️ Packet reception | Delay of **± 30 ms** accepted |
| 🧪 Bonus part | Evaluated **only** if the mandatory part is **perfect** |
| 📂 Git repository | Only the repository content is evaluated during the defense |

---

## 🎁 Bonus part

The bonus part is evaluated only if **the entire** mandatory part works **without any malfunction**. If there is any defect on the mandatory part, the bonus is **not taken into account at all**.

---

## 🧩 Challenges & resolutions

This section summarizes the main difficulties encountered while implementing `ft_ping`, and how they were solved.

### 1. Raw sockets & privileges

**Challenge:** ICMP Echo requires a raw socket (`SOCK_RAW` / `IPPROTO_ICMP`). Creating it fails without sufficient privileges (`Operation not permitted`).

**Resolution:**
- Use `socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)`
- Run with `sudo`, or grant the binary `CAP_NET_RAW`
- Fail cleanly with a clear error when the socket cannot be created

### 2. Matching inetutils-2.0 output

**Challenge:** The subject requires output indentation identical to **inetutils-2.0**, except the RTT line and reverse DNS. Small differences (`PING` banner, stats line, verbose dumps) are easy to get wrong.

**Resolution:**
- Align the banner on `PING host (ip) 56(84) bytes of data.`
- Keep reply lines as `bytes from / icmp_seq / ttl / time`
- Print final stats with packet loss (and `time` when relevant)
- Never resolve reverse DNS on addresses taken from received packets (mandatory + `-n`)

### 3. RTT statistics without storing every sample

**Challenge:** Computing `min` / `avg` / `max` / `mdev` without keeping an array of all RTTs in memory.

**Resolution:**
- Accumulate `sum` and `sumsq` on each reply (`rtt_add`)
- Derive average and variance at the end:
  - `avg = sum / count`
  - `variance = (sumsq / count) - avg²`
  - `mdev = sqrt(variance)`
- Constant memory footprint `O(1)`, same approach as classic ping implementations

### 4. ICMP errors & verbose mode (`-v`)

**Challenge:** Besides Echo Reply, the program must handle error packets (Destination Unreachable, Time Exceeded, …), especially with `-v`, without crashing and without stopping the loop.

**Resolution:**
- Parse the outer IP/ICMP headers, then the embedded original datagram
- Filter with the Echo identifier (`getpid() & 0xFFFF`) to keep only our packets
- Print a short human-readable message
- With `-v`, dump the embedded IP/ICMP headers (inetutils-style)
- Use a low TTL (`--ttl`) to reliably trigger Time Exceeded during tests

### 5. Event loop: send every second + receive + signals

**Challenge:** Send one Echo Request per second, receive replies asynchronously, and stop cleanly on `Ctrl+C` or timeout (`-w`).

**Resolution:**
- `timerfd` for the 1-second interval
- `poll()` on the ICMP socket and the timer
- `SIGINT` / `SIGALRM` handlers to leave the loop and print statistics
- `-w` implemented with `alarm()` so the program exits after N seconds regardless of replies

### 6. Byte order & packet identity

**Challenge:** ICMP fields (`id`, `sequence`) and multi-byte values must be consistent on the wire; otherwise replies are ignored or sequences look wrong.

**Resolution:**
- Keep a stable Echo ID derived from the process PID
- Handle endianness of the sequence with `BIG16`
- Recompute the ICMP checksum after filling the packet (`checksum`)

### 7. CLI parsing edge cases

**Challenge:** Options such as `-?`, invalid values (`-c abc`, `--ttl 999`), missing host, and `-V` must not crash or accidentally start a ping with a `NULL` host.

**Resolution:**
- Centralize parsing with `getopt_long`
- Validate numeric arguments via `parse_unumber` + range checks (TTL 1–255, timeout > 0)
- Special-case `-?` / `--help` / `--usage` so help always works (including under zsh, where `?` is a glob: use `'-?'`)
- Dedicated return codes: help → exit error path, version (`-V`) → exit 0 without starting the ping

### 8. Testing & CI

**Challenge:** Guaranteeing behavior without relying only on manual runs, and catching regressions early.

**Resolution:**
- Unit tests in C for pure functions (`checksum`, `parse_unumber`, `rtt_*`, `resolve_host`, CLI parsing)
- Integration tests with `fork` + `pipe` comparing structural output against system `ping`
- `make test` rebuilds objects after `make clean` when needed
- GitHub Actions CI pipeline: **build** + **unit tests** on every push / pull request

---

## 📚 References

| Document | Link |
|:---|:---|
| 📄 RFC 792 — ICMP | [rfc-editor.org/rfc/rfc792](https://www.rfc-editor.org/rfc/rfc792) |
| 📄 RFC 791 — IP | [rfc-editor.org/rfc/rfc791](https://www.rfc-editor.org/rfc/rfc791) |
| 🔧 inetutils-2.0 (ping reference) | [gnu.org/software/inetutils](https://www.gnu.org/software/inetutils/) |
| 📋 42 ft_ping subject | `subjects/en.subject.pdf` |

---

<div align="center">

### 🏓 *« Are you there? »* — *« Yes, in 12.3 ms. »*

*Project completed as part of the **42** curriculum*

</div>
