/*
 * Copyright (C) 2016 Kaspar Schleiser <kaspar@schleiser.de>
 *               2015 Freie Universität Berlin
 *               2015 Engineering-Spirit
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     cpu_cortexm_common
 * @ingroup     drivers_periph_pm
 * @{
 *
 * @file
 * @brief       Implementation of the kernels power management interface
 *
 * @author      Nick v. IJzendoorn <nijzndoorn@engineering-spirit.nl>
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Fabian Nack <nack@inf.fu-berlin.de>
 *
 * @}
 */

#include "irq.h"
#include "periph/pm.h"

#define ENABLE_DEBUG (0)
#include "debug.h"

void pm_set(unsigned mode)
{
    int deep = 0;

/* I just copied it from stm32f1/2/4, but I suppose it would work for the
 * others... /KS */
#if defined(CPU_FAM_STM32F1) || defined(CPU_FAM_STM32F2) || defined(CPU_FAM_STM32F4)
    switch (mode) {
        case 0:
            /* Set PDDS to enter standby mode on deepsleep and clear flags */
            PWR->CR |= (PWR_CR_PDDS | PWR_CR_CWUF | PWR_CR_CSBF);
            /* Enable WKUP pin to use for wakeup from standby mode */
            PWR->CSR |= PWR_CSR_EWUP;
            /* Set SLEEPDEEP bit of system control block */
            deep = 1;
            break;
        case 1:                 /* STM Stop mode */
            /* Clear PDDS and LPDS bits to enter stop mode on */
            /* deepsleep with voltage regulator on */
            PWR->CR &= ~(PWR_CR_PDDS | PWR_CR_LPDS);
            /* Set SLEEPDEEP bit of system control block */
            deep = 1;
            break;
    }
#elif defined(CPU_FAM_STM32L4)
    /**
     * Accessibility of modes from other modes:
     *  Run mode: Low power run mode, shutdown mode, standby mode, sleep mode, 
     *            stop mode 0, stop mode 1, stop mode 2
     *  Low power run mode: low power sleep mode, shutdown mode, standby mode,
     *            run mode, stop 1 mode
     *  Low power sleep mode: -
     *  Shutdown mode: run mode
     *  Standby mode: run mode
     *  Sleep mode: run mode
     *  Stop mode 0: run mode
     *  Stop mode 1: run mode, low power run mode
     *  Stop mode 2: run mode
     */
    /**
     * Open question for WFI, do I need to set SEVONPEND = 1 (on PWR->LPR?)
     * There seems to be an alternate mode to enter modes on return from ISR,
     * when `SLEEPONEXIT = 1` and no interrupt is pending.
     */
    switch (mode) {
        case 0:
            /* 5.3.4: Sleep Mode */
            /* All I/O pins keep the same state as in Run mode. */
            /* The Sleep mode is entered according Section : Entering low
            power mode, when the SLEEPDEEP bit in the Cortex®-M4 System
            Control register is clear. */
            deep = 0;
            break;
        case 1:
            /* 5.3.2: Low-power mode */
            FLASH->ACR |= FLASH_ACR_RUN_PD; /* THIS IS OPTIONAL */
            /* STEP 2 MISSING: Decrease the system clock freq. below 2 MHz. */
            PWR->CR1 |= PWR_CR1_LPR;
            deep = 1;
            break;
            /*
            To exit the Low-power run mode, proceed as follows:
            1. Force the regulator in main mode by clearing the LPR bit in the PWR_CR1 register.
            2. Wait until REGLPF bit is cleared in the PWR_SR2 register.
            3. Increase the system clock frequency
            */
        case 2:
            /* 5.3.5: Low-power sleep mode */
            /* All I/O pins keep the same state as in Run mode. */
            /* The Low-power sleep mode is entered from low-power run mode
            according Section : Entering low power mode, when the SLEEPDEEP
            bit in the Cortex®-M4 System Control register is clear. */
            deep = 0;
            break;
        case 3:
            /* 5.3.6: Stop 0 */
            /* In Stop 0 mode, all clocks in the VCORE domain are stopped;
            the PLL, the MSI, the HSI16 and the HSE oscillators are disabled.
            Some peripherals with the wakeup capability (I2Cx (x=1,2,3), U(S)
            ARTx(x=1,2...5) and LPUART) can switch on the HSI16 to receive a
            frame, and switch off the HSI16 after receiving the frame if it is
            not a wakeup frame. In this case, the HSI16 clock is propagated
            only to the peripheral requesting it.
            SRAM1, SRAM2 and register contents are preserved.
            The BOR is always available in Stop 0 mode. The consumption is increased when thresholds higher than VBOR0 are used. */
            /* All I/O pins keep the same state as in the Run mode. */
            RCC->BDCR |= RCC_BDCR_RTCEN;
            deep = 1;
            break;
        case 4:
            /* 5.3.6: Stop 1 */
            /* NOTE: To enter Stop 1 mode, all EXTI Line pending bits (in
            Pending register 1 (EXTI_PR1)), and the peripheral flags
            generating wakeup interrupts must be cleared. Otherwise, the Stop
            1 mode entry procedure is ignored and program execution continues.
            */
            /* Wakeup latency: Longest wakeup time between: MSI or HSI16
            wakeup time and regulator wakeup time from Low-power mode + Flash
            wakeup time from Stop 1 mode. */
            /* TODO: This is supposed to be a `LPMS = “001” in PWR_CR1`,
            maybe we should actively clear the bits? */
            PWR->CR1 |= PWR_CR1_LPMS_STOP1;
            deep = 1;
            break;
        case 5:
            /* 5.3.8: Stop 2 */
            /* The Stop 2 mode is based on the Cortex®-M4 deepsleep mode
            combined with peripheral clock gating. In Stop 2 mode, all clocks
            in the VCORE domain are stopped, the PLL, the MSI, the HSI16 and
            the HSE oscillators are disabled. Some peripherals with wakeup
            capability (I2C3 and LPUART) can switch on the HSI16 to receive a
            frame, and switch off the HSI16 after receiving the frame if it is
            not a wakeup frame. In this case the HSI16 clock is propagated
            only to the peripheral requesting it.
            SRAM1, SRAM2 and register contents are preserved.
            The BOR is always available in Stop 2 mode. The consumption is
            increased when thresholds higher than VBOR0 are used.*/
            /* Note: To enter Stop 2 mode, all EXTI Line pending bits (in
            Pending register 1 (EXTI_PR1)), and the peripheral flags
            generating wakeup interrupts must be cleared. Otherwise, the Stop
            mode entry procedure is ignored and program execution continues.*/
            RCC->BDCR |= RCC_BDCR_RTCEN;
            /* TODO: This is supposed to be a `LPMS = “010” in PWR_CR1`,
            maybe we should actively clear the bits? */
            PWR->CR1 |= PWR_CR1_LPMS_STOP2;
            deep = 1;
            break;
        case 6:
            /* 5.3.9: Standby mode */
            /* The PLL, the HSI16, the MSI and the HSE oscillators are also
            switched off.*/
            /* SRAM1 and register contents are lost except for registers in
            the Backup domain and Standby circuitry (see Figure 9). SRAM2
            content can be preserved if the bit RRS is set in the PWR_CR3
            register. In this case the Low-power regulator is ON and provides
            the supply to SRAM2 only.*/
            /* In the Standby mode, the I/Os can be configured either with a
            pull-up (refer to PWR_PUCRx registers (x=A,B,C,D,E,F,G,H)), or
            with a pull-down (refer to PWR_PDCRx registers
            (x=A,B,C,D,E,F,G,H)), or can be kept in analog state.
            The RTC outputs on PC13 are functional in Standby mode. PC14 and
            PC15 used for LSE are also functional. 5 wakeup pins (WKUPx,
            x=1,2...5) and the 3 RTC tampers are available.*/
            RCC->BDCR |= RCC_BDCR_RTCEN;
            /* TODO: This is supposed to be a `LPMS = “011” in PWR_CR1`,
            maybe we should actively clear the bits? */
            PWR->CR1 |= PWR_CR1_LPMS_STANDBY;
            /* WUFx bits are cleared in power status register 1 (PWR_SR1) */
            PWR->SR1 &= ~(PWR_SR1_WUF5 | PWR_SR1_WUF4 | PWR_SR1_WUF3 | PWR_SR1_WUF2 | PWR_SR1_WUF1);
            deep = 1;
            break;
        case 7:
            /* 5.3.10: Shutdown mode */
            /* The Shutdown mode allows to achieve the lowest power
            consumption. It is based on the deepsleep mode, with the voltage
            regulator disabled. The VCORE domain is consequently powered off.
            The PLL, the HSI16, the MSI, the LSI and the HSE oscillators are
            also switched off.
            SRAM1, SRAM2 and register contents are lost except for registers
            in the Backup domain. The BOR is not available in Shutdown mode.
            No power voltage monitoring is possible in this mode, therefore
            the switch to Backup domain is not supported. */
            /* In the Shutdown mode, the I/Os can be configured either with a
            pull-up (refer to PWR_PUCRx registers (x=A,B,C,D,E,F,G,H), or with
            a pull-down (refer to PWR_PDCRx registers (x=A,B,C,D,E,F,G,H)), or
            can be kept in analog state. However this configuration is lost
            when exiting the Shutdown mode due to the power-on reset.
            The RTC outputs on PC13 are functional in Shutdown mode. PC14 and
            PC15 used for LSE are also functional. 5 wakeup pins (WKUPx,
            x=1,2...5) and the 3 RTC tampers are available. */
            RCC->BDCR |= RCC_BDCR_RTCEN;
            /* LPMS = “1XX” in PWR_CR1 */
            PWR->CR1 |= PWR_CR1_LPMS_SHUTDOWN;
            /* WUFx bits are cleared in power status register 1 (PWR_SR1) */
            PWR->SR1 &= ~(PWR_SR1_WUF5 | PWR_SR1_WUF4 | PWR_SR1_WUF3 | PWR_SR1_WUF2 | PWR_SR1_WUF1);
            deep = 1;
            break;
        // default:
        //     /* Run mode or reset? */
        //     deep = 0;
        //     break;
    }
#endif

    cortexm_sleep(deep);
}

#if defined(CPU_FAM_STM32F1) || defined(CPU_FAM_STM32F2) || defined(CPU_FAM_STM32F4)
void pm_off(void)
{
    irq_disable();
    pm_set(0);
}
#endif
