# ns3-tarr-prototype

 Implementation of TCP ACK Rate Request (TARR) on the existing ns-3 TCP stack.

Reference draft:
- [draft-ietf-tcpm-ack-rate-request-10](https://datatracker.ietf.org/doc/draft-ietf-tcpm-ack-rate-request/)

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

## ns-3 files modified

- `src/internet/model/tcp-option-tarr.h`
- `src/internet/model/tcp-option-tarr.cc`
- `src/internet/model/tcp-option.h`
- `src/internet/model/tcp-option.cc`
- `src/internet/model/tcp-socket-base.h`
- `src/internet/model/tcp-socket-base.cc`

## How to build and run

## Installing into ns-3

This prototype is built against **ns-3.46.1**. Other versions may work but are untested.

### Steps

1. Copy the new files into your ns-3 internet model directory:
   - `src/internet/model/tcp-option-tarr.h`
   - `src/internet/model/tcp-option-tarr.cc`

2. Replace the following stock ns-3 files with the modified versions from this repo:
   - `src/internet/model/tcp-option.h`
   - `src/internet/model/tcp-option.cc`
   - `src/internet/model/tcp-socket-base.h`
   - `src/internet/model/tcp-socket-base.cc`

   > **Note:** These files replace the originals. If you have local changes to them, merge carefully.

3. Rebuild ns-3 from the root directory:
   ```bash
    ./ns3 build
    ./ns3 run "tcp-bulk-send --tracing=true --ns3::TcpSocketBase::TarrEnabled=1 --ns3::TcpSocketBase::TarrAdvertisedCapability=1 --ns3::TcpSocketBase::TarrRequestRatio=4"


## Remaining work
- Add/validate policy bounds using cwnd/rwnd constraints from draft guidance.
- Discourage advertising tarr capability on last ACK of 3WH
- Add retransmission-specific policy handling for TARR requests.
- [Optional] Add dedicated trace sources for TARR runtime state.
- Expand tests for corner cases and draft conformance behavior.
- Add `TCP_ACK_RATE_REQ_PROCESS` attribute to provide receiver side control to enable/disable TARR processing.
