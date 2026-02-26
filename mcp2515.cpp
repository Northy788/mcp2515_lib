/**
 * @file mcp2515.cpp
 * @author Mr. Thaithat Kantha
 * @brief Source file for MCP2515 CAN controller library, implementing core driver functions.
 * @version 1.0.0
 * @date 2026-02-23
 * @copyright Copyright (c) 2026 MongMongTech Co., Ltd. All rights reserved.
 **/

#include <stdint.h>

#include "mcp2515.hpp"
#include "mcp2515_regs.hpp"

namespace { /* Anonymous namespace for internal linkage and encapsulation of helper types and constants. */

struct BitrateConfig_t {
    uint8_t cnf1;
    uint8_t cnf2;
    uint8_t cnf3;
};

static const BitrateConfig_t k_bitrateTable[] = {
#define MCP2515_CONF(freq, speed, c1, c2, c3) { c1, c2, c3 }, // (A)
    MCP2515_BITRATE_LIST // (B)
#undef MCP2515_CONF
};
}

/**
 * @brief Send Reset instruction to MCP2515.
 * SPI mode 0 (0,0), max 10MHz, CS low during transfer, then delay for internal reset to complete.
 * @return true if reset command was successfully sent, false otherwise.
 */
bool Mcp2515_Driver_t::mcp2515_reset(void) {
    bool isSuccess = false;
    const uint8_t instruction = Mcp2515_Regs::INST__RESET;

    /* Reset sequence: CS low -> Send 0xC0 -> CS high -> Delay */
    if (nullptr != p_hal->spiTransfer) {
        isSuccess = executeSpiCommand(&instruction, nullptr, 1); /* Use helper for consistency and safety checks */
        
        /* Wait for internal reset to complete */
        p_hal->delayMs(10);
    }

    return isSuccess;
}

/**
 * @brief Write a single byte to a specific register.
 * @param address The register address to write to.
 * @param data The value to be written.
 * @return true if the operation was successful.
 */
bool Mcp2515_Driver_t::mcp2515_writeRegister(const uint8_t address, const uint8_t data)
{
    bool isSuccess = false;

    /* MCP2515 Write Sequence: [Instruction (0x02)] -> [Address] -> [Data] */
    uint8_t txBuffer[3] = {
        Mcp2515_Regs::INST__WRITE,
        address,
        data
    };

    /* Verify HAL and SPI function pointer availability */
    if (nullptr != p_hal->spiTransfer)
    {
        /* 1. Pull CS Low to start SPI transaction */
        p_hal->setChipSelect(true);

        /* 2. Transfer 3 bytes (Instruction, Address, Data) */
        /* pRxBuffer is set to nullptr as we don't need to read back during Write */
        isSuccess = p_hal->spiTransfer(txBuffer, nullptr, 3);

        /* 3. Pull CS High to terminate SPI transaction */
        p_hal->setChipSelect(false);
    }

    return isSuccess; /* Rule 12.6: Single return point */
}

/**
 * @brief Read a single byte from a specific register.
 * @param address The register address to read from.
 * @return The 8-bit value read from the register. 
 * Note: In a professional driver, you might return a status code 
 * and use a pointer for the data, but for simplicity here we return the value.
 */
uint8_t Mcp2515_Driver_t::mcp2515_readRegister(const uint8_t address)
{
    uint8_t readValue = 0x00;
    
    /* Prepare 3-byte buffers for the full-duplex transfer 
     * [0] Instruction, [1] Address, [2] Dummy Byte (to clock out the data)
     */
    uint8_t txData[3] = {
        Mcp2515_Regs::INST__READ,
        address,
        0x00 /* Dummy byte */
    };
    
    uint8_t rxData[3] = {0};

    /* Use our helper to handle CS and safety checks */
    if (executeSpiCommand(txData, rxData, 3))
    {
        /* The data from MCP2515 shifts in while we shift out the dummy byte.
         * So the result is in the 3rd byte of our receive buffer.
         */
        readValue = rxData[2];
    }
    else
    {
        /* Rule: In professional systems, log an error here if SPI fails */
    }

    return readValue;
}

/**
 * @brief Modify specific bits in a register using a mask.
 * @note This instruction is only available for certain registers (refer to datasheet).
 * @param address The register address to modify.
 * @param mask The bit mask (1 = allow change, 0 = keep original).
 * @param data The new values for the bits enabled in the mask.
 * @return true if the SPI operation was successful.
 */
