/**
 * @file mcp2515_regs.hpp
 * @author Mr. Thaithat Kantha
 * @brief Register address definitions for the MCP2515 CAN controller.
 * @version 1.0.0
 * @date 2026-02-23
 * @copyright Copyright (c) 2026 MongMongTech Co., Ltd. All rights reserved.
 **/

#ifndef MCP2515_REGS_HPP_
#define MCP2515_REGS_HPP_

#include <stdint.h>

namespace Mcp2515_Regs
{
    /* --- 1. Bitrate Configurations (X-Macro) --- */
    #define MCP2515_BITRATE_LIST \
      /* Frequency, Speed, CNF1, CNF2, CNF3 */  \
      MCP2515_CONF(8,  1000, 0x00, 0x80, 0x80)  \
      MCP2515_CONF(8,  500, 0x00, 0x90, 0x82)   \
      MCP2515_CONF(8,  250, 0x00, 0xB1, 0x85)   \
      MCP2515_CONF(8,  125, 0x01, 0xB1, 0x85)   \
      MCP2515_CONF(8,  100, 0x01, 0xB4, 0x86)   \
      MCP2515_CONF(16,  1000, 0x00, 0xD0, 0x82) \
      MCP2515_CONF(16,  500, 0x00, 0xF0, 0x86)  \
      MCP2515_CONF(16,  250, 0x41, 0xF1, 0x85)  \
      MCP2515_CONF(16,  125, 0x03, 0xF0, 0x86)  \
      MCP2515_CONF(16,  100, 0x03, 0xFA, 0x87)  \
      MCP2515_CONF(20,  1000, 0x00, 0xD9, 0x82) \
      MCP2515_CONF(20,  500, 0x00, 0xFA, 0x87)  \
      MCP2515_CONF(20,  250, 0x41, 0xFB, 0x86)  \
      MCP2515_CONF(20,  125, 0x03, 0xFA, 0x87)  \
      MCP2515_CONF(20,  100, 0x03, 0xFA, 0x87)

    typedef enum {
      #define MCP2515_CONF(freq, speed, c1, c2, c3) Mcp2515_Speed_##freq##MHz_##speed##k,
        MCP2515_BITRATE_LIST
      #undef MCP2515_CONF
        Mcp2515_Speed_Max
    } Mcp2515_Speed_e;

    /**
     * @brief MCP2515 Operation Modes (REQOP bits in CANCTRL / OPMOD bits in CANSTAT)
     */
    typedef enum {
      Mcp2515_Mode_Normal     = (0x00), /* 0000 0000 */
      Mcp2515_Mode_Sleep      = (0x20), /* 0010 0000 */
      Mcp2515_Mode_Loopback   = (0x40), /* 0100 0000 */
      Mcp2515_Mode_ListenOnly = (0x60), /* 0110 0000 */
      Mcp2515_Mode_Config     = (0x80)  /* 1000 0000 */
    } Mcp2515_Mode_e;

    /**
     * @brief MCP2515 Interrupt Code (ICOD) from CANSTAT register - Rule 15.6
     */
    typedef enum {
      Mcp2515_InterruptCode_None   = 0x00,
      Mcp2515_InterruptCode_Error  = 0x01,
      Mcp2515_InterruptCode_Wakeup = 0x02,
      Mcp2515_InterruptCode_Txb0   = 0x03,
      Mcp2515_InterruptCode_Txb1   = 0x04,
      Mcp2515_InterruptCode_Txb2   = 0x05,
      Mcp2515_InterruptCode_Rxb0   = 0x06,
      Mcp2515_InterruptCode_Rxb1   = 0x07
    } Mcp2515_InterruptCode_e;  

    /**
     * @brief MCP2515 Interrupt Flag Bits in CANINTF register.
     */
    typedef enum {
        Mcp2515_Int_Rx0      = 0x01, /**< Receive Buffer 0 Full */
        Mcp2515_Int_Rx1      = 0x02, /**< Receive Buffer 1 Full */
        Mcp2515_Int_Tx0      = 0x04, /**< Transmit Buffer 0 Empty */
        Mcp2515_Int_Tx1      = 0x08, /**< Transmit Buffer 1 Empty */
        Mcp2515_Int_Tx2      = 0x10, /**< Transmit Buffer 2 Empty */
        Mcp2515_Int_Error    = 0x20, /**< Error Interrupt */
        Mcp2515_Int_Wakeup   = 0x40, /**< Wake-up Interrupt */
        Mcp2515_Int_Message  = 0x80  /**< Message Error Interrupt */
    } Mcp2515_Interrupt_e;

