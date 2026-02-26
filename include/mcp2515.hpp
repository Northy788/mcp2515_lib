/**
 * @file mcp2515.hpp
 * @author Mr. Thaithat Kantha
 * @brief Header file for MCP2515 CAN controller library, providing class definitions and function prototypes.
 * @version 1.0.0
 * @date 2026-02-23
 * @copyright Copyright (c) 2026 MongMongTech Co., Ltd. All rights reserved.
 **/

#ifndef MCP2515_HPP_
#define MCP2515_HPP_

#include <stdint.h>
#include <stdbool.h>

#include "mcp2515_regs.hpp"
#include "mcp2515_hal.hpp"


/**
 * @brief Structure to hold CAN message data.
 */
typedef struct {
    uint32_t id;          /**< CAN ID (Standard or Extended) */
    uint8_t  dlc;         /**< Data Length Code (0-8) */
    uint8_t  data[8];     /**< Data payload */
    bool     isExtended;  /**< True if Extended ID, False if Standard */
    bool     isRtr;       /**< Remote Transmission Request flag */
} CanMessage_t;

class Mcp2515_Driver_t {
public:
    /**
     * @brief Constructor requiring HAL implementation.
     * Rule 3.4: Prefix pointer parameters with 'p'.
     */
    Mcp2515_Driver_t(const Mcp2515_Hal_t* pHal) : p_hal(pHal) {}

    /* --- Core API --- */
    bool mcp2515_reset(void);
    bool mcp2515_writeRegister(const uint8_t address, const uint8_t value);
    uint8_t mcp2515_readRegister(const uint8_t address);
    bool mcp2515_bitModify(const uint8_t address, const uint8_t mask, const uint8_t data);
    bool mcp2515_setMode(const Mcp2515_Regs::Mcp2515_Mode_e targetMode);
    bool mcp2515_setBitrate(const Mcp2515_Regs::Mcp2515_Speed_e speedIndex);
    bool mcp2515_readMessage(const uint8_t bufferId, CanMessage_t* pMsg);
    bool mcp2515_sendMessage(const uint8_t bufferId, const CanMessage_t* pMsg);
    bool mcp2515_clearInterrupt(const Mcp2515_Regs::Mcp2515_Interrupt_e interrupt);
    
private:
    const Mcp2515_Hal_t* p_hal; /* Pointer to HAL interface */

    /* Internal helper for SPI operations to keep code DRY (Rule 14.2) */
    bool executeSpiCommand(const uint8_t* pTxData, uint8_t* pRxData, uint16_t len);
};

#endif /* MCP2515_HPP_ */
 