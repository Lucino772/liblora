#ifndef _LIBLORA_SX127x_CONFIG_H_
#define _LIBLORA_SX127x_CONFIG_H_

/****************** OpModes *******************/
#define SX127x_OPMODE_SLEEP                 0x00
#define SX127x_OPMODE_STDBY                 0x01
#define SX127x_OPMODE_FSTX                  0x02
#define SX127x_OPMODE_TX                    0x03
#define SX127x_OPMODE_FSRX                  0x04
#define SX127x_OPMODE_RX_CONT               0x05
#define SX127x_OPMODE_RX_SINGLE             0x06
#define SX127x_OPMODE_CAD                   0x07

/***************** Bandwidth ******************/
#define SX127x_BW_7_8                       0x00
#define SX127x_BW_10_4                      0x01
#define SX127x_BW_15_6                      0x02
#define SX127x_BW_20_8                      0x03
#define SX127x_BW_31_25                     0x04
#define SX127x_BW_41_7                      0x05
#define SX127x_BW_62_5                      0x06
#define SX127x_BW_125                       0x07
#define SX127x_BW_250                       0x08
#define SX127x_BW_500                       0x09

/************* Spreading Factor ***************/
#define SX127x_SF_6                         0x06
#define SX127x_SF_7                         0x07
#define SX127x_SF_8                         0x08
#define SX127x_SF_9                         0x09
#define SX127x_SF_10                        0x10
#define SX127x_SF_11                        0x11
#define SX127x_SF_12                        0x12

/*************** Coding Rate ******************/
#define SX127x_CR_45                        0x01
#define SX127x_CR_46                        0x02
#define SX127x_CR_47                        0x03
#define SX127x_CR_48                        0x04

/******************* PA RAMP ******************/
#define SX127x_PA_RAMP_3_4MS                0x00
#define SX127x_PA_RAMP_2MS                  0x01
#define SX127x_PA_RAMP_1MS                  0x02
#define SX127x_PA_RAMP_500US                0x03
#define SX127x_PA_RAMP_250US                0x04
#define SX127x_PA_RAMP_125US                0x05
#define SX127x_PA_RAMP_100US                0x06
#define SX127x_PA_RAMP_62US                 0x07
#define SX127x_PA_RAMP_50US                 0x08
#define SX127x_PA_RAMP_40US                 0x09
#define SX127x_PA_RAMP_31US                 0x0A
#define SX127x_PA_RAMP_25US                 0x0B
#define SX127x_PA_RAMP_20US                 0x0C
#define SX127x_PA_RAMP_15US                 0x0D
#define SX127x_PA_RAMP_12US                 0x0E
#define SX127x_PA_RAMP_10US                 0x0F

/****************** IRQ (Mask) ****************/
#define SX127x_IRQ_CAD_DETECTED             0x01 // -------1
#define SX127x_IRQ_FHSS_CHANGE_CHAN         0x02 // ------1-
#define SX127x_IRQ_CAD_DONE                 0x04 // -----1--
#define SX127x_IRQ_TX_DONE                  0x08 // ----1---
#define SX127x_IRQ_VALID_HEADER             0x10 // ---1----
#define SX127x_IRQ_PAYLOAD_CRC_ERR          0x20 // --1-----
#define SX127x_IRQ_RX_DONE                  0x40 // -1------
#define SX127x_IRQ_RX_TIMEOUT               0x80 // 1-------

/**************** DIO Mapping *****************/
#define SX127x_DIO0_RX_DONE                 0x00 // 00------
#define SX127x_DIO0_TX_DONE                 0x01 // 01------
#define SX127x_DIO0_CAD_DONE                0x02 // 10------
#define SX127x_DIO0_NOP                     0xC0 // 11------

#define SX127x_DIO1_RX_TIMEOUT              0x00 // --00----
#define SX127x_DIO1_FHSS_CHANGE_CHAN        0x10 // --01----
#define SX127x_DIO1_CAD_DETECTED            0x20 // --10----
#define SX127x_DIO1_NOP                     0x30 // --11----

#define SX127x_DIO2_FHSS_CHANGE_CHAN        0x00 // ----00--
#define SX127x_DIO2_NOP                     0x0C // ----11--

#define SX127x_DIO3_CAD_DONE                0x00 // ------00
#define SX127x_DIO3_VALID_HEADER            0x01 // ------01
#define SX127x_DIO3_PAYLOAD_CRC_ERR         0x02 // ------10
#define SX127x_DIO3_NOP                     0x03 // ------11

#endif