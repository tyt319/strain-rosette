#include "ads131m08.h"
#include "main.h"
#include "apm32f4xx_dal.h"
#include <string.h>

/* 外部SPI句柄 */
extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;

/* ------------------- 【唯一】数组定义区域 (仅在此处定义) ------------------- */
static const uint16_t g_cs_pins[ADS131M08_NUM_CHIPS] = ADS131M08_CS_PINS;

// 寄存器地址数组定义
const uint8_t CH_CFG_ADDR[8]     = {0x09, 0x0E, 0x13, 0x18, 0x1D, 0x22, 0x27, 0x2C};
const uint8_t OCAL_MSB_ADDR[8]   = {0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x23, 0x28, 0x2D};
const uint8_t OCAL_LSB_ADDR[8]   = {0x0B, 0x10, 0x15, 0x1A, 0x1F, 0x24, 0x29, 0x2E};
const uint8_t GCAL_MSB_ADDR[8]   = {0x0C, 0x11, 0x16, 0x1B, 0x20, 0x25, 0x2A, 0x2F};
const uint8_t GCAL_LSB_ADDR[8]   = {0x0D, 0x12, 0x17, 0x1C, 0x21, 0x26, 0x2B, 0x30};
// 偏置校准值数组定义(12)
uint32_t ads131m08_ocal_values[ADS131M08_NUM_CHIPS][8] = {
    // ---------- 芯片 0 ----------
    {0x00631E, 0x0065BA, 0x008429, 0x007C55, 0x004E40, 0x004BA5, 0x009749, 0x006BD0},

    // ---------- 芯片 1 ----------
    {0x00A297, 0x008429, 0x006935, 0x007108, 0x006AF2, 0x00598E, 0x008E98, 0x007561},

    // ---------- 芯片 2 ----------
    {0x006935, 0x006E6C, 0x006CAF, 0x006AF2, 0x004D62, 0x00771E, 0x006AF2, 0x006240},

    // ---------- 芯片 3 ----------
    {0x005FA4, 0x008B1D, 0x006161, 0x006D8D, 0x005378, 0x005457, 0x006083, 0x005B4B}
};
// 增益校准值数组定义
uint32_t ads131m08_gcal_values[ADS131M08_NUM_CHIPS][8] = {
    // ---------- 芯片 0 ----------
    {0x0081A6BA, 0x00817EA2, 0x008284FD, 0x00824152, 0x0080E5D6, 0x0080B79C, 0x0083064F, 0x0081DC56},
    // ---------- 芯片 1 ----------
    {0x0083663E, 0x00825598, 0x0081BAD0, 0x0081CEEB, 0x0081A00A, 0x0081004F, 0x0082ADB8, 0x00820B63},
    // ---------- 芯片 2 ----------
    {0x0081F07C, 0x0081C183, 0x0081D5A0, 0x0081C183, 0x00810D8F, 0x008233D2, 0x0081A6BA, 0x00818BFD},
    // ---------- 芯片 3 ----------
    {0x00816AA0, 0x00827E35, 0x00815D4C, 0x0081CEEB, 0x00812175, 0x00810D8F, 0x00814954, 0x008156A3}
};

#define ADS131M08_DISCARD_FRAMES   3u
#define ADS131M08_SIGN_BIT        0x00800000u
#define ADS131M08_SIGN_EXT_MASK   0xFF000000u

static inline uint32_t Bytes3ToU32(const uint8_t *p)
{
    return ((uint32_t)p[0] << 16) | ((uint32_t)p[1] << 8) | p[2];
}