bool Mcp2515_Driver_t::mcp2515_bitModify(const uint8_t address, const uint8_t mask, const uint8_t data)
{
    bool isSuccess = false;

    /* BIT MODIFY Sequence: [0x05] -> [Address] -> [Mask] -> [Data] */
    uint8_t txData[4] = {
        Mcp2515_Regs::INST__BIT_MODIFY,
        address,
        mask,
        data
    };

    /* Use our internal helper to handle safety checks and CS management */
    isSuccess = executeSpiCommand(txData, nullptr, 4);

    return isSuccess;
}

/**
 * @brief Configure the CAN bitrate using pre-defined CNF values.
 * @note The MCP2515 MUST be in Configuration Mode to execute this.
 * @param speedIndex The index from Mcp2515_Speed_e enum.
 * @return true if all registers were written successfully.
 */
bool Mcp2515_Driver_t::mcp2515_setBitrate(const Mcp2515_Regs::Mcp2515_Speed_e speedIndex)
{
    bool isSuccess = false;

    /* 1. Range Check: Defensive Programming */
    if (speedIndex < Mcp2515_Regs::Mcp2515_Speed_Max)
    {
        const BitrateConfig_t* pCfg = &k_bitrateTable[speedIndex];

        /* 2. Execute Writes 
         * Note: We use the helper 'mcp2515_writeRegister' which already 
         * handles CS and HAL safety checks.
         */
        bool w1 = mcp2515_writeRegister(Mcp2515_Regs::CNF1, pCfg->cnf1);
        bool w2 = mcp2515_writeRegister(Mcp2515_Regs::CNF2, pCfg->cnf2);
        bool w3 = mcp2515_writeRegister(Mcp2515_Regs::CNF3, pCfg->cnf3);

        /* 3. Logical AND for final success status (Single Exit Point) */
        if (w1 && w2 && w3)
        {
            uint8_t v1 = mcp2515_readRegister(Mcp2515_Regs::CNF1);
            uint8_t v2 = mcp2515_readRegister(Mcp2515_Regs::CNF2);
            uint8_t v3 = mcp2515_readRegister(Mcp2515_Regs::CNF3);

            /* เปรียบเทียบค่าที่อ่านได้กับค่าที่ตั้งใจเขียนลงไป */
            if ((v1 == pCfg->cnf1) && (v2 == pCfg->cnf2) && (v3 == pCfg->cnf3))
            {
                isSuccess = true;
            }
            else
            {
                /* Log Error: Bitrate Verification Failed! 
                 * อาจเกิดจาก Chip ไม่ได้อยู่ใน Configuration Mode
                 */
            }
        }
    }

    return isSuccess;
}

/**
 * @brief Change the operation mode of the MCP2515.
 * @param targetMode The desired mode (e.g., Normal, Sleep, Config).
 * @return true if the mode was successfully changed within the timeout period.
 */
bool Mcp2515_Driver_t::mcp2515_setMode(const Mcp2515_Regs::Mcp2515_Mode_e targetMode)
{
    bool isSuccess = false;
    uint32_t timeoutCounter = 0;
    const uint32_t MAX_POLLING_RETRY = 2000; /* Adjust based on SPI speed/System clock */

    /* 1. Request Mode Change using Bit Modify
     * Address: CANCTRL (0x0F)
     * Mask: 0xE0 (1110 0000) - We only want to touch the top 3 bits (REQOP)
     * Data: targetMode (should be shifted/defined to occupy bits 7,6,5)
     */
    mcp2515_bitModify(Mcp2515_Regs::CANCTRL, 0xE0, static_cast<uint8_t>(targetMode));

    /* 2. Verification via Polling
     * We must check the OPMOD bits (7,6,5) in CANSTAT to confirm the change.
     */
    while (timeoutCounter < MAX_POLLING_RETRY)
    {
        uint8_t currentStat = mcp2515_readRegister(Mcp2515_Regs::CANSTAT);
        
        /* Mask bits 7,6,5 to isolate OPMOD */
        if ((currentStat & 0xE0) == static_cast<uint8_t>(targetMode))
        {
            isSuccess = true;
            break; /* Exit loop early if successful */
        }
        
        timeoutCounter++;
    }

    return isSuccess;
}

