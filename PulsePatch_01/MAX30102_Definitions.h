
/*
  control bytes and register definitions for controling MAX30102
*/

//  SIMBLEE PINS
#define BOARD_LED 25  // on board LED
#define MAX_INT 30    // MAX30102 interrupts on this Simblee pin
#define SCL_PIN 13
#define SDA_PIN 10


//  MAX30102 REGISTERS
#define MAX_ADD   0x57  // slave address
#define STATUS_1  0x00
#define STATUS_2  0x01
#define ENABLE_1  0x02
#define ENABLE_2  0x03
#define FIFO_WRITE  0x04
#define OVF_COUNTER 0x05
#define FIFO_READ 0x06
#define FIFO_DATA 0x07
#define FIFO_CONFIG 0x08
#define MODE_CONFIG 0x09
#define SPO2_CONFIG 0x0A
#define RED_PA 0x0C
#define IR_PA 0x0D
#define PILOT_PA  0x10
#define MODE_CNTRL_1  0x11
#define MODE_CNTRL_2  0x12
#define TEMP_INT  0x1F
#define TEMP_FRAC 0x20
#define TEMP_CONFIG 0x21
#define PROX_INT_THRESH 0x30
#define REV_ID  0xFE
#define PART_ID 0xFF

// MASKS
#define A_FULL  0x80
#define PPG_RDY 0x40
#define ALC_OVF 0x20
#define PROX_INT 0x10
#define PWR_RDY 0x01
#define TEMP_RDY  0x02
#define SMP_AVE_1 0x00
#define SMP_AVE_2 0x20
#define SMP_AVE_4 0x40
#define SMP_AVE_8 0x60
#define SMP_AVE_16 0x80
#define SMP_AVE_32 0xA0
#define ROLLOVER_EN 0x10
#define SHUTDOWN  0x80
#define RESET   0x40
#define HR_MODE 0x02
#define SPO2_MODE 0x03
#define MULTI_MODE 0x07
#define ADC_RGE_2048  0x00
#define ADC_RGE_4096  0x20
#define ADC_RGE_8192  0x40
#define ADC_RGE_16348 0x60
#define SR_50 0x00
#define SR_100  0x04
#define SR_200  0x08
#define SR_400  0x0C
#define SR_800  0x10
#define SR_1000 0x14
#define SR_1600 0x18
#define SR_3200 0x1C
#define PW_69 0x00
#define PW_118  0x01
#define PW_215  0x02
#define PW_411  0x03
#define TEMP_EN 0x01

// filter stuff
#define CUTTOFF_LOW 5.5
#define CUTTOFF_HIGH 10.0
#define NUM_SAMPLES 50