/* ------------------- 内部变量 ------------------- */
static volatile uint8_t  g_dma_chip_idx;
static volatile bool     g_dma_busy = false;
static volatile bool     g_dma_round_done = false;
static volatile uint8_t  g_discard_count = ADS131M08_DISCARD_FRAMES;
static volatile bool     g_drdy_was_high = true;
static ADS131M08_Frame_t *g_dma_frames_ptr = NULL;
static ADS131M08_RxCpltCallback g_rx_cplt_cb = NULL;
static volatile bool     g_is_reg_mode = false;
static volatile bool     g_dma_reg_done = false;
static uint8_t           g_reg_tx_buf[ADS131M08_FRAME_BYTES] __attribute__((aligned(4)));
static uint8_t           g_reg_rx_buf[ADS131M08_FRAME_BYTES] __attribute__((aligned(4)));
static uint8_t           g_current_reg_cs_idx = 0;
static uint8_t g_dma_tx_buf[ADS131M08_NUM_CHIPS][ADS131M08_FRAME_BYTES] __attribute__((aligned(4)));
static uint8_t g_dma_rx_buf[ADS131M08_NUM_CHIPS][ADS131M08_FRAME_BYTES] __attribute__((aligned(4)));
ADS131M08_Frame_t adc_frames[ADS131M08_NUM_CHIPS] = {0};

/* ------------------- 微秒延时 ------------------- */
void ads_Delay_us(uint32_t us)
{
    uint32_t cycles = us * (SystemCoreClock / 1000000 / 5);
    while(cycles--) __NOP();
}

/* ------------------- DMA通道强制恢复 ------------------- */
void DMA_Channel_Recovery(DMA_HandleTypeDef *hdma)
{
    if(hdma == NULL) return;

    //强制禁用 DMA 通道
    __DAL_DMA_DISABLE(hdma);

    //复位软件状态机与错误码
    hdma->State = DAL_DMA_STATE_READY;
    hdma->ErrorCode = DAL_DMA_ERROR_NONE;

    //释放锁
    __DAL_UNLOCK(hdma);
}

/* ------------------- CS控制 ------------------- */
void ADS131M08_CS_Low(uint8_t chip_idx)
{
    if(chip_idx < ADS131M08_NUM_CHIPS) {
        DAL_GPIO_WritePin(ADS131M08_CS_PORT, g_cs_pins[chip_idx], GPIO_PIN_RESET);
    }
}

void ADS131M08_CS_High(uint8_t chip_idx)
{
    if(chip_idx < ADS131M08_NUM_CHIPS) {
        DAL_GPIO_WritePin(ADS131M08_CS_PORT, g_cs_pins[chip_idx], GPIO_PIN_SET);
    }
}

/* ------------------- 统一DMA启动准备 ------------------- */
static void Prepare_SPI_DMA_For_Transfer(void)
{
    // 1. 恢复 DMA 通道状态
    DMA_Channel_Recovery(&hdma_spi1_tx);
    DMA_Channel_Recovery(&hdma_spi1_rx);
    
}

/* ------------------- 核心SPI帧传输 (全DMA版，同步阻塞) ------------------- */
void ADS131M08_SPI_TransferFrame(uint8_t chip_idx, ADS131M08_Frame_t *tx_frame, ADS131M08_Frame_t *rx_frame)
{
    // 1. 检查是否正在进行数据采集，如果是则等待
    // if(g_dma_busy) return;
    while(g_dma_busy);

    // 2. 准备数据
    memset(g_reg_tx_buf, 0, ADS131M08_FRAME_BYTES);
    memset(g_reg_rx_buf, 0, ADS131M08_FRAME_BYTES);

    if(tx_frame != NULL)
    {
        for(uint8_t i=0; i<ADS131M08_FRAME_WORDS; i++)
        {
            g_reg_tx_buf[i*3 + 0] = (tx_frame->raw[i] >> 16) & 0xFF;
            g_reg_tx_buf[i*3 + 1] = (tx_frame->raw[i] >> 8) & 0xFF;
            g_reg_tx_buf[i*3 + 2] = tx_frame->raw[i] & 0xFF;
        }
    }

    // 3. 统一准备 SPI 和 DMA 状态
    Prepare_SPI_DMA_For_Transfer();

    // 4. 设置寄存器模式标志
    g_is_reg_mode = true;
    g_dma_reg_done = false;
    g_current_reg_cs_idx = chip_idx;

    // 5. 拉片选，启动 DMA
    ADS131M08_CS_Low(chip_idx);
    if(DAL_SPI_TransmitReceive_DMA(&hspi1, g_reg_tx_buf, g_reg_rx_buf, ADS131M08_FRAME_BYTES) != DAL_OK)
    {
        ADS131M08_CS_High(chip_idx);
        g_is_reg_mode = false;
        return;
    }

    // 6. 等待 DMA 完成 (同步阻塞)
    while(g_dma_reg_done == false);

    // 7. 解析接收到的数据
    if(rx_frame != NULL)
    {
        for(uint8_t i=0; i<ADS131M08_FRAME_WORDS; i++)
        {
            rx_frame->raw[i] = Bytes3ToU32(&g_reg_rx_buf[i * 3]);
        }
    }
}