    /**
    * @name Acceptance Filters (RXFn) - Rule 15.1
    * @{ */
    static constexpr uint8_t RXF0__SIDH = 0x00; /**< Acceptance Filter 0 Standard High */
    static constexpr uint8_t RXF0__SIDL = 0x01; /**< Acceptance Filter 0 Standard Low */
    static constexpr uint8_t RXF0__EID8 = 0x02; /**< Acceptance Filter 0 Extended High */
    static constexpr uint8_t RXF0__EID0 = 0x03; /**< Acceptance Filter 0 Extended Low */

    static constexpr uint8_t RXF1__SIDH = 0x04; /**< Acceptance Filter 1 Standard High */
    static constexpr uint8_t RXF1__SIDL = 0x05; /**< Acceptance Filter 1 Standard Low */
    static constexpr uint8_t RXF1__EID8 = 0x06; /**< Acceptance Filter 1 Extended High */
    static constexpr uint8_t RXF1__EID0 = 0x07; /**< Acceptance Filter 1 Extended Low */

    static constexpr uint8_t RXF2__SIDL = 0x08; /**< Acceptance Filter 2 Standard Low */
    static constexpr uint8_t RXF2__EID8 = 0x09; /**< Acceptance Filter 2 Extended High */
    static constexpr uint8_t RXF2__EID0 = 0x0A; /**< Acceptance Filter 2 Extended Low */
    static constexpr uint8_t RXF2__SIDH = 0x0B; /**< Acceptance Filter 2 Standard High */

    static constexpr uint8_t RXF3__SIDH = 0x10; /**< Acceptance Filter 3 Standard High */
    static constexpr uint8_t RXF3__SIDL = 0x11; /**< Acceptance Filter 3 Standard Low */
    static constexpr uint8_t RXF3__EID8 = 0x12; /**< Acceptance Filter 3 Extended High */
    static constexpr uint8_t RXF3__EID0 = 0x13; /**< Acceptance Filter 3 Extended Low */

    static constexpr uint8_t RXF4__SIDH = 0x14; /**< Acceptance Filter 4 Standard High */
    static constexpr uint8_t RXF4__SIDL = 0x15; /**< Acceptance Filter 4 Standard Low */
    static constexpr uint8_t RXF4__EID8 = 0x16; /**< Acceptance Filter 4 Extended High */
    static constexpr uint8_t RXF4__EID0 = 0x17; /**< Acceptance Filter 4 Extended Low */

    static constexpr uint8_t RXF5__SIDH = 0x18; /**< Acceptance Filter 5 Standard High */
    static constexpr uint8_t RXF5__SIDL = 0x19; /**< Acceptance Filter 5 Standard Low */
    static constexpr uint8_t RXF5__EID8 = 0x1A; /**< Acceptance Filter 5 Extended High */
    static constexpr uint8_t RXF5__EID0 = 0x1B; /**< Acceptance Filter 5 Extended Low */
    /** @} */

    /**
    * @name Error Counter Registers
    * @{ */
    static constexpr uint8_t TEC = 0x1C; /**< Transmit Error Counter */
    static constexpr uint8_t REC = 0x1D; /**< Receive Error Counter */
    /** @} */

    /**
    * @name Acceptance Mask Registers
    * @{ */
    static constexpr uint8_t RXM0__SIDH = 0x20; /**< Mask 0 Standard Identifier High */
    static constexpr uint8_t RXM0__SIDL = 0x21; /**< Mask 0 for Standard Identifier Low */
    static constexpr uint8_t RXM0__EID8 = 0x22; /**< Mask 0 for Extended Identifier High */
    static constexpr uint8_t RXM0__EID0 = 0x23; /**< Mask 0 for Extended Identifier Low */
    static constexpr uint8_t RXM1__SIDH = 0x24; /**< Mask 1 for Standard Identifier High */
    static constexpr uint8_t RXM1__SIDL = 0x25; /**< Mask 1 for Standard Identifier Low */
    static constexpr uint8_t RXM1__EID8 = 0x26; /**< Mask 1 for Extended Identifier High */
    static constexpr uint8_t RXM1__EID0 = 0x27; /**< Mask 1 for Extended Identifier Low */
    /** @} */

