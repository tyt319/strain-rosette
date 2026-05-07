/**
 * @file    bridge_cal.h
 * @brief   惠斯通电桥 / 应变片 线性校准接口
 *
 * 本模块提供:
 *   - 二次多项式拟合校准 (float + 定点 Q16.16 双版本)
 *   - 零点偏移 (tare) 校准
 *   - 每通道独立使能 / 系数
 *
 * 标定流程:
 *   1. 硬件校零: 空载时记录各通道 ADC 值, 写入 zero_offset
 *   2. 施加已知载荷, 记录 (ADC_raw, 真实物理量) 对
 *   3. 用最小二乘拟合求 a0/a1/a2, 写入系数并使能
 *   4. 可选: 若输入已经是电压 μV, 系数直接作用在 μV 上
 */

#ifndef __BRIDGE_CAL_H
#define __BRIDGE_CAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "ads131m08.h"

/* ===================== 多晶常数 ===================== */
#define BRCAL_NUM_CHIPS    ADS131M08_NUM_CHIPS   /* 4 */
#define BRCAL_NUM_CHANNELS ADS131M08_NUM_CHANNELS /* 8 */

/* ===================== 二次多项式系数结构体 ===================== */
typedef struct
{
    float    a0, a1, a2;   /* y = a0 + a1*x + a2*x^2  (x = ADC 原始码或电压) */
    bool     enabled;       /* 该通道是否启用校准 */
    int32_t  zero_offset;   /* 零点偏置 (ADC 原始码), 校准前先减去 */
} BridgeCalCoeff_t;

/* ===================== 定点 Q16.16 系数 (无浮点版) ===================== */
typedef struct
{
    int32_t  a0_q16;       /* a0 << 16 */
    int32_t  a1_q16;       /* a1 << 16 */
    int32_t  a2_q16;       /* a2 << 16 */
    bool     enabled;
    int32_t  zero_offset;  /* 零点偏置 (ADC 原始码) */
} BridgeCalCoeffFixed_t;

/* ===================== 全局校准表 ===================== */
/* 每个芯片每个通道一组系数; 可放在 Flash 中预存 */
extern BridgeCalCoeff_t      g_brcal_coeff[BRCAL_NUM_CHIPS][BRCAL_NUM_CHANNELS];
extern BridgeCalCoeffFixed_t g_brcal_coeff_fixed[BRCAL_NUM_CHIPS][BRCAL_NUM_CHANNELS];

/* ===================== API ===================== */

/**
 * @brief  初始化校准模块
 * @note   将所有通道设为 passthrough (a0=0, a1=1, a2=0, enabled=false, offset=0)
 *         上电时调用一次即可
 */
void BridgeCal_Init(void);

/**
 * @brief  设置某通道的浮点校准系数并立即使能
 * @param  chip  芯片索引 [0..3]
 * @param  ch    通道索引 [0..7]
 * @param  a0    零阶系数 (单位: 物理量)
 * @param  a1    一阶系数
 * @param  a2    二阶系数
 */
void BridgeCal_SetCoeff(uint8_t chip, uint8_t ch, float a0, float a1, float a2);

/**
 * @brief  设置某通道的定点 Q16.16 校准系数并立即使能
 * @param  chip   芯片索引 [0..3]
 * @param  ch     通道索引 [0..7]
 * @param  a0_q16 a0 左移16位
 * @param  a1_q16 a1 左移16位
 * @param  a2_q16 a2 左移16位
 */
void BridgeCal_SetCoeffFixed(uint8_t chip, uint8_t ch,
                             int32_t a0_q16, int32_t a1_q16, int32_t a2_q16);

/**
 * @brief  启用 / 禁用某通道校准
 */
void BridgeCal_Enable(uint8_t chip, uint8_t ch);
void BridgeCal_Disable(uint8_t chip, uint8_t ch);

/**
 * @brief  设置零点偏置 (空载 ADC 码), 并存入校准表
 * @note   校准公式: corrected = cal( adc_raw - zero_offset )
 */
void BridgeCal_SetZeroOffset(uint8_t chip, uint8_t ch, int32_t offset);

/**
 * @brief  捕获当前所有通道的 ADC 值为零点 (需要传入 adc_frames)
 * @param  frames 当前有效的 ADC 帧数组指针
 */
void BridgeCal_TareAll(const ADS131M08_Frame_t *frames);

/* ========== 校正计算 ========== */

/**
 * @brief  浮点校正: ADC 原始码 → 物理量
 * @param  chip 芯片索引
 * @param  ch   通道索引
 * @param  adc_raw ADC 原始 24 位有符号码值
 * @return 校正后的物理量 (单位由标定系数决定)
 */
float BridgeCal_Apply(uint8_t chip, uint8_t ch, int32_t adc_raw);

/**
 * @brief  定点校正 (Q16.16): ADC 原始码 → Q16.16 物理量
 * @param  chip 芯片索引
 * @param  ch   通道索引
 * @param  adc_raw ADC 原始 24 位有符号码值
 * @return 校正后的物理量 (Q16.16 格式, 需右移16位得到整数部分)
 */
int32_t BridgeCal_ApplyFixed(uint8_t chip, uint8_t ch, int32_t adc_raw);

/**
 * @brief  定点校正, 输入/输出均为浮点 (内部自动选择浮点实现)
 */
static inline float BridgeCal_ApplyFloat(uint8_t chip, uint8_t ch, int32_t adc_raw)
{
    return BridgeCal_Apply(chip, ch, adc_raw);
}

/* ========== 电桥解析解 (可选) ========== */

/**
 * @brief  由 1/4 桥电压推算出真实应变
 * @param  vo      桥输出电压 (mV)
 * @param  vex     激励电压 (mV)
 * @param  gf      应变片灵敏系数 (典型 2.0)
 * @return 应变 ε (无单位, 微应变需 × 1e6)
 */
float BridgeCal_StrainFromVoltage(float vo, float vex, float gf);

#ifdef __cplusplus
}
#endif

#endif /* __BRIDGE_CAL_H */
