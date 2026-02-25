#ifndef TCP_OPTION_TARR_H
#define TCP_OPTION_TARR_H

#include "ns3/tcp-option.h"

namespace ns3 {

/**
 * RFC_DRAFT: draft-ietf-tcpm-ack-rate-request-10
 * Minimal TARR TCP option (Kind=TcpOption::TARR)
 * Layout: | Kind (254) | Length= 4 or 5  | ExID 2 bytes (0x00AC) | Optional R
 */
class TcpOptionTARR : public TcpOption
{
public:
  TcpOptionTARR();
  ~TcpOptionTARR() override;

  static TypeId GetTypeId();

  static constexpr uint16_t TARR_EXID = 0x00AC;
  static constexpr uint8_t TARR_LEN_CAPABILITY = 4;   // TO ANNOUNCE SUPPORT OF TARR OPTION
  static constexpr uint8_t TARR_LEN_REQUEST = 5;      // TARR OPTION FORMAT


  // TARR methods
  void SetCapabilityAnnouncement();
  void SetRequest(uint8_t r, bool v = false);

  bool IsCapabilityAnnouncement() const;
  bool IsRequest() const;
  uint8_t GetR() const;
  bool GetV() const;
  uint16_t GetExId() const;


  // TcpOption interface
  void Print(std::ostream& os) const override;
  void Serialize(Buffer::Iterator i) const override;
  uint32_t Deserialize(Buffer::Iterator i) override;
  uint8_t GetKind() const override;
  uint32_t GetSerializedSize() const override;


  // TARR Fields
private:
  uint8_t m_len{TARR_LEN_CAPABILITY};
  uint16_t m_exId{TARR_EXID};
  uint8_t m_r{0};      // 7-bit R value 0..127
  bool m_v{false};     // top reserved bit
};

} // namespace ns3

#endif // TCP_OPTION_TARR_H