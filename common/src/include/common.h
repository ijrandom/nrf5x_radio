
#ifndef NRF51822_XXAC_COMMON_H_H
#define NRF51822_XXAC_COMMON_H_H

#define PACKET_BASE_ADDRESS_LENGTH  (4UL)                   //!< Packet base address length field size in bytes
#define PACKET_STATIC_LENGTH        (10UL)                   //!< Packet static length in bytes
#define PACKET_PAYLOAD_MAXSIZE      (PACKET_STATIC_LENGTH)  //!< Packet payload maximum size in bytes

void radio_configure2();

#endif //NRF51822_XXAC_COMMON_H_H
