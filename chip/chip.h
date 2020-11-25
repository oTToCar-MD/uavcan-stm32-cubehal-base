#pragma once

#include "hal_header.h"

#if defined(STM32F4)
    #define CAN1_TX_IRQHandler CAN1_TX_IRQHandler
    #define CAN1_RX0_IRQHandler CAN1_RX0_IRQHandler
    #define CAN1_RX1_IRQHandler CAN1_RX1_IRQHandler

    #if UAVCAN_STM32_NUM_IFACES == 2
    #define CAN2_TX_IRQHandler CAN2_TX_IRQHandler
    #define CAN2_RX0_IRQHandler CAN2_RX0_IRQHandler
    #define CAN2_RX1_IRQHandler CAN2_RX1_IRQHandler
    #endif
#endif

#if defined(STM32F1)
    #define CAN1_RX1_IRQHandler CAN1_RX1_IRQHandler
#endif