/* ------------------- 寄存器解锁 ------------------- */
static void ADS131M08_Unlock(uint8_t chip_idx)
{
    ADS131M08_Frame_t tx_frame = {0};
    tx_frame.status_word = (uint32_t)ADS131M08_CMD_UNLOCK << 8;
    ADS131M08_SPI_TransferFrame(chip_idx, &tx_frame, NULL);
    DAL_Delay(1);
}

/* ------------------- 寄存器读写 ------------------- */
uint16_t ADS131M08_WriteReg(uint8_t chip_idx, uint8_t reg_addr, uint16_t reg_val)
{
    ADS131M08_Frame_t tx_frame = {0}, rx_frame = {0};
    uint16_t wreg_cmd = ADS131M08_CMD_WREG | ((reg_addr & 0x3F) << 7);
    tx_frame.status_word = (uint32_t)wreg_cmd << 8;
    tx_frame.ch_data[0] = (uint32_t)reg_val << 8;

    ADS131M08_SPI_TransferFrame(chip_idx, &tx_frame, NULL);
    DAL_Delay(1);
    ADS131M08_SPI_TransferFrame(chip_idx, NULL, &rx_frame);
    return (rx_frame.status_word >> 8) & 0xFFFF;
}

uint16_t ADS131M08_ReadReg(uint8_t chip_idx, uint8_t reg_addr)
{
    ADS131M08_Frame_t tx_frame = {0}, rx_frame = {0};
    uint16_t rreg_cmd = ADS131M08_CMD_RREG | ((reg_addr & 0x3F) << 7);
    tx_frame.status_word = (uint32_t)rreg_cmd << 8;

    ADS131M08_SPI_TransferFrame(chip_idx, &tx_frame, NULL);
    DAL_Delay(1);
    ADS131M08_SPI_TransferFrame(chip_idx, NULL, &rx_frame);
    return (rx_frame.status_word >> 8) & 0xFFFF;
}

/* ------------------- 初始化单颗芯片 ------------------- */
static void ADS131M08_InitSingle(uint8_t chip_idx)
{
    ADS131M08_Unlock(chip_idx);
    ADS131M08_WriteReg(chip_idx, ADS131M08_REG_CLOCK, CLOCK_ALL_CH_DISABLE);
    DAL_Delay(1);
    ADS131M08_WriteReg(chip_idx, ADS131M08_REG_GAIN1, GAIN_32);
    ADS131M08_WriteReg(chip_idx, ADS131M08_REG_GAIN2, GAIN_32);
    DAL_Delay(1);
    ADS131M08_WriteReg(chip_idx, ADS131M08_REG_MODE, MODE_CONFIG_LEVEL_24BIT);
    DAL_Delay(1);
    ADS131M08_WriteReg(chip_idx, ADS131M08_REG_CLOCK, CLOCK_EXT_CLK_EXT_REF);
    DAL_Delay(1);
    for(uint8_t ch=0; ch<8; ch++)
    {
        ADS131M08_WriteReg(chip_idx, CH_CFG_ADDR[ch], CH_MUX_EXTERNAL);
        DAL_Delay(1);
    }

    ADS131M08_SPI_TransferFrame(chip_idx, NULL, NULL);
    ADS131M08_SPI_TransferFrame(chip_idx, NULL, NULL);
}

