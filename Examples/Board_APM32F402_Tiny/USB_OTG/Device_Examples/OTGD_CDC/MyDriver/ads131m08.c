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
const uint32_t ads131m08_ocal_values[ADS131M08_NUM_CHIPS][8] = {
    // ---------- 芯片 0 ----------
    {0x0050A0, 0x0053C9, 0x007429, 0x006CFD, 0x003E9A, 0x003CA8, 0x0086C4, 0x005CAF},
    
    // ---------- 芯片 1 ----------
    {0x008FDC, 0x00734A, 0x005BA4, 0x006135, 0x005CAF, 0x004A9A, 0x007F4A, 0x0063FD},
    
    // ---------- 芯片 2 ----------
    {0x005822, 0x005D35, 0x005D61, 0x0059BB, 0x003B69, 0x006935, 0x005B78, 0x00531F},
    
    // ---------- 芯片 3 ----------
    {0x004DD7, 0x007B16, 0x0052C6, 0x005DE7, 0x004457, 0x0044DD, 0x0050F9, 0x004B1F}
};
// 增益校准值数组定义
const uint32_t ads131m08_gcal_values[4][8] = {
    // ---------- 芯片 0 ----------
    {0x7EEA63, 0x7E8A4D, 0x7EAF53, 0x7E8543, 0x7F0621, 0x7EA05F, 0x7E7F35, 0x7F4745},
    // ---------- 芯片 1 ----------
    {0x7EB47B, 0x7E465D, 0x7ED299, 0x7E99A5, 0x7EAFD7, 0x7E3997, 0x7E45D9, 0x7EFBC1},
    // ---------- 芯片 2 ----------
    {0x7F9157, 0x7E9E9D, 0x7EBF43, 0x7EC47B, 0x7F1A19, 0x7EED9B, 0x7E8477, 0x7F62A3},
    // ---------- 芯片 3 ----------
    {0x7EAF53, 0x7E989F, 0x7E9DD7, 0x7E8A4D, 0x7F2E8F, 0x7EC993, 0x7E99A5, 0x7F2E8F}
};

/* ------------------- 内部变量 ------------------- */
static volatile uint8_t  g_dma_chip_idx;
static volatile bool     g_dma_busy = false;
static volatile bool     g_dma_round_done = false;  // 一轮 DMA 链已完成, 待 main 处理
static ADS131M08_Frame_t *g_dma_frames_ptr = NULL;
static ADS131M08_RxCpltCallback g_rx_cplt_cb = NULL;
static volatile bool     g_is_reg_mode = false;
static volatile bool     g_dma_reg_done = false;
static volatile uint8_t  g_drdy_edge_count = 0;
static volatile bool     g_drdy_trigger = false;
static volatile bool     g_drdy_enable  = false;
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
            rx_frame->raw[i] = ((uint32_t)g_reg_rx_buf[i*3 + 0] << 16) | 
                               ((uint32_t)g_reg_rx_buf[i*3 + 1] << 8)  | 
                               ((uint32_t)g_reg_rx_buf[i*3 + 2]);
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
    // ADS131M08_WriteReg(chip_idx, ADS131M08_REG_CFG, CFG_VALUE);
    // DAL_Delay(1);
    for(uint8_t ch=0; ch<8; ch++)
    {
        ADS131M08_WriteReg(chip_idx, CH_CFG_ADDR[ch], CH_CFG_VALUE);
        DAL_Delay(1);
    }
    for(uint8_t ch=0; ch<8; ch++)
    {
        uint32_t ocal_val = ads131m08_ocal_values[chip_idx][ch];
        uint32_t gcal_val = ads131m08_gcal_values[chip_idx][ch];
        
        uint16_t ocal_msb = (ocal_val >> 8) & 0xFFFF;
        uint16_t ocal_lsb = ocal_val & 0x00FF;
        uint16_t gcal_msb = (gcal_val >> 8) & 0xFFFF;
        uint16_t gcal_lsb = gcal_val & 0x00FF;

        // 3. 依次写入两个寄存器
        ADS131M08_WriteReg(chip_idx, OCAL_MSB_ADDR[ch], ocal_msb);
        DAL_Delay(1);
        ADS131M08_WriteReg(chip_idx, OCAL_LSB_ADDR[ch], ocal_lsb);
        DAL_Delay(1);
        ADS131M08_WriteReg(chip_idx, GCAL_MSB_ADDR[ch], gcal_msb);
        DAL_Delay(1);
        ADS131M08_WriteReg(chip_idx, GCAL_LSB_ADDR[ch], gcal_lsb);
        DAL_Delay(1);
    }
    
    // 【优化】初始化时就清空FIFO，避免在DMA采集中途调用阻塞函数
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

    ADS131M08_InitDRDY_EXTI();
}

/* ===================== DMA 核心逻辑 ===================== */