    /**
    * @name Configuration Registers
    * @{ */
    static constexpr uint8_t CNF3 = 0x28; /**< Configuration Register 3 */
    static constexpr uint8_t CNF2 = 0x29; /**< Configuration Register 2 */
    static constexpr uint8_t CNF1 = 0x2A; /**< Configuration Register 1 */
    /** @} */

    /**
    * @name Interrupt and Error Registers
    * @{ */
    static constexpr uint8_t CANINTE = 0x2B; /**< CAN Interrupt Enable Register */
    static constexpr uint8_t CANINTF = 0x2C; /**< CAN Interrupt Flag Register */
    static constexpr uint8_t EFLG = 0x2D; /**< Error Flag Register */
    /** @} */

    /**
    * @name Transmit Buffer Registers
    * @{ */

    /* Transmit Buffer 0 */
    static constexpr uint8_t TXB0__CTRL = 0x30; /**< Transmit Buffer 0 Control Register */
    static constexpr uint8_t TXB0__SIDH = 0x31; /**< Transmit Buffer 0 Standard Identifier High */
    static constexpr uint8_t TXB0__SIDL = 0x32; /**< Transmit Buffer 0 Standard Identifier Low */
    static constexpr uint8_t TXB0__EID8 = 0x33; /**< Transmit Buffer 0 Extended Identifier High */
    static constexpr uint8_t TXB0__EID0 = 0x34; /**< Transmit Buffer 0 Extended Identifier Low */
    static constexpr uint8_t TXB0__DLC = 0x35; /**< Transmit Buffer 0 Data Length Code */
    static constexpr uint8_t TXB0__D0 = 0x36; /**< Transmit Buffer 0 Data Byte 0 */
    static constexpr uint8_t TXB0__D1 = 0x37; /**< Transmit Buffer 0 Data Byte 1 */
    static constexpr uint8_t TXB0__D2 = 0x38; /**< Transmit Buffer 0 Data Byte 2 */
    static constexpr uint8_t TXB0__D3 = 0x39; /**< Transmit Buffer 0 Data Byte 3 */
    static constexpr uint8_t TXB0__D4 = 0x3A; /**< Transmit Buffer 0 Data Byte 4 */
    static constexpr uint8_t TXB0__D5 = 0x3B; /**< Transmit Buffer 0 Data Byte 5 */
    static constexpr uint8_t TXB0__D6 = 0x3C; /**< Transmit Buffer 0 Data Byte 6 */
    static constexpr uint8_t TXB0__D7 = 0x3D; /**< Transmit Buffer 0 Data Byte 7 */

    /* Transmit Buffer 1 */
    static constexpr uint8_t TXB1__CTRL = 0x40; /**< Transmit Buffer 1 Control Register */
    static constexpr uint8_t TXB1__SIDH = 0x41; /**< Transmit Buffer 1 Standard Identifier High */
    static constexpr uint8_t TXB1__SIDL = 0x42; /**< Transmit Buffer 1 Standard Identifier Low */
    static constexpr uint8_t TXB1__EID8 = 0x43; /**< Transmit Buffer 1 Extended Identifier High */
    static constexpr uint8_t TXB1__EID0 = 0x44; /**< Transmit Buffer 1 Extended Identifier Low */
    static constexpr uint8_t TXB1__DLC = 0x45; /**< Transmit Buffer 1 Data Length Code */
    static constexpr uint8_t TXB1__D0 = 0x46; /**< Transmit Buffer 1 Data Byte 0 */
    static constexpr uint8_t TXB1__D1 = 0x47; /**< Transmit Buffer 1 Data Byte 1 */
    static constexpr uint8_t TXB1__D2 = 0x48; /**< Transmit Buffer 1 Data Byte 2 */
    static constexpr uint8_t TXB1__D3 = 0x49; /**< Transmit Buffer 1 Data Byte 3 */
    static constexpr uint8_t TXB1__D4 = 0x4A; /**< Transmit Buffer 1 Data Byte 4 */
    static constexpr uint8_t TXB1__D5 = 0x4B; /**< Transmit Buffer 1 Data Byte 5 */
    static constexpr uint8_t TXB1__D6 = 0x4C; /**< Transmit Buffer 1 Data Byte 6 */
    static constexpr uint8_t TXB1__D7 = 0x4D; /**< Transmit Buffer 1 Data Byte 7 */