/* ------------------- 批量初始化 ------------------- */
void ADS131M08_InitAll(void)
{
    // DAL_Delay(1000);
    DAL_GPIO_WritePin(ADS131M08_SYNC_PORT, ADS131M08_SYNC_PIN, GPIO_PIN_RESET);
    DAL_Delay(10);
    DAL_GPIO_WritePin(ADS131M08_SYNC_PORT, ADS131M08_SYNC_PIN, GPIO_PIN_SET);
    DAL_Delay(100);

    for(uint8_t i=0; i<ADS131M08_NUM_CHIPS; i++)
    {
        ADS131M08_InitSingle(i);
    }
}

/* ===================== DMA 核心逻辑 ===================== */

static void ADS131M08_StartNextDMA(void)
{
    uint8_t idx = g_dma_chip_idx;

    // 恢复 DMA 通道 (禁能硬件通道 + 复位状态机, 确保寄存器可安全写入)
    Prepare_SPI_DMA_For_Transfer();

    // g_dma_tx_buf 是静态BSS段, 编译时零初始化, 采集过程从不修改, 无需 memset

    // 拉片选，启动 DMA
    ADS131M08_CS_Low(idx);
    DAL_SPI_TransmitReceive_DMA(&hspi1, g_dma_tx_buf[idx], g_dma_rx_buf[idx], ADS131M08_FRAME_BYTES);
}

void ADS131M08_DMA_TxRxCpltCallback(void)
{
    if (g_is_reg_mode)
    {
        ADS131M08_CS_High(g_current_reg_cs_idx);
        Prepare_SPI_DMA_For_Transfer();
        g_dma_reg_done = true;
        g_is_reg_mode = false;
        return;
    }

    if (!g_dma_busy) return;

    ADS131M08_CS_High(g_dma_chip_idx);

    ADS131M08_Frame_t *frame = &g_dma_frames_ptr[g_dma_chip_idx];
    for (int i = 0; i < ADS131M08_FRAME_WORDS; i++)
    {
        frame->raw[i] = Bytes3ToU32(&g_dma_rx_buf[g_dma_chip_idx][i * 3]);
    }

    if (g_dma_chip_idx + 1 >= ADS131M08_NUM_CHIPS)
    {
        g_dma_busy = false;
        g_dma_round_done = true;
    }
    else
    {
        g_dma_chip_idx++;
        ADS131M08_StartNextDMA();
    }
}

/* 启动异步读取 (首次调用用于设置回调并触发第一轮 DMA) */
void ADS131M08_ReadAllChips_Async(ADS131M08_Frame_t *frames_array, ADS131M08_RxCpltCallback cb)
{
    if (g_dma_busy || g_is_reg_mode)
        return;

    g_dma_frames_ptr = frames_array;
    g_rx_cplt_cb = cb;
    g_dma_chip_idx = 0;
    g_dma_busy = true;

    ADS131M08_StartNextDMA();
}

void ADS131M08_ProcessRound(void)
{
    if (g_dma_round_done)
    {
        g_dma_round_done = false;

        if (g_discard_count < ADS131M08_DISCARD_FRAMES)
        {
            g_discard_count++;
        }
        else
        {
            for (uint8_t chip = 0; chip < ADS131M08_NUM_CHIPS; chip++)
            {
                for (uint8_t ch = 0; ch < ADS131M08_NUM_CHANNELS; ch++)
                {
                    uint32_t raw = (uint32_t)g_dma_frames_ptr[chip].ch_data[ch];
                    if (raw & ADS131M08_SIGN_BIT)
                        g_dma_frames_ptr[chip].ch_data[ch] = (int32_t)(raw | ADS131M08_SIGN_EXT_MASK);
                }
            }

            if (g_rx_cplt_cb) g_rx_cplt_cb();
        }
    }

    {
        bool drdy_low = (ADS131M08_DRDY_Read() == GPIO_PIN_RESET);

        if (!g_dma_busy && !g_is_reg_mode && !g_dma_round_done
            && g_dma_frames_ptr != NULL
            && g_drdy_was_high && drdy_low)
        {
            g_dma_chip_idx = 0;
            g_dma_busy = true;
            ADS131M08_StartNextDMA();
        }

        g_drdy_was_high = !drdy_low;
    }
}

