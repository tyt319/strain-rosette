#include "main.h"

#define IS_MASTER_BOARD

/************************ 固定指令 ************************/
#define CMD_CONFIG_ADDR      0x7EE7
#define CMD_READ_ADC_DATA    0x7BB7
#define CMD_WRITE_REG        0x7AA7
#define CMD_READ_REG         0x7CC7

/************************ 硬件定义 ************************/
#define ADC_TOTAL_CHIPS      ADS131M08_NUM_CHIPS
#define ADC_CHANNELS_PER_CHIP 8
#define ADC_DATA_BYTES_PER_CH 3
#define ADC_TOTAL_DATA_BYTES (ADC_TOTAL_CHIPS * ADC_CHANNELS_PER_CHIP * ADC_DATA_BYTES_PER_CH)

/************************ 全局缓存 ************************/
static uint8_t adc_raw_data[ADC_TOTAL_CHIPS][ADC_CHANNELS_PER_CHIP * ADC_DATA_BYTES_PER_CH] = {0};

/************************ 外部变量 ************************/
extern USBD_INFO_T gUsbDeviceFS;
extern USBD_CDC_VCP_INFO_T gUsbVCP;
extern uint8_t cdcRxBuffer[USBD_CDC_RX_BUF_LEN];
extern SPI_HandleTypeDef hspi1;
extern ADS131M08_Frame_t adc_frames[ADS131M08_NUM_CHIPS];

typedef enum
{
    USER_APP_INIT,
    USER_APP_RUN,
} USER_APP_STATE_T;

/************************ 函数声明 ************************/
static void ParseCommand(uint8_t *buf, uint16_t len);
static void ReadAllADCDataToBuffer(void);
static void SendString(const char *str);
static int32_t RawToDecimal(uint8_t *data);  // 数组3字节 → 十进制

/* ========== 异步读取完成标志及回调 ========== */
static volatile bool adc_data_ready = false;

void ADC_ReadCompleteCallback(void)
{
    adc_data_ready = true;
}

/************************ 主函数 ************************/
int main(void)
{
    DAL_DeviceConfig();
    DAL_RCM_MCOConfig(RCM_MCO1, RCM_MCO1SOURCE_HSE, RCM_MCODIV_1);
    ADS131M08_InitAll();
    BridgeCal_Init();
    ADS131M08_AutoCalibrate();
    ADS131M08_Sync();
    ADS131M08_ReadAllChips_Async(adc_frames, ADC_ReadCompleteCallback);

    while (1)
    {
        ADS131M08_ProcessRound();

        if (adc_data_ready)
        {
            adc_data_ready = false;
            DAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
            ReadAllADCDataToBuffer();
        }

        USB_DevUserApplication();
    }
}

void USB_DevUserApplication(void)
{
    static uint8_t userAppState = USER_APP_INIT;
    switch (userAppState)
    {
        case USER_APP_INIT:
            userAppState = USER_APP_RUN;
            break;
        case USER_APP_RUN:
            if (gUsbVCP.state == USBD_CDC_VCP_REV_UPDATE)
            {
                gUsbVCP.state = USBD_CDC_VCP_IDLE;
                ParseCommand(cdcRxBuffer, gUsbVCP.rxUpdateLen);
            }
            break;
    }
}

static void SendString(const char *str)
{
    USBD_FS_CDC_ItfSend((uint8_t *)str, strlen(str));
}

// ==============================
// 数组 3字节 → 十进制（符号位+数值）
// ==============================
static int32_t RawToDecimal(uint8_t *data)
{
    uint32_t raw = ((uint32_t)data[0] << 16) | ((uint32_t)data[1] << 8) | data[2];
    int32_t val = raw & 0x7FFFFF;
    if (raw & 0x800000) val = -val;
    return val;
}