/**
 * @brief Read a CAN message from the specified hardware RX buffer.
 * @param bufferId 0 for RXB0, 1 for RXB1.
 * @param pMsg Pointer to CanMessage_t to store the result.
 * @return true if the message was read successfully.
 */
bool Mcp2515_Driver_t::mcp2515_readMessage(const uint8_t bufferId, CanMessage_t* pMsg) {
    if (nullptr == pMsg) return false;

    /* 1. Determine the Optimized Instruction (Starting at SIDH) */
    uint8_t instruction = (bufferId == 0) ? Mcp2515_Regs::INST__READ_RX0_SIDH : Mcp2515_Regs::INST__READ_RX1_SIDH;

    /* 2. Prepare Buffers
     * [0] Instruction
     * [1..4] ID Registers
     * [5] DLC
     * [6..13] Data Bytes
     * Total = 1 + 13 = 14 bytes
     */
    uint8_t txData[14] = {0}; /* Rest are implicitly 0 (dummy) */
    uint8_t rxData[14] = {0};
	txData[0] = instruction;
	
    if (!executeSpiCommand(txData, rxData, 14)) return false;
    
    
    pMsg->id = (static_cast<uint32_t>(rxData[1]) << 3) | (rxData[2] >> 5);
    
    pMsg->isExtended = (rxData[2] & 0x08) != 0;
    if (pMsg->isExtended) {
        pMsg->id = (pMsg->id << 18) | 
                   (static_cast<uint32_t>(rxData[2] & 0x03) << 16) |
                   (static_cast<uint32_t>(rxData[3]) << 8) |
                   rxData[4];
    }
    
    pMsg->dlc = rxData[5] & 0x0F;
    pMsg->isRtr = (rxData[5] & 0x40) != 0;
    
    for (uint8_t i = 0; i < pMsg->dlc; i++) {
        pMsg->data[i] = rxData[6 + i];
    }
    
    return true;
}


/**
 * @brief ส่งข้อความ CAN ผ่านบัฟเฟอร์ที่กำหนด
 * @param bufferId หมายเลขบัฟเฟอร์ (0, 1 หรือ 2)
 * @param pMsg ตัวชี้ไปยังข้อมูลข้อความที่ต้องการส่ง
 * @return true ถ้าส่งคำสั่งสำเร็จ
 */
bool Mcp2515_Driver_t::mcp2515_sendMessage(const uint8_t bufferId, const CanMessage_t* pMsg) {
    if (pMsg == nullptr || bufferId > 2) return false;

    /* 1. เตรียม Instruction สำหรับ Load TX Buffer 
     * เริ่มที่ตำแหน่ง ID (SIDH) ของบัฟเฟอร์นั้นๆ
     */
    uint8_t instruction = 0;
    if (bufferId == 0)      instruction = 0x40; // LOAD TXB0 SIDH
    else if (bufferId == 1) instruction = 0x42; // LOAD TXB1 SIDH
    else                    instruction = 0x44; // LOAD TXB2 SIDH

    /* 2. prepare data 13 bytes (ID 4 + DLC 1 + Data 8) */
    uint8_t txData[14] = {0};
    txData[0] = instruction;

    // จัดการ ID (ตัวอย่างสำหรับ Standard ID)
    txData[1] = static_cast<uint8_t>(pMsg->id >> 3);       // SIDH
    txData[2] = static_cast<uint8_t>(pMsg->id << 5);       // SIDL
    
    if (pMsg->isExtended) {
        txData[2] |= 0x08; // Set EXIDE bit
        // เพิ่มเติมการจัดการ EID ถ้าใช้งาน Extended ID...
        txData[3] = static_cast<uint8_t>(pMsg->id >> 16); // EID8
        txData[4] = static_cast<uint8_t>(pMsg->id >> 8);  // EID0
    }

    // จัดการ DLC
    txData[5] = pMsg->dlc & 0x0F;
    if (pMsg->isRtr) txData[5] |= 0x40; // Set RTR bit

    // โหลด Data Payload
    for (uint8_t i = 0; i < pMsg->dlc; i++) {
        txData[6 + i] = pMsg->data[i];
    }

    /* 3. ส่งข้อมูลเข้าบัฟเฟอร์ผ่าน SPI */
    if (!executeSpiCommand(txData, nullptr, 14)) return false;

    /* 4. สั่ง Request to Send (RTS) เพื่อเริ่มการส่งจริง
     * Instruction: 1000 0nnn (n คือบิตระบุบัฟเฟอร์)
     */
    uint8_t rtsInstruction = 0x80 | (1 << bufferId);
    return executeSpiCommand(&rtsInstruction, nullptr, 1);
}

