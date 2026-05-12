/**
 * @file    bridge_cal.c
 * @brief   惠斯通电桥 / 应变片 线性校准接口 实现
 */

#include "bridge_cal.h"
#include <string.h>

/* ===================== 全局校准表定义 (唯一) ===================== */
BridgeCalCoeff_t      g_brcal_coeff[BRCAL_NUM_CHIPS][BRCAL_NUM_CHANNELS];
BridgeCalCoeffFixed_t g_brcal_coeff_fixed[BRCAL_NUM_CHIPS][BRCAL_NUM_CHANNELS];

/* ===================== 预设系数表 (用户在此配置默认校准参数, 存于 Flash) ===================== */
const BridgeCalCoeffFixed_t g_brcal_presets[BRCAL_NUM_CHIPS][BRCAL_NUM_CHANNELS] = {
    /* CHIP 0 ────────────────────────────── */
    {
        {       0, 1 << 16,       0,           0,   false},  /* CH0 */
        {       0, 1 << 16,       0,           0,   false},  /* CH1 */
        {       0, 1 << 16,       0,           0,   false},  /* CH2 */
        {       0, 1 << 16,       0,           0,   false},  /* CH3 */
        {       0, 1 << 16,       0,           0,   false},  /* CH4 */
        {       0, 1 << 16,       0,           0,   false},  /* CH5 */
        {       0, 1 << 16,       0,           0,   false},  /* CH6 */
        {       0, 1 << 16,       0,           0,   false},  /* CH7 */
    },
    /* CHIP 1 ────────────────────────────── */
    {
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
    },
    /* CHIP 2 ────────────────────────────── */
    {
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
    },
    /* CHIP 3 ────────────────────────────── */
    {
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
        {       0, 1 << 16,       0,           0,   false},
    },
};

void BridgeCal_LoadPresets(void)
{
    for (uint8_t chip = 0; chip < BRCAL_NUM_CHIPS; chip++)
    {
        for (uint8_t ch = 0; ch < BRCAL_NUM_CHANNELS; ch++)
        {
            const BridgeCalCoeffFixed_t *p = &g_brcal_presets[chip][ch];
            if (!p->enabled) continue;

            BridgeCal_SetCoeffFixed(chip, ch, p->a0_q16, p->a1_q16, p->a2_q16);
            BridgeCal_SetZeroOffset(chip, ch, p->zero_offset);
        }
    }
}

/* ===================== 初始化 ===================== */
void BridgeCal_Init(void)
{
    for (uint8_t chip = 0; chip < BRCAL_NUM_CHIPS; chip++)
    {
        for (uint8_t ch = 0; ch < BRCAL_NUM_CHANNELS; ch++)
        {
            /* 浮点: 线性 passthrough */
            g_brcal_coeff[chip][ch].a0          = 0.0f;
            g_brcal_coeff[chip][ch].a1          = 1.0f;
            g_brcal_coeff[chip][ch].a2          = 0.0f;
            g_brcal_coeff[chip][ch].zero_offset = 0;
            g_brcal_coeff[chip][ch].enabled     = false;

            /* 定点: Q16.16 passthrough */
            g_brcal_coeff_fixed[chip][ch].a0_q16      = 0;
            g_brcal_coeff_fixed[chip][ch].a1_q16      = 1 << 16;   /* 1.0 */
            g_brcal_coeff_fixed[chip][ch].a2_q16      = 0;
            g_brcal_coeff_fixed[chip][ch].zero_offset = 0;
            g_brcal_coeff_fixed[chip][ch].enabled     = false;
        }
    }
}

/* ===================== 系数设置 ===================== */
void BridgeCal_SetCoeff(uint8_t chip, uint8_t ch, float a0, float a1, float a2)
{
    if (chip >= BRCAL_NUM_CHIPS || ch >= BRCAL_NUM_CHANNELS) return;

    g_brcal_coeff[chip][ch].a0      = a0;
    g_brcal_coeff[chip][ch].a1      = a1;
    g_brcal_coeff[chip][ch].a2      = a2;
    g_brcal_coeff[chip][ch].enabled = true;
}