    /* Transmit Buffer 2 */
    static constexpr uint8_t TXB2__CTRL = 0x50; /**< Transmit Buffer 2 Control Register */
    static constexpr uint8_t TXB2__SIDH = 0x51; /**< Transmit Buffer 2 Standard Identifier High */
    static constexpr uint8_t TXB2__SIDL = 0x52; /**< Transmit Buffer 2 Standard Identifier Low */
    static constexpr uint8_t TXB2__EID8 = 0x53; /**< Transmit Buffer 2 Extended Identifier High */
    static constexpr uint8_t TXB2__EID0 = 0x54; /**< Transmit Buffer 2 Extended Identifier Low */
    static constexpr uint8_t TXB2__DLC = 0x55; /**< Transmit Buffer 2 Data Length Code */
    static constexpr uint8_t TXB2__D0 = 0x56; /**< Transmit Buffer 2 Data Byte 0 */
    static constexpr uint8_t TXB2__D1 = 0x57; /**< Transmit Buffer 2 Data Byte 1 */
    static constexpr uint8_t TXB2__D2 = 0x58; /**< Transmit Buffer 2 Data Byte 2 */
    static constexpr uint8_t TXB2__D3 = 0x59; /**< Transmit Buffer 2 Data Byte 3 */
    static constexpr uint8_t TXB2__D4 = 0x5A; /**< Transmit Buffer 2 Data Byte 4 */
    static constexpr uint8_t TXB2__D5 = 0x5B; /**< Transmit Buffer 2 Data Byte 5 */
    static constexpr uint8_t TXB2__D6 = 0x5C; /**< Transmit Buffer 2 Data Byte 6 */
    static constexpr uint8_t TXB2__D7 = 0x5D; /**< Transmit Buffer 2 Data Byte 7 */
    /** @} */

    /**
    * @name Receive Buffer Registers
    * @{ */

    /* Receive Buffer 0 */
    static constexpr uint8_t RXB0__CTRL = 0x60; /**< Receive Buffer 0 Control Register */
    static constexpr uint8_t RXB0__SIDH = 0x61; /**< Receive Buffer 0 Standard Identifier High */
    static constexpr uint8_t RXB0__SIDL = 0x62; /**< Receive Buffer 0 Standard Identifier Low */
    static constexpr uint8_t RXB0__EID8 = 0x63; /**< Receive Buffer 0 Extended Identifier High */
    static constexpr uint8_t RXB0__EID0 = 0x64; /**< Receive Buffer 0 Extended Identifier Low */
    static constexpr uint8_t RXB0__DLC = 0x65; /**< Receive Buffer 0 Data Length Code */
    static constexpr uint8_t RXB0__D0 = 0x66; /**< Receive  Buffer 0 Data Byte 0 */
    static constexpr uint8_t RXB0__D1 = 0x67; /**< Receive  Buffer 0 Data Byte 1 */
    static constexpr uint8_t RXB0__D2 = 0x68; /**< Receive  Buffer 0 Data Byte 2 */
    static constexpr uint8_t RXB0__D3 = 0x69; /**< Receive  Buffer 0 Data Byte 3 */
    static constexpr uint8_t RXB0__D4 = 0x6A; /**< Receive  Buffer 0 Data Byte 4 */
    static constexpr uint8_t RXB0__D5 = 0x6B; /**< Receive  Buffer 0 Data Byte 5 */
    static constexpr uint8_t RXB0__D6 = 0x6C; /**< Receive  Buffer 0 Data Byte 6 */
    static constexpr uint8_t RXB0__D7 = 0x6D; /**< Receive  Buffer 0 Data Byte 7 */

    /* Receive Buffer 1 */
    static constexpr uint8_t RXB1__CTRL = 0x70; /**< Receive Buffer 1 Control Register */
    static constexpr uint8_t RXB1__SIDH = 0x71; /**< Receive Buffer 1 Standard Identifier High */
    static constexpr uint8_t RXB1__SIDL = 0x72; /**< Receive Buffer 1 Standard Identifier Low */
    static constexpr uint8_t RXB1__EID8 = 0x73; /**< Receive Buffer 1 Extended Identifier High */
    static constexpr uint8_t RXB1__EID0 = 0x74; /**< Receive Buffer 1 Extended Identifier Low */
    static constexpr uint8_t RXB1__DLC = 0x75; /**< Receive Buffer 1 Data Length Code */
    static constexpr uint8_t RXB1__D0 = 0x76; /**< Receive  Buffer 1 Data Byte 0 */
    static constexpr uint8_t RXB1__D1 = 0x77; /**< Receive  Buffer 1 Data Byte 1 */
    static constexpr uint8_t RXB1__D2 = 0x78; /**< Receive  Buffer 1 Data Byte 2 */
    static constexpr uint8_t RXB1__D3 = 0x79; /**< Receive  Buffer 1 Data Byte 3 */
    static constexpr uint8_t RXB1__D4 = 0x7A; /**< Receive  Buffer 1 Data Byte 4 */
    static constexpr uint8_t RXB1__D5 = 0x7B; /**< Receive  Buffer 1 Data Byte 5 */
    static constexpr uint8_t RXB1__D6 = 0x7C; /**< Receive  Buffer 1 Data Byte 6 */
    static constexpr uint8_t RXB1__D7 = 0x7D; /**< Receive  Buffer 1 Data Byte 7 */