/* 查询忙状态 */
bool ADS131M08_IsBusy(void)
{
    return g_dma_busy || g_is_reg_mode || g_dma_round_done;
}

void ADS131M08_DRDY_IRQHandler(void)
{
    EINT->IPEND = (uint32_t)GPIO_PIN_0;
}

void ADS131M08_Sync(void)
{
    DAL_GPIO_WritePin(ADS131M08_SYNC_PORT, ADS131M08_SYNC_PIN, GPIO_PIN_RESET);
    ads_Delay_us(1);
    DAL_GPIO_WritePin(ADS131M08_SYNC_PORT, ADS131M08_SYNC_PIN, GPIO_PIN_SET);
    g_discard_count = 0;
}

/* ===================== 自动校准 ===================== */

#define ADC_CAL_NUM_SAMPLES        16u
#define ADC_CAL_DISCARD_FRAMES     4u
#define ADC_CAL_DRDY_TIMEOUT_US    100000u
#define ADC_CAL_POLL_INTERVAL_US   10u
#define ADC_CAL_TEST_UV            5025.0f
#define ADC_CAL_GCAL_NOMINAL       0x800000u

static bool ADS131M08_WaitForDRDY(uint32_t timeout_us)
{
    uint32_t elapsed = 0;
    bool was_high = (ADS131M08_DRDY_Read() != GPIO_PIN_RESET);

    while (elapsed < timeout_us)
    {
        bool is_low = (ADS131M08_DRDY_Read() == GPIO_PIN_RESET);
        if (was_high && is_low)
            return true;
        was_high = !is_low;
        ads_Delay_us(ADC_CAL_POLL_INTERVAL_US);
        elapsed += ADC_CAL_POLL_INTERVAL_US;
    }
    return false;
}

static void ADS131M08_ReadAllChipsSync(ADS131M08_Frame_t frames[ADS131M08_NUM_CHIPS])
{
    for (uint8_t chip = 0; chip < ADS131M08_NUM_CHIPS; chip++)
    {
        ADS131M08_SPI_TransferFrame(chip, NULL, &frames[chip]);
    }
}

static void ADS131M08_WriteAllChMux(uint16_t mux)
{
    for (uint8_t chip = 0; chip < ADS131M08_NUM_CHIPS; chip++)
    {
        ADS131M08_Unlock(chip);
        for (uint8_t ch = 0; ch < 8; ch++)
        {
            ADS131M08_WriteReg(chip, CH_CFG_ADDR[ch], mux);
            DAL_Delay(1);
        }
    }
}

static void ADS131M08_WriteOCALSingle(uint8_t chip, uint8_t ch, uint32_t val)
{
    uint16_t msb = (val >> 8) & 0xFFFF;
    uint16_t lsb = val & 0x00FF;
    ADS131M08_WriteReg(chip, OCAL_MSB_ADDR[ch], msb);
    DAL_Delay(1);
    ADS131M08_WriteReg(chip, OCAL_LSB_ADDR[ch], lsb);
    DAL_Delay(1);
}

static void ADS131M08_WriteGCALSingle(uint8_t chip, uint8_t ch, uint32_t val)
{
    uint16_t msb = (val >> 8) & 0xFFFF;
    uint16_t lsb = val & 0x00FF;
    ADS131M08_WriteReg(chip, GCAL_MSB_ADDR[ch], msb);
    DAL_Delay(1);
    ADS131M08_WriteReg(chip, GCAL_LSB_ADDR[ch], lsb);
    DAL_Delay(1);
}

