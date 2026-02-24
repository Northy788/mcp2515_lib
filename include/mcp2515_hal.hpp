/**
 * @file mcp2515_hal.hpp
 * @author Mr. Thaithat Kantha
 * @brief Header file defining the Hardware Abstraction Layer (HAL) interface for the MCP2515 CAN controller driver.
 * @version 1.0.0
 * @date 2026-02-23
 * @copyright Copyright (c) 2026 MongMongTech Co., Ltd. All rights reserved.
 **/

 #ifndef MCP2515_HAL_HPP_
 #define MCP2515_HAL_HPP_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief HAL Function Pointers - The Contract between Logic and Physical Layer.
 * Rule 5.1: Use _t suffix for structs.
 */
typedef struct {
    /** * @brief SPI Transfer function.
     * @param pTxData Pointer to transmit buffer.
     * @param pRxData Pointer to receive buffer (can be NULL if not needed).
     * @param len Number of bytes to transfer.
     * @return true if SPI transfer was successful.
     */
    bool (*spiTransfer)(const uint8_t* pTxBuffer, uint8_t* pRxBuffer, uint16_t len);

    /** @brief Function to control the Chip Select (CS) pin. */
    void (*setChipSelect)(bool active);

    /** @brief Millisecond delay function. */
    void (*delayMs)(uint32_t ms);
} Mcp2515_Hal_t;

#endif /* MCP2515_HAL_HPP_ */