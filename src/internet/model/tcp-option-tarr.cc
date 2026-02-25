#include "tcp-option-tarr.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("TcpOptionTARR");
NS_OBJECT_ENSURE_REGISTERED(TcpOptionTARR);

TcpOptionTARR::TcpOptionTARR()
    : m_r(2) // Default set to R=2 like standard delayed ACK
{
}

TcpOptionTARR::~TcpOptionTARR() = default;

TypeId
TcpOptionTARR::GetTypeId()
{
  static TypeId tid = TypeId("ns3::TcpOptionTARR")
    .SetParent<TcpOption>()
    .SetGroupName("Internet")
    .AddConstructor<TcpOptionTARR>();
  return tid;
}
/**
 * 
 * @brief Sets format to capability announcement only (Len = 4)
 */
void
TcpOptionTARR::SetCapabilityAnnouncement()
{
  m_len =TARR_LEN_CAPABILITY;
  m_r = 0;
  m_v = false;
  m_exId = TARR_EXID;
}

/**
 * 
 * @brief Sets format to TCP Ack Rate Request (Len = 5)
 */
void 
TcpOptionTARR::SetRequest(uint8_t r, bool v)
{
  m_r = (r & 0x7F);         // Requested R value and clamps to 7 bits
  m_len = TARR_LEN_REQUEST; // Len = 5
  m_exId = TARR_EXID;       // ExID remains the same in all cases = 0x00AC
  m_v = v;
}

/**
 * 
 * @brief Check if this is a announcement format
 */
bool
TcpOptionTARR::IsCapabilityAnnouncement() const
{
  return m_len == TARR_LEN_CAPABILITY;
}

/**
 * 
 * @brief Check if this is a request format
 */
bool
TcpOptionTARR::IsRequest() const
{
  return m_len == TARR_LEN_REQUEST;
}


// Getters
uint8_t
TcpOptionTARR::GetR() const
{
  return m_r;
}

bool
TcpOptionTARR::GetV() const
{
  return m_v;
}

uint16_t
TcpOptionTARR::GetExId() const
{
  return m_exId;
}

void
TcpOptionTARR::Print(std::ostream& os) const
{
    if (IsCapabilityAnnouncement())
    {
        os << "TARR(capability, ExID=0x" << std::hex << m_exId << std::dec << ")";
    }
    else
    {
        os << "TARR(request, ExID=0x" << std::hex << m_exId << std::dec
           << ", v=" << (m_v ? 1 : 0) << ", r=" << static_cast<uint32_t>(m_r) << ")";
    }
}

void
TcpOptionTARR::Serialize(Buffer::Iterator i) const
{
  i.WriteU8(GetKind());
  i.WriteU8(m_len);
  i.WriteHtonU16(m_exId);

  if (m_len == TARR_LEN_REQUEST)
  {
    uint8_t vr = static_cast<uint8_t>(((m_r & 0x7F) << 1) | (m_v ? 0x01 : 0x00));

    i.WriteU8(vr);
  }
}

uint32_t
TcpOptionTARR::Deserialize(Buffer::Iterator i)
{
  uint8_t kind = i.ReadU8();
  uint8_t len  = i.ReadU8();

  if (kind != GetKind() || (len != TARR_LEN_CAPABILITY && len != TARR_LEN_REQUEST))
  {
    NS_LOG_WARN("Malformed TARR option kind=" << static_cast<uint32_t>(kind)
                                              << " len=" << static_cast<uint32_t>(len));
    return 0;
  }

  uint16_t exid = i.ReadNtohU16();
  if (exid != TARR_EXID)
  {
    NS_LOG_WARN("Unexpected TARR ExID=0x"<<std::hex << exid << std::dec);
    return 0;
  }

  m_len = len;
  m_exId = exid;
  m_r = 0;
  m_v = false;

  if (m_len == TARR_LEN_REQUEST)
  {
    uint8_t vr = i.ReadU8();

    m_r = static_cast<uint8_t>((vr >> 1) & 0x7F);
    m_v = (vr & 0x01) != 0;
  }

  return m_len;
}

uint8_t
TcpOptionTARR::GetKind() const
{
  return TcpOption::TARR; // ensure enum has been extended
}

uint32_t
TcpOptionTARR::GetSerializedSize() const
{
  return m_len; // 4 or 5
}

} // namespace ns3