/* DRDY EXTI 初始化: PA0 下降沿触发, IMASK 由 ProcessRound 动态门控 */
void ADS131M08_InitDRDY_EXTI(void)
{
    uint32_t regval;

    /* AFIO: 选择 PA 作为 EINT0 的输入源 */
    regval = AFIO->EINTSEL[0];
    regval &= ~AFIO_EINTSEL1_EINT0_Msk;
    regval |= AFIO_EINTSEL1_EINT0_PA;
    AFIO->EINTSEL[0] = regval;

    /* EINT: 使能 line0 下降沿触发 (中断由 ProcessRound 按需开关) */
    EINT->FTEN  |= EINT_IMASK_IMASK0;
}

/* DRDY 中断处理: 在第 5 次下降沿时置触发标志 */
void ADS131M08_DRDY_IRQHandler(void)
{
    /* 清除中断挂起 */
    EINT->IPEND = EINT_IPEND_IPEND0;

    if (g_drdy_enable && !g_dma_busy && !g_is_reg_mode && !g_dma_round_done)
    {
        g_drdy_edge_count++;
        if (g_drdy_edge_count >= 5)
        {
            g_drdy_edge_count = 0;
            g_drdy_trigger = true;
            g_drdy_enable  = false;
        }
    }
}

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
    // ==========================================
    // 分支 1：处理寄存器操作的回调
    // ==========================================
    if (g_is_reg_mode)
    {
        // 1. 立即拉高 CS
        ADS131M08_CS_High(g_current_reg_cs_idx);

        // 2. 统一恢复状态 (为下一次传输做准备)
        Prepare_SPI_DMA_For_Transfer();

        // 3. 标记完成
        g_dma_reg_done = true;
        g_is_reg_mode = false;
        return;
    }

    // ==========================================
    // 分支 2：处理正常数据采集的回调
    // ==========================================
    if (!g_dma_busy) return;

    // 1. 拉高 CS
    ADS131M08_CS_High(g_dma_chip_idx);

    // 2. 判断是否所有芯片传输完成
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

/* DMA 轮次处理：在 main 循环中调用
 * 处理丢弃/捕获/SYNC/符号扩展，并自动触发下一轮 DMA
 * 将耗时操作从中断上下文移至 main 上下文，保持回调轻量 */
void ADS131M08_ProcessRound(void)
{
    /* ---- 处理已完成的 DMA 轮次 ---- */
    if (g_dma_round_done)
    {
        g_dma_round_done = false;

        DAL_GPIO_WritePin(ADS131M08_SYNC_PORT, ADS131M08_SYNC_PIN, GPIO_PIN_RESET);
        ads_Delay_us(1);
        DAL_GPIO_WritePin(ADS131M08_SYNC_PORT, ADS131M08_SYNC_PIN, GPIO_PIN_SET);

        // 将所有芯片的原始字节数据解包为32-bit字并做符号扩展 (从ISR移至main上下文)
        for (uint8_t chip = 0; chip < ADS131M08_NUM_CHIPS; chip++)
        {
            const uint8_t *buf = g_dma_rx_buf[chip];
            uint32_t *out = g_dma_frames_ptr[chip].raw;

            // Word 0: STATUS
            out[0] = ((uint32_t)buf[0] << 16) | ((uint32_t)buf[1] << 8) | buf[2];

            // Words 1~8: CH_DATA (含24-bit符号扩展)
            const uint8_t *p = &buf[3];
            for (int i = 1; i <= 8; i++, p += 3)
            {
                uint32_t val = ((uint32_t)p[0] << 16) | ((uint32_t)p[1] << 8) | p[2];
                if (val & 0x00800000)
                    val |= 0xFF000000;
                out[i] = val;
            }

            // Word 9: CRC
            out[9] = ((uint32_t)buf[27] << 16) | ((uint32_t)buf[28] << 8) | buf[29];
        }

        if (g_rx_cplt_cb) g_rx_cplt_cb();

        /* SYNC 已发送, 启动 DRDY 下降沿计数 */
        g_drdy_edge_count = 0;
        g_drdy_enable = true;
        EINT->IMASK |= EINT_IMASK_IMASK0;
    }

    /* ---- 自动触发下一轮 DMA (由 DRDY 第5次下降沿触发) ---- */
    if (!g_dma_busy && !g_is_reg_mode && !g_dma_round_done
        && g_dma_frames_ptr != NULL
        && g_drdy_trigger)
    {
        EINT->IMASK &= ~EINT_IMASK_IMASK0;
        g_drdy_trigger = false;
        g_dma_chip_idx = 0;
        g_dma_busy = true;
        ADS131M08_StartNextDMA();
    }
}

/* 查询忙状态 */
bool ADS131M08_IsBusy(void)
{
    return g_dma_busy || g_is_reg_mode || g_dma_round_done;
}
