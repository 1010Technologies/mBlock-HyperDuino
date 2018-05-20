#ifndef MPR121_REGISTERS_H
#define MPR121_REGISTERS_H

// touch and OOR statuses
#define MPR121_TS1 0x00
#define MPR121_TS2 0x01
#define MPR121_OORS1 0x02
#define MPR121_OORS2 0x03

// filtered data
#define MPR121_E0FDL 0x04
#define MPR121_E0FDH 0x05
#define MPR121_E1FDL 0x06
#define MPR121_E1FDH 0x07
#define MPR121_E2FDL 0x08
#define MPR121_E2FDH 0x09
#define MPR121_E3FDL 0x0A
#define MPR121_E3FDH 0x0B
#define MPR121_E4FDL 0x0C
#define MPR121_E4FDH 0x0D
#define MPR121_E5FDL 0x0E
#define MPR121_E5FDH 0x0F
#define MPR121_E6FDL 0x10
#define MPR121_E6FDH 0x11
#define MPR121_E7FDL 0x12
#define MPR121_E7FDH 0x13
#define MPR121_E8FDL 0x14
#define MPR121_E8FDH 0x15
#define MPR121_E9FDL 0x16
#define MPR121_E9FDH 0x17
#define MPR121_E10FDL 0x18
#define MPR121_E10FDH 0x19
#define MPR121_E11FDL 0x1A
#define MPR121_E11FDH 0x1B
#define MPR121_E12FDL 0x1C
#define MPR121_E12FDH 0x1D

// baseline values
#define MPR121_E0BV 0x1E
#define MPR121_E1BV 0x1F
#define MPR121_E2BV 0x20
#define MPR121_E3BV 0x21
#define MPR121_E4BV 0x22
#define MPR121_E5BV 0x23
#define MPR121_E6BV 0x24
#define MPR121_E7BV 0x25
#define MPR121_E8BV 0x26
#define MPR121_E9BV 0x27
#define MPR121_E10BV 0x28
#define MPR121_E11BV 0x29
#define MPR121_E12BV 0x2A

// general electrode touch sense baseline filters
// rising filter
#define MPR121_MHDR 0x2B
#define MPR121_NHDR 0x2C
#define MPR121_NCLR 0x2D
#define MPR121_FDLR 0x2E

// falling filter
#define MPR121_MHDF 0x2F
#define MPR121_NHDF 0x30
#define MPR121_NCLF 0x31
#define MPR121_FDLF 0x32

// touched filter
#define MPR121_NHDT 0x33
#define MPR121_NCLT 0x34
#define MPR121_FDLT 0x35

// proximity electrode touch sense baseline filters
// rising filter
#define MPR121_MHDPROXR 0x36
#define MPR121_NHDPROXR 0x37
#define MPR121_NCLPROXR 0x38
#define MPR121_FDLPROXR 0x39

// falling filter
#define MPR121_MHDPROXF 0x3A
#define MPR121_NHDPROXF 0x3B
#define MPR121_NCLPROXF 0x3C
#define MPR121_FDLPROXF 0x3D

// touched filter
#define MPR121_NHDPROXT 0x3E
#define MPR121_NCLPROXT 0x3F
#define MPR121_FDLPROXT 0x40

// electrode touch and release thresholds
#define MPR121_E0TTH 0x41
#define MPR121_E0RTH 0x42
#define MPR121_E1TTH 0x43
#define MPR121_E1RTH 0x44
#define MPR121_E2TTH 0x45
#define MPR121_E2RTH 0x46
#define MPR121_E3TTH 0x47
#define MPR121_E3RTH 0x48
#define MPR121_E4TTH 0x49
#define MPR121_E4RTH 0x4A
#define MPR121_E5TTH 0x4B
#define MPR121_E5RTH 0x4C
#define MPR121_E6TTH 0x4D
#define MPR121_E6RTH 0x4E
#define MPR121_E7TTH 0x4F
#define MPR121_E7RTH 0x50
#define MPR121_E8TTH 0x51
#define MPR121_E8RTH 0x52
#define MPR121_E9TTH 0x53
#define MPR121_E9RTH 0x54
#define MPR121_E10TTH 0x55
#define MPR121_E10RTH 0x56
#define MPR121_E11TTH 0x57
#define MPR121_E11RTH 0x58
#define MPR121_E12TTH 0x59
#define MPR121_E12RTH 0x5A

// debounce settings
#define MPR121_DTR 0x5B

// configuration registers
#define MPR121_AFE1 0x5C
#define MPR121_AFE2 0x5D
#define MPR121_ECR 0x5E

// electrode currents
#define MPR121_CDC0 0x5F
#define MPR121_CDC1 0x60
#define MPR121_CDC2 0x61
#define MPR121_CDC3 0x62
#define MPR121_CDC4 0x63
#define MPR121_CDC5 0x64
#define MPR121_CDC6 0x65
#define MPR121_CDC7 0x66
#define MPR121_CDC8 0x67
#define MPR121_CDC9 0x68
#define MPR121_CDC10 0x69
#define MPR121_CDC11 0x6A
#define MPR121_CDC12 0x6B

// electrode charge times
#define MPR121_CDT01 0x6C
#define MPR121_CDT23 0x6D
#define MPR121_CDT45 0x6E
#define MPR121_CDT67 0x6F
#define MPR121_CDT89 0x70
#define MPR121_CDT1011 0x71
#define MPR121_CDT11 0x72

// GPIO
#define MPR121_CTL0 0x73
#define MPR121_CTL1 0x74
#define MPR121_DAT 0x75
#define MPR121_DIR 0x76
#define MPR121_EN 0x77
#define MPR121_SET 0x78
#define MPR121_CLR 0x79
#define MPR121_TOG 0x7A

// auto-config 
#define MPR121_ACCR0 0x7B
#define MPR121_ACCR1 0x7C
#define MPR121_USL 0x7D
#define MPR121_LSL 0x7E
#define MPR121_TL 0x7F

// soft reset
#define MPR121_SRST 0x80

// PWM
#define MPR121_PWM0 0x81
#define MPR121_PWM1 0x82
#define MPR121_PWM2 0x83
#define MPR121_PWM3 0x84

#endif