// ==============================
// 数据存入数组（只算一次）
// ==============================
static void ReadAllADCDataToBuffer(void)
{
    for (uint8_t chip = 0; chip < ADC_TOTAL_CHIPS; chip++)
    {
        for (uint8_t ch = 0; ch < ADC_CHANNELS_PER_CHIP; ch++)
        {
            int32_t adc_code = adc_frames[chip].ch_data[ch];
            float output_val;

            /* ---- 线性校准接口 ---- */
            if (g_brcal_coeff[chip][ch].enabled)
            {
                /* 二次多项式校正: ADC码 → 物理量 */
                output_val = BridgeCal_Apply(chip, ch, adc_code);
            }
            else
            {
                /* 未校准: ADC码 → 电压 μV (保留原有行为) */
                output_val = ((float)adc_code / 8388608.0f) * ADC_FULL_SCALE_UV;
            }

            int32_t voltage_uv = (int32_t)(output_val + 0.5f);

            int32_t abs_val = (voltage_uv >= 0) ? voltage_uv : -voltage_uv;
            uint32_t send_val = (uint32_t)abs_val & 0x007FFFFF;
            if (voltage_uv < 0) send_val |= 0x00800000;

            uint16_t offset = ch * 3;
            adc_raw_data[chip][offset + 0] = (send_val >> 16) & 0xFF;
            adc_raw_data[chip][offset + 1] = (send_val >> 8) & 0xFF;
            adc_raw_data[chip][offset + 2] = send_val & 0xFF;
        }
    }
}

// ==============================
// 指令解析：直接用数组数据！
// ==============================
static void ParseCommand(uint8_t *buf, uint16_t len)
{
    char msg[512];
    uint16_t cmd = (buf[0] << 8) | buf[1];

    if (cmd == CMD_CONFIG_ADDR && len == 3)
    {
        sprintf(msg, "[CMD] ADDR: 0x%02X\r\n", buf[2]);
        SendString(msg);
        return;
    }

    // ==============================
    // 读 ADC
    // ==============================
    if (cmd == CMD_READ_ADC_DATA && len == 3)
    {
        DAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);
        SendString("=====================================\r\n");

        for (uint8_t chip = 0; chip < ADC_TOTAL_CHIPS; chip++)
        {
            char line[256];
            sprintf(line, "CHIP %d: ", chip);

            for (uint8_t ch = 0; ch < ADC_CHANNELS_PER_CHIP; ch++)
            {
                uint8_t *data = &adc_raw_data[chip][ch * 3]; // 直接取数组
                int32_t val = RawToDecimal(data);           // 转十进制

                char temp[32];
                sprintf(temp, "CH%d=%6d ", ch, val);
                strcat(line, temp);
            }

            strcat(line, "\r\n");
            SendString(line);
        }

        SendString("=====================================\r\n\r\n");
        return;
    }

    if (cmd == CMD_WRITE_REG && len == 7)
    {
        uint8_t chip = buf[3], reg = buf[4];
        uint16_t val = (buf[5] << 8) | buf[6];
        uint16_t echo = ADS131M08_WriteReg(chip, reg, val);
        ADS131M08_Sync();
        sprintf(msg, "[WRITE] CHIP%d REG%02X = 0x%04X (ECHO:0x%04X)\r\n", chip, reg, val, echo);
        SendString(msg);
        return;
    }

    if (cmd == CMD_READ_REG && len == 5)
    {
        uint8_t chip = buf[3], reg = buf[4];
        uint16_t val = ADS131M08_ReadReg(chip, reg);
        ADS131M08_Sync();
        sprintf(msg, "[READ]  CHIP%d REG%02X = 0x%04X\r\n", chip, reg, val);
        SendString(msg);
        return;
    }

    SendString("[ERROR] Unknown CMD\r\n");
    DAL_GPIO_TogglePin(GPIOD, GPIO_PIN_2);
}

void DAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
    {
        ADS131M08_DMA_TxRxCpltCallback();
    }
}
