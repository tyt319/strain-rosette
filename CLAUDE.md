# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

APM32F4xx DAL SDK V1.2.0 — Geehy Semiconductor's MCU SDK for APM32F402/407/465 series (ARM Cortex-M4F). Currently working on the `Board_APM32F402_Tiny` USB OTG CDC device example with ADS131M08 24-bit ADC data acquisition over SPI+DMA, bridge sensor calibration, and USB CDC virtual COM port output.

## Build System

- **IDE**: Keil MDK (μVision 5). Project file: `Project/MDK/OTGD_CDC.uvprojx`
- **Compiler**: ARMCC (Keil toolchain). DSP extensions and FPU enabled.
- **No CLI build** — compile and flash from Keil IDE only.

## Architecture

### DAL / DDL layering

The SDK has two driver layers, both in `Libraries/APM32F4xx_DAL_Driver/`:

- **DAL** (HAL-style): `apm32f4xx_dal_*.c` — high-level peripheral drivers with state machines (SPI, DMA, GPIO, NVIC). Use `DAL_` prefixed functions.
- **DDL** (LL-style): `apm32f4xx_ddl_*.h` — static-inline register-level macros. Use `DDL_` prefixed functions for direct register access without enabling the DAL module.

Module enable/disable is controlled by `#define DAL_XXX_MODULE_ENABLED` in `Config/Include/apm32f4xx_dal_cfg.h`.

### Project structure (under `Examples/Board_APM32F402_Tiny/USB_OTG/Device_Examples/OTGD_CDC/`)

| Directory | Role |
|---|---|
| `Source/main.c` | Main loop: calls `ADS131M08_ProcessRound()`, handles USB CDC, parses UART commands |
| `MyDriver/ads131m08.c/h` | ADS131M08 ADC driver: 4-chip SPI+DMA read, DRDY EXTI edge counting, 24→32 bit unpacking |
| `MyDriver/bridge_cal.c/h` | Bridge sensor calibration: quadratic polynomial correction per chip/channel |
| `Config/Source/apm32f4xx_*_cfg.c` | Hardware config: GPIO, SPI, DMA, NVIC |
| `Config/Include/apm32f4xx_*_cfg.h` | Config headers |
| `Source/apm32f4xx_int.c` | All ISR handlers (SysTick, USB, SPI1, EINT0) |
| `Source/usbd_*.c` | USB CDC device stack |

### Data flow

```
ADS131M08 (4 chips, SPI1+DMA, 10-word frames)
  → DMA ISR chains through 4 chips (CS control per chip)
  → ProcessRound (main context): SYNC pulse → byte unpacking + 24-bit sign extension → user callback
  → main.c callback: bridge calibration → 24-bit packed output → adc_raw_data buffer
  → USB CDC: ParseCommand → reads adc_raw_data → sends formatted text over VCP
```

### Key constraints

- **DMA callback runs in SPI1 IRQ context** (priority 3,0). Must be lightweight — heavy operations (byte unpacking) moved to `ProcessRound` in main context.
- **DRDY EXTI (PA0, EINT0_IRQn, priority 2,2)** counts 5 falling edges to time the next DMA read, replacing a fixed 650µs delay.
- **SYNC pin (PA1)** pulses before each round to synchronize all 4 ADCs.
- **DAL_EINT_MODULE_ENABLED is NOT defined** — EXTI is configured via direct register access (AFIO->EINTSEL, EINT->FTEN/IMASK). This is intentional to avoid enabling another DAL module.
- **`g_dma_rx_buf[4][30]`** is a 2D buffer — one 30-byte slot per chip. DMA writes each chip's slot independently, so data persists until the next round's DMA.

## Common Patterns

- Direct register access for peripherals without DAL module: `EINT->FTEN |= EINT_IMASK_IMASK0`
- Volatile flags for ISR ↔ main-loop communication: `g_drdy_trigger`, `g_dma_round_done`
- The `Prepare_SPI_DMA_For_Transfer()` + `DMA_Channel_Recovery()` pattern resets DMA state machine and releases locks before each transfer
- CS pins are manually controlled (not hardware NSS) for multi-chip SPI
- GPIO init pattern: declare `GPIO_InitTypeDef GPIO_InitStruct = {0U}`, set fields, call `DAL_GPIO_Init()`