/**
 * @brief Clear a specific interrupt flag in the CANINTF register.
 * @param interrupt The specific interrupt flag to clear.
 * @return true if the SPI command was successful.
 */
bool Mcp2515_Driver_t::mcp2515_clearInterrupt(const Mcp2515_Regs::Mcp2515_Interrupt_e interrupt)
{
    /* * bitModify(Address, Mask, Data)
     * Address: CANINTF (0x2C)
     * Mask: The specific bit we want to change (e.g., 0x01 for RX0IF)
     * Data: 0x00 (Because writing 0 clears the flag in MCP2515)
     */
    return mcp2515_bitModify(Mcp2515_Regs::CANINTF, 
                             static_cast<uint8_t>(interrupt), 
                             0x00);
}

/** @brief อ่านค่า Error Flag ทั้งหมดจากรีจิสเตอร์ EFLG (0x2D) */
uint8_t Mcp2515_Driver_t::mcp2515_getErrorFlags(void) {
    return mcp2515_readRegister(Mcp2515_Regs::EFLG);
}

/** @brief อ่านค่า Interrupt Flag ทั้งหมดจากรีจิสเตอร์ CANINTF (0x2C) */
uint8_t Mcp2515_Driver_t::mcp2515_getInterrupts(void) {
    return mcp2515_readRegister(Mcp2515_Regs::CANINTF);
}

/** @brief ล้างบิต Overflow ทั้งบัฟเฟอร์ 0 และ 1 (0xC0) ใน EFLG */
void Mcp2515_Driver_t::mcp2515_clearRXnOVR(void) {
    /* 0xC0 = 1100 0000 (RX1OVR และ RX0OVR) */
    mcp2515_bitModify(Mcp2515_Regs::EFLG, 0xC0, 0x00);
}

/** @brief ล้าง Error Interrupt Flag ใน CANINTF */
void Mcp2515_Driver_t::mcp2515_clearERRIF(void) {
    mcp2515_clearInterrupt(Mcp2515_Regs::Mcp2515_Int_Error);
}

/** @brief ล้าง Message Error Interrupt Flag ใน CANINTF */
void Mcp2515_Driver_t::mcp2515_clearMERR(void) {
    mcp2515_clearInterrupt(Mcp2515_Regs::Mcp2515_Int_Message);
}


/** @brief ตั้งค่า Acceptance Filter (RXFn) โดยระบุที่อยู่เริ่มต้น (SIDH) */
bool Mcp2515_Driver_t::mcp2515_setFilter(const uint8_t filterAddr, const bool isExtended, const uint32_t id) {
    uint8_t txData[6] = { Mcp2515_Regs::INST__WRITE, filterAddr };
    
    if (isExtended) {
        txData[2] = static_cast<uint8_t>(id >> 21);
        txData[3] = static_cast<uint8_t>(((id >> 13) & 0xE0) | 0x08 | ((id >> 16) & 0x03));
        txData[4] = static_cast<uint8_t>(id >> 8);
        txData[5] = static_cast<uint8_t>(id);
    } else {
        txData[2] = static_cast<uint8_t>(id >> 3);
        txData[3] = static_cast<uint8_t>(id << 5);
        txData[4] = 0;
        txData[5] = 0;
    }
    return executeSpiCommand(txData, nullptr, 6);
}

/** @brief ตั้งค่า Acceptance Mask (RXMn) โดยระบุที่อยู่เริ่มต้น (SIDH) */
bool Mcp2515_Driver_t::mcp2515_setMask(const uint8_t maskAddr, const bool isExtended, const uint32_t maskValue) {
    /* ใช้ Logic เดียวกับ Filter แต่บิต EXIDE (0x08) ในไบต์ที่ 2 มักใช้ต่างกันเล็กน้อย */
    return mcp2515_setFilter(maskAddr, isExtended, maskValue);
}