void BridgeCal_SetCoeffFixed(uint8_t chip, uint8_t ch,
                             int32_t a0_q16, int32_t a1_q16, int32_t a2_q16)
{
    if (chip >= BRCAL_NUM_CHIPS || ch >= BRCAL_NUM_CHANNELS) return;

    g_brcal_coeff_fixed[chip][ch].a0_q16  = a0_q16;
    g_brcal_coeff_fixed[chip][ch].a1_q16  = a1_q16;
    g_brcal_coeff_fixed[chip][ch].a2_q16  = a2_q16;
    g_brcal_coeff_fixed[chip][ch].enabled = true;
}

/* ===================== 使能控制 ===================== */
void BridgeCal_Enable(uint8_t chip, uint8_t ch)
{
    if (chip >= BRCAL_NUM_CHIPS || ch >= BRCAL_NUM_CHANNELS) return;
    g_brcal_coeff[chip][ch].enabled       = true;
    g_brcal_coeff_fixed[chip][ch].enabled = true;
}

void BridgeCal_Disable(uint8_t chip, uint8_t ch)
{
    if (chip >= BRCAL_NUM_CHIPS || ch >= BRCAL_NUM_CHANNELS) return;
    g_brcal_coeff[chip][ch].enabled       = false;
    g_brcal_coeff_fixed[chip][ch].enabled = false;
}

/* ===================== 零点偏置 ===================== */
void BridgeCal_SetZeroOffset(uint8_t chip, uint8_t ch, int32_t offset)
{
    if (chip >= BRCAL_NUM_CHIPS || ch >= BRCAL_NUM_CHANNELS) return;
    g_brcal_coeff[chip][ch].zero_offset       = offset;
    g_brcal_coeff_fixed[chip][ch].zero_offset = offset;
}

void BridgeCal_TareAll(const ADS131M08_Frame_t *frames)
{
    if (frames == NULL) return;

    for (uint8_t chip = 0; chip < BRCAL_NUM_CHIPS; chip++)
    {
        for (uint8_t ch = 0; ch < BRCAL_NUM_CHANNELS; ch++)
        {
            int32_t raw = frames[chip].ch_data[ch];
            BridgeCal_SetZeroOffset(chip, ch, raw);
        }
    }
}

/* ===================== 浮点校正 (输入/输出均为 μV) ===================== */
float BridgeCal_Apply(uint8_t chip, uint8_t ch, int32_t adc_raw)
{
    if (chip >= BRCAL_NUM_CHIPS || ch >= BRCAL_NUM_CHANNELS)
        return ((float)adc_raw / BRCAL_ADC_FULLSCALE_F) * ADC_FULL_SCALE_UV;

    BridgeCalCoeff_t *c = &g_brcal_coeff[chip][ch];
    float x = ((float)(adc_raw - c->zero_offset) / BRCAL_ADC_FULLSCALE_F) * ADC_FULL_SCALE_UV;

    if (!c->enabled)
        return x;

    return c->a0 + c->a1 * x + c->a2 * x * x;
}

/* ===================== 定点校正 (Q16.16, 输入/输出均为 μV) ===================== */
int32_t BridgeCal_ApplyFixed(uint8_t chip, uint8_t ch, int32_t adc_raw)
{
    if (chip >= BRCAL_NUM_CHIPS || ch >= BRCAL_NUM_CHANNELS)
        return (int32_t)(((int64_t)adc_raw * BRCAL_ADC_TO_Q16_NUM) >> BRCAL_ADC_TO_Q16_SHIFT);

    BridgeCalCoeffFixed_t *c = &g_brcal_coeff_fixed[chip][ch];
    int32_t x_adc = (int32_t)(adc_raw - c->zero_offset);
    int32_t x = (int32_t)(((int64_t)x_adc * BRCAL_ADC_TO_Q16_NUM) >> BRCAL_ADC_TO_Q16_SHIFT);

    if (!c->enabled)
        return x;

    int64_t result = (int64_t)c->a0_q16;
    result += ((int64_t)c->a1_q16 * x) >> 16;
    result += ((int64_t)c->a2_q16 * ((int64_t)x * x >> 16)) >> 16;
    return (int32_t)result;
}

/* ===================== 电桥解析解 ===================== */
float BridgeCal_StrainFromVoltage(float vo, float vex, float gf)
{
    if (vex == 0.0f) return 0.0f;

    float ratio = vo / vex;

    /*
     * 1/4 桥:  ε = 4·(Vo/Vex) / [ GF · (2 - Vo/Vex) ]
     */
    return (4.0f * ratio) / (gf * (2.0f - ratio));
}
