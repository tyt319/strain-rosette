#ifndef __ADS131M08_H
#define __ADS131M08_H

#ifdef __cplusplus
extern "C" {
#endif

#include "apm32f4xx.h"
#include <stdint.h>
#include <stdbool.h>

// ===================== 四芯片配置 =====================
#define ADS131M08_NUM_CHIPS    4
#define ADS131M08_NUM_CHANNELS 8
#define ADS131M08_FRAME_WORDS  10
#define ADS131M08_FRAME_BYTES  (ADS131M08_FRAME_WORDS * 3)

// ===================== 核心配置 =====================
#define ADC_V_REF_MV          1206.0f
#define ADC_PGA_GAIN          32
#define ADC_FULL_SCALE_UV     ((ADC_V_REF_MV / (float)ADC_PGA_GAIN) * 1000.0f)

// ------------------- GPIO引脚定义 -------------------
#define ADS131M08_CS_PORT      GPIOC
#define ADS131M08_CS_PINS      {GPIO_PIN_1, GPIO_PIN_11, GPIO_PIN_0, GPIO_PIN_12}
#define ADS131M08_DRDY_PIN     GPIO_PIN_0
#define ADS131M08_DRDY_PORT    GPIOA
#define ADS131M08_DRDY_Read()  DAL_GPIO_ReadPin(ADS131M08_DRDY_PORT, ADS131M08_DRDY_PIN)
#define ADS131M08_SYNC_PIN     GPIO_PIN_1
#define ADS131M08_SYNC_PORT    GPIOA

/* ------------------- 寄存器定义 ------------------- */
#define ADS131M08_CMD_NULL     0x0000
#define ADS131M08_CMD_RESET    0x0011
#define ADS131M08_CMD_STANDBY  0x0022
#define ADS131M08_CMD_WAKEUP   0x0033
#define ADS131M08_CMD_LOCK     0x0555
#define ADS131M08_CMD_UNLOCK   0x0655
#define ADS131M08_CMD_RREG     0xA000
#define ADS131M08_CMD_WREG     0x6000

#define ADS131M08_REG_ID       0x00
#define ADS131M08_REG_STATUS   0x01
#define ADS131M08_REG_MODE     0x02
#define ADS131M08_REG_CLOCK    0x03
#define ADS131M08_REG_GAIN1    0x04
#define ADS131M08_REG_GAIN2    0x05
#define ADS131M08_REG_CFG      0x06

#define CLOCK_EXT_CLK_EXT_REF    0xFFCE
#define CLOCK_ALL_CH_DISABLE      0x000E
#define MODE_CONFIG_LEVEL_24BIT   (0x0100 | 0x0010 | 0x0002)
#define GAIN_32                    0x5555
#define CFG_VALUE           0x0700

#define CH_MUX_EXTERNAL      0x0000u
#define CH_MUX_SHORTED       0x0001u
#define CH_MUX_POS_TEST      0x0002u
#define CH_MUX_NEG_TEST      0x0003u

/* ------------------- 数据帧结构体 ------------------- */
typedef union
{
    uint32_t raw[ADS131M08_FRAME_WORDS];
    struct __attribute__((packed))
    {
        uint32_t status_word;
        int32_t  ch_data[ADS131M08_NUM_CHANNELS];
        uint32_t crc_word;
    };
} ADS131M08_Frame_t;

/* ------------------- 全局变量声明 (extern) ------------------- */
extern ADS131M08_Frame_t adc_frames[ADS131M08_NUM_CHIPS];
// 【关键】声明偏置数组，不分配内存
extern uint32_t ads131m08_ocal_values[ADS131M08_NUM_CHIPS][8];
extern uint32_t ads131m08_gcal_values[ADS131M08_NUM_CHIPS][8];
extern const uint8_t CH_CFG_ADDR[8];
extern const uint8_t OCAL_MSB_ADDR[8];
extern const uint8_t OCAL_LSB_ADDR[8];
extern const uint8_t GCAL_MSB_ADDR[8];
extern const uint8_t GCAL_LSB_ADDR[8];

/* ------------------- 函数声明 ------------------- */
void ads_Delay_us(uint32_t us);
void ADS131M08_CS_Low(uint8_t chip_idx);
void ADS131M08_CS_High(uint8_t chip_idx);
void ADS131M08_SPI_TransferFrame(uint8_t chip_idx, ADS131M08_Frame_t *tx_frame, ADS131M08_Frame_t *rx_frame);
void ADS131M08_InitAll(void);
uint16_t ADS131M08_WriteReg(uint8_t chip_idx, uint8_t reg_addr, uint16_t reg_val);
uint16_t ADS131M08_ReadReg(uint8_t chip_idx, uint8_t reg_addr);

typedef void (*ADS131M08_RxCpltCallback)(void);
void ADS131M08_ReadAllChips_Async(ADS131M08_Frame_t *frames_array, ADS131M08_RxCpltCallback cb);
bool ADS131M08_IsBusy(void);
void ADS131M08_DMA_TxRxCpltCallback(void);
void ADS131M08_ProcessRound(void);
void ADS131M08_Sync(void);
void ADS131M08_DRDY_IRQHandler(void);
void ADS131M08_AutoCalibrate(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADS131M08_H */
