# TARR Notes (ns-3 prototype)

## Files touched

- `src/internet/model/tcp-option-tarr.h`
- `src/internet/model/tcp-option-tarr.cc`
- `src/internet/model/tcp-option.h`
- `src/internet/model/tcp-option.cc`
- `src/internet/model/tcp-socket-base.h`
- `src/internet/model/tcp-socket-base.cc`
- `src/internet/CMakeLists.txt`

## Behavior implemented

- Added TCP option kind registration for TARR (`Kind=254`) in TCP option factory.
- Implemented TARR wire format support:
  - Capability format: `Len=4` (`Kind, Len, ExID`).
  - Request format: `Len=5` (`Kind, Len, ExID, V/R byte`).
  - ExID used: `0x00AC`.
- Added socket-level TARR configuration/state:
  - Enable/disable (`TarrEnabled`).
  - Requested ratio (`TarrRequestRatio`, range `0..127`).
  - Local capability advertisement toggle (`TarrAdvertisedCapability`).
  - Request-on-data policy toggle (`TarrRequestOnData`).
  - Runtime state for peer capability and applied peer ratio.
- Added send/receive integration in `TcpSocketBase`:
  - Sends TARR capability and requests.
  - Parses incoming TARR.
  - Applies peer request ratio to delayed-ACK threshold logic.
  - Handles `R=0` as immediate-ACK request.

## How to run

From repository root:

```bash
./ns3 build
./ns3 run "tcp-bulk-send --tracing=true --ns3::TcpSocketBase::TarrEnabled=1 --ns3::TcpSocketBase::TarrAdvertisedCapability=1 --ns3::TcpSocketBase::TarrRequestRatio=4"
```

Optional logging:

```bash
NS_LOG="TcpSocketBase=level_info|prefix_time:TcpOptionTARR=level_info|prefix_time" \
./ns3 run "tcp-bulk-send --tracing=true --ns3::TcpSocketBase::TarrEnabled=1 --ns3::TcpSocketBase::TarrAdvertisedCapability=1 --ns3::TcpSocketBase::TarrRequestRatio=4"
```

PCAPs are generated where the run command executes (or in your chosen output directory), with names like:

- `tcp-bulk-send-0-0.pcap`
- `tcp-bulk-send-1-0.pcap`

## What to look for in pcap (`fe04` / `fe05`, `R` byte)

- TARR capability announcement (`Len=4`):
  - Bytes: `fe 04 00 ac`
- TARR request (`Len=5`):
  - Bytes: `fe 05 00 ac XX`
  - `XX` is the combined `V/R` byte.

Examples:

- `fe 05 00 ac 04` -> request with `R=4` (with current bit mapping/decoder behavior).
- Wireshark may show `Experimental: TCP ACK Rate Request (0x00ac)` for ExID-matched packets.

## What is still not implemented from full TARR spec

- No explicit `cwnd`/`rwin` policy bound for chosen/sent `R` (only local clamp to `<=127`).
- No strict policy to send request only on change (current behavior may repeat requests).
- No complete retransmission-specific rule handling for TARR request suppression/refresh.
- No exhaustive conformance test coverage for all draft corner cases.
- No dedicated ns-3 trace sources yet for TARR runtime state (`peer capable`, applied `R`, counter progression).

