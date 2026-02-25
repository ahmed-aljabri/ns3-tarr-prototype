# ns3-tarr-prototype

Prototype implementation of TCP ACK Rate Request (TARR) in the ns-3 TCP stack.

Reference draft:
- `draft-ietf-tcpm-ack-rate-request`

## What is implemented

- TARR TCP option registration in ns-3 option factory (`Kind=254`).
- TARR wire formats:
  - Capability format (`Len=4`): `Kind, Len, ExID`
  - Request format (`Len=5`): `Kind, Len, ExID, V/R byte`
- ExID support (`0x00AC`) with parser validation.
- Socket-level attributes for TARR configuration:
  - `TarrEnabled`
  - `TarrRequestRatio` (`0..127`)
  - `TarrAdvertisedCapability`
  - `TarrRequestOnData`
- Send/receive integration in `TcpSocketBase`:
  - Capability and request option insertion
  - Incoming TARR processing and peer capability tracking
  - Applied peer ratio state updates
- Delayed-ACK path integration using peer-requested ratio.

## Files touched

- `src/internet/model/tcp-option-tarr.h`
- `src/internet/model/tcp-option-tarr.cc`
- `src/internet/model/tcp-option.h`
- `src/internet/model/tcp-option.cc`
- `src/internet/model/tcp-socket-base.h`
- `src/internet/model/tcp-socket-base.cc`
- `src/internet/CMakeLists.txt`
- `TARR_NOTES.md`

## How to build and run

From ns-3 root:

```bash
./ns3 build
./ns3 run "tcp-bulk-send --tracing=true --ns3::TcpSocketBase::TarrEnabled=1 --ns3::TcpSocketBase::TarrAdvertisedCapability=1 --ns3::TcpSocketBase::TarrRequestRatio=4"
```

Optional runtime logs:

```bash
NS_LOG="TcpSocketBase=level_info|prefix_time:TcpOptionTARR=level_info|prefix_time" \
./ns3 run "tcp-bulk-send --tracing=true --ns3::TcpSocketBase::TarrEnabled=1 --ns3::TcpSocketBase::TarrAdvertisedCapability=1 --ns3::TcpSocketBase::TarrRequestRatio=4"
```


## Remaining work

- Add/validate policy bounds using cwnd/rwnd constraints from draft guidance.
- Improve request policy (avoid unnecessary repeated request emission).
- Add retransmission-specific policy handling for TARR requests.
- Add dedicated trace sources for TARR runtime state (`peer capability`, applied `R`, counters).
- Expand tests for corner cases and draft conformance behavior.