/** @brief ตรวจสอบสถานะ Bus-Off (TXBO บิต 5 ใน EFLG) */
bool Mcp2515_Driver_t::mcp2515_isBusOff(void) {
    return (mcp2515_readRegister(Mcp2515_Regs::EFLG) & 0x20) != 0;
}

/** @brief อ่านค่าตัวนับข้อผิดพลาดฝั่งรับ (REC) */
uint8_t Mcp2515_Driver_t::mcp2515_getREC(void) {
    return mcp2515_readRegister(Mcp2515_Regs::REC);
}

/** @brief อ่านค่าตัวนับข้อผิดพลาดฝั่งส่ง (TEC) */
uint8_t Mcp2515_Driver_t::mcp2515_getTEC(void) {
    return mcp2515_readRegister(Mcp2515_Regs::TEC);
}

/** @brief อ่านสถานะด่วนโดยใช้คำสั่ง 0xA0 (เร็วกว่าการอ่านรีจิสเตอร์ปกติ) */
uint8_t Mcp2515_Driver_t::mcp2515_getStatus(void) {
    uint8_t tx[2] = { Mcp2515_Regs::INST__READ_STATUS, 0x00 };
    uint8_t rx[2] = { 0 };
    executeSpiCommand(tx, rx, 2);
    return rx[1]; /* ข้อมูลสถานะจะถูกส่งกลับมาในไบต์ที่ 2 */
}

/** @brief เช็คว่าบัฟเฟอร์การส่งที่ระบุ (0, 1, 2) กำลังรอคิวส่งอยู่หรือไม่ (TXREQ) */
bool Mcp2515_Driver_t::mcp2515_isTxPending(const uint8_t bufferId) {
    uint8_t addr = (bufferId == 0) ? Mcp2515_Regs::TXB0__CTRL :
                   (bufferId == 1) ? Mcp2515_Regs::TXB1__CTRL : Mcp2515_Regs::TXB2__CTRL;
    /* เช็คบิตที่ 3 (TXREQ) */
    return (mcp2515_readRegister(addr) & 0x08) != 0;
}

/**
 * @brief อ่านรีจิสเตอร์สำคัญ 12 ตัวเพื่อใช้ในการ Debug
 * @param pDestBuffer ตัวชี้ไปยังอาเรย์ขนาดอย่างน้อย 12 ไบต์
 */
void Mcp2515_Driver_t::mcp2515_getTelemetry(uint8_t* pDestBuffer) {
    if (nullptr == pDestBuffer) return;

    // ลิสต์รีจิสเตอร์ตามลำดับที่ออกแบบไว้
    const uint8_t regList[] = {
        Mcp2515_Regs::CANSTAT,  Mcp2515_Regs::CANCTRL,  // 0x0E, 0x0F
        Mcp2515_Regs::TEC,      Mcp2515_Regs::REC,      // 0x1C, 0x1D
        Mcp2515_Regs::CANINTE,  Mcp2515_Regs::CANINTF,  // 0x2B, 0x2C
        Mcp2515_Regs::EFLG,                             // 0x2D
        Mcp2515_Regs::TXB0__CTRL, Mcp2515_Regs::TXB1__CTRL, Mcp2515_Regs::TXB2__CTRL, // 0x30, 0x40, 0x50
        Mcp2515_Regs::RXB0__CTRL, Mcp2515_Regs::RXB1__CTRL  // 0x60, 0x70
    };

    for (uint8_t i = 0; i < 12; i++) {
        pDestBuffer[i] = mcp2515_readRegister(regList[i]);
    }
}

/**
 * @brief Internal helper to encapsulate the CS logic and safety checks.
 * Rule 12.6: Single Exit Point.
 */
bool Mcp2515_Driver_t::executeSpiCommand(const uint8_t* pTxData, uint8_t* pRxData, uint16_t len) {
    bool isSuccess = false;

    if ((nullptr != p_hal) && (nullptr != p_hal->spiTransfer)) {

        if (p_hal->lockBus) p_hal->lockBus(); 

        p_hal->setChipSelect(true);  
        isSuccess = p_hal->spiTransfer(pTxData, pRxData, len); 
        p_hal->setChipSelect(false); 

        if (p_hal->unlockBus) p_hal->unlockBus();
    }
    return isSuccess;
}