    /**
    * @name Main Control and Status Registers - Rule 15.1
    * @{ */
    static constexpr uint8_t BFPCTRL = 0x0C; /**< RXnBF Pin Control */
    static constexpr uint8_t TXRTSCTRL = 0x0D; /**< TXnRTS Pin Control */
    static constexpr uint8_t CANSTAT = 0x0E; /**< CAN Status Register (Read-Only) */
    static constexpr uint8_t CANCTRL = 0x0F; /**< CAN Control Register */
    /** @} */

/**
     * @name Basic Commands
     * @brief Standard commands for general register access and control.
     * @{ */
    static constexpr uint8_t INST__RESET       = 0xC0; /**< Resets internal registers to default state */
    static constexpr uint8_t INST__READ        = 0x03; /**< Read data from register beginning at selected address */
    static constexpr uint8_t INST__WRITE       = 0x02; /**< Write data to register beginning at selected address */
    static constexpr uint8_t INST__BIT_MODIFY  = 0x05; /**< Allows setting/clearing individual bits in specific registers */
    /** @} */


    /**
     * @name Optimized Commands - READ RX BUFFER
     * @brief Specialized commands to read RX buffers without sending an address byte.
     * @{ */
    static constexpr uint8_t INST__READ_RX0_SIDH = 0x90; /**< Start reading at RXB0__SIDH (0x61) */
    static constexpr uint8_t INST__READ_RX0_D0   = 0x92; /**< Start reading at RXB0__D0 (0x66) */
    static constexpr uint8_t INST__READ_RX1_SIDH = 0x94; /**< Start reading at RXB1__SIDH (0x71) */
    static constexpr uint8_t INST__READ_RX1_D0   = 0x96; /**< Start reading at RXB1__D0 (0x76) */
    /** @} */

    /**
     * @name Optimized Commands - LOAD TX BUFFER
     * @brief Specialized commands to load TX buffers without sending an address byte.
     * @{ */
    static constexpr uint8_t INST__LOAD_TX0_SIDH = 0x40; /**< Start loading at TXB0__SIDH (0x31) */
    static constexpr uint8_t INST__LOAD_TX0_D0   = 0x41; /**< Start loading at TXB0__D0 (0x36) */
    static constexpr uint8_t INST__LOAD_TX1_SIDH = 0x42; /**< Start loading at TXB1__SIDH (0x41) */
    static constexpr uint8_t INST__LOAD_TX1_D0   = 0x43; /**< Start loading at TXB1__D0 (0x46) */
    static constexpr uint8_t INST__LOAD_TX2_SIDH = 0x44; /**< Start loading at TXB2__SIDH (0x51) */
    static constexpr uint8_t INST__LOAD_TX2_D0   = 0x45; /**< Start loading at TXB2__D0 (0x56) */
    /** @} */

    /**
     * @name Optimized Commands - Request to Send (RTS)
     * @brief Commands to initiate message transmission for specific buffers.
     * @{ */
    static constexpr uint8_t INST__RTS_TX0     = 0x81; /**< RTS for Transmit Buffer 0 */
    static constexpr uint8_t INST__RTS_TX1     = 0x82; /**< RTS for Transmit Buffer 1 */
    static constexpr uint8_t INST__RTS_TX2     = 0x84; /**< RTS for Transmit Buffer 2 */
    static constexpr uint8_t INST__RTS_ALL     = 0x87; /**< RTS for all Transmit Buffers */
    /** @} */

/**
     * @name Quick Polling Commands
     * @brief Fast status check commands, ideal for use within ISR.
     * @{ */
    static constexpr uint8_t INST__READ_STATUS = 0xA0; /**< Poll critical status bits for TX/RX */
    static constexpr uint8_t INST__RX_STATUS   = 0xB0; /**< Poll filter match and message type status */
    /** @} */
}

#endif /* MCP2515_REGS_HPP_ */