static bool ADS131M08_CollectAndAverage(uint32_t avg[ADS131M08_NUM_CHIPS][8])
{
    ADS131M08_Frame_t frames[ADS131M08_NUM_CHIPS];
    int64_t acc[ADS131M08_NUM_CHIPS][8];
    memset(acc, 0, sizeof(acc));

    for (uint32_t d = 0; d < ADC_CAL_DISCARD_FRAMES; d++)
    {
        if (!ADS131M08_WaitForDRDY(ADC_CAL_DRDY_TIMEOUT_US))
            return false;
        ADS131M08_ReadAllChipsSync(frames);
    }

    for (uint32_t s = 0; s < ADC_CAL_NUM_SAMPLES; s++)
    {
        if (!ADS131M08_WaitForDRDY(ADC_CAL_DRDY_TIMEOUT_US))
            return false;
        ADS131M08_ReadAllChipsSync(frames);

        for (uint8_t chip = 0; chip < ADS131M08_NUM_CHIPS; chip++)
        {
            for (uint8_t ch = 0; ch < 8; ch++)
            {
                uint32_t raw = (uint32_t)frames[chip].ch_data[ch];
                int32_t val;
                if (raw & ADS131M08_SIGN_BIT)
                    val = (int32_t)(raw | ADS131M08_SIGN_EXT_MASK);
                else
                    val = (int32_t)raw;
                acc[chip][ch] += val;
            }
        }
    }

    for (uint8_t chip = 0; chip < ADS131M08_NUM_CHIPS; chip++)
    {
        for (uint8_t ch = 0; ch < 8; ch++)
        {
            int64_t sum = acc[chip][ch];
            avg[chip][ch] = (uint32_t)((sum + (int64_t)(ADC_CAL_NUM_SAMPLES / 2))
                                       / (int64_t)ADC_CAL_NUM_SAMPLES);
        }
    }
    return true;
}

void ADS131M08_AutoCalibrate(void)
{
    uint32_t avg[ADS131M08_NUM_CHIPS][8];
    float expected_f = (ADC_CAL_TEST_UV / ADC_FULL_SCALE_UV) * 8388608.0f;
    int32_t expected = (int32_t)(expected_f + 0.5f);

    // Phase A: offset calibration (MUX=short)
    ADS131M08_WriteAllChMux(CH_MUX_SHORTED);
    ADS131M08_Sync();
    if (ADS131M08_CollectAndAverage(avg))
    {
        for (uint8_t chip = 0; chip < ADS131M08_NUM_CHIPS; chip++)
        {
            ADS131M08_Unlock(chip);
            for (uint8_t ch = 0; ch < 8; ch++)
            {
                uint32_t ocal = (uint32_t)((int32_t)(avg[chip][ch] & 0x00FFFFFFu));
                ads131m08_ocal_values[chip][ch] = ocal;
                ADS131M08_WriteOCALSingle(chip, ch, ocal);
            }
        }
    }

    // Phase B: gain calibration (MUX=test signal)
    ADS131M08_WriteAllChMux(CH_MUX_POS_TEST);
    ADS131M08_Sync();
    if (ADS131M08_CollectAndAverage(avg))
    {
        for (uint8_t chip = 0; chip < ADS131M08_NUM_CHIPS; chip++)
        {
            ADS131M08_Unlock(chip);
            for (uint8_t ch = 0; ch < 8; ch++)
            {
                int32_t measured = (int32_t)(avg[chip][ch] & 0x00FFFFFFu);
                if (measured == 0) continue;

                uint32_t gcal = (uint32_t)((float)expected / (float)measured
                                           * (float)ADC_CAL_GCAL_NOMINAL + 0.5f);
                if (gcal > 0x00FFFFFFu) gcal = 0x00FFFFFFu;

                ads131m08_gcal_values[chip][ch] = gcal;
                ADS131M08_WriteGCALSingle(chip, ch, gcal);
            }
        }
    }

    // Phase C: restore external input and flush FIFO
    ADS131M08_WriteAllChMux(CH_MUX_EXTERNAL);
    ADS131M08_Sync();
    for (uint8_t chip = 0; chip < ADS131M08_NUM_CHIPS; chip++)
    {
        ADS131M08_SPI_TransferFrame(chip, NULL, NULL);
        ADS131M08_SPI_TransferFrame(chip, NULL, NULL);
    }
}
