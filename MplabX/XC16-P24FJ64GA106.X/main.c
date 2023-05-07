// Configuration Registers.
#pragma config WPFP = WPFP511, WPDIS = WPDIS, WPCFG = WPCFGDIS
#pragma config WPEND = WPENDMEM, POSCMOD = EC, IOL1WAY = ON, OSCIOFNC = OFF
#pragma config FCKSM = CSDCMD, FNOSC = PRI, IESO = OFF, WDTPS = PS32768
#pragma config FWPSA = PR128, WINDIS = OFF, FWDTEN = OFF, ICS = PGx2
#pragma config GWRP = OFF, GCP = OFF, JTAGEN = OFF

// MCU Frequency.
#define FOSC    (24576000UL)
#define FCY     (FOSC/2)

#include <xc.h>
#include <libpic30.h>
#include <p24FJ64GA106.h>
// PIC24FJ64GA106 - Compile with XC16(v2.10).
// PIC24FJ64GA106 - @24.576MHz External Oscillator.
// v0.1 - xx/2023.

// TCAR - Rev.A.

// TODO: EUSART Tx
// TODO: EUSART Rx
// TODO: Read Battery
// TODO: Motor function VBAT
// TODO: Motor Ramp
// TODO: Motor Encoder

// Pinout.
// MCU.RB1 -> LED1.
// MCU.RB2 -> LED2.
// MCU.RF3 -> LED3.
// MCU.RF2 -> LED4.
// MCU.RB5 <- SWTICH1.
// MCU.RB4 <- SWTICH2.
// MCU.RF5 <- SWTICH3.
// MCU.RF4 <- SWTICH4.

// Definitions.
// IR Encoder Motor.
#define ENCODER_M12_DISABLE        LATDbits.LATD6 = 0b0
#define ENCODER_M12_ENABLE         LATDbits.LATD6 = 0b1
#define ENCODER_M34_DISABLE        LATBbits.LATB10 = 0b0
#define ENCODER_M34_ENABLE         LATBbits.LATB10 = 0b1
// LED.
#define LED1_OFF                   LATBbits.LATB1 = 0b0
#define LED1_ON                    LATBbits.LATB1 = 0b1
#define LED2_OFF                   LATBbits.LATB2 = 0b0
#define LED2_ON                    LATBbits.LATB2 = 0b1
#define LED3_OFF                   LATFbits.LATF3 = 0b0
#define LED3_ON                    LATFbits.LATF3 = 0b1
#define LED4_OFF                   LATFbits.LATF2 = 0b0
#define LED4_ON                    LATFbits.LATF2 = 0b1
// Motor.
#define MOTOR_PWM_FREQUENCY         ((FCY/15000)-1)
#define MOTOR_PWM_DUTY_CYCLE_STOP   410
#define MOTOR_PWM_FRONT_LEFT        OC1R
#define MOTOR_PWM_FRONT_RIGHT       OC2R
#define MOTOR_PWM_REAR_LEFT         OC3R
#define MOTOR_PWM_REAR_RIGHT        OC4R
#define MOTOR_STOP_ENGAGE           LATDbits.LATD7 = 0b0
#define MOTOR_STOP_RELEASE          LATDbits.LATD7 = 0b1
#define MOTOR_BRAKE_ENGAGE          LATBbits.LATB13 = 0b1
#define MOTOR_BRAKE_RELEASE         LATBbits.LATB13 = 0b0
// Switch.
#define SWITCH1                     PORTBbits.RB5
#define SWITCH2                     PORTBbits.RB4
#define SWITCH3                     PORTFbits.RF5
#define SWITCH4                     PORTFbits.RF4

// Main.
int main(void)
{
    // MCU Initialization.
    // Oscillator Settings.
    CLKDIVbits.DOZE = 0b00;
    CLKDIVbits.DOZEN = 0b0;
    // Ports Initialization.
    // Analog Inputs Settings.
    AD1PCFGL = 0b1111111111111110;
    AD1PCFGH = 0b0000000000000011;
    // PORTB Settings.
    TRISB = 0b1000000100110001;
    PORTB = 0b0000000000000000;
    LATB = 0b0000000000000000;
    ODCB = 0b0000000000000000;
    // PORTC Settings.
    TRISC = 0b0000000000000000;
    PORTC = 0b0000000000000000;
    LATC = 0b0000000000000000;
    ODCC = 0b0000000000000000;
    // PORTD Settings.
    TRISD = 0b0000000000000110;
    PORTD = 0b0000000000000000;
    LATD = 0b0000000000000000;
    ODCD = 0b0000000000000000;
    // PORTE Settings.
    TRISE = 0b0000000000000000;
    PORTE = 0b0000000000000000;
    LATE = 0b0000000000000000;
    ODCE = 0b0000000000000000;
    // PORTF Settings.
    TRISF = 0b0000000000110000;
    PORTF = 0b0000000000000000;
    LATF = 0b0000000000000000;
    ODCF = 0b0000000000000000;
    // PORTG Settings.
    TRISG = 0b0000000000000000;
    PORTG = 0b0000000000000000;
    LATG = 0b0000000000000000;
    ODCG = 0b0000000000000000;
    // PPS Initialization.
    __builtin_write_OSCCONL(OSCCON & 0xBF);
    // PPS Inputs Settings.
    RPINR7bits.IC1R = 0x17;		// RD2  - RP23 - IC1R.
    RPINR7bits.IC2R = 0x18;		// RD1  - RP24 - IC2R.
    RPINR8bits.IC3R = 0x8;		// RB8  - RP8  - IC3R.
    RPINR8bits.IC4R = 0x1D;		// RB15 - RP29 - IC4R.
    // PPS Outputs Settings.
    RPOR10bits.RP20R = 0x12;    // RD5  - RP20 - OC1.
    RPOR12bits.RP25R = 0x13;    // RD4  - RP25 - OC2.
    RPOR4bits.RP9R = 0x14;      // RB9  - RP9  - OC3.
    RPOR7bits.RP14R = 0x15;     // RB14 - RP14 - OC4.
    __builtin_write_OSCCONL(OSCCON | 0x40);

    // InputCapture1 Settings.
	IC1CON1bits.ICSIDL = 0b1;
	IC1CON1bits.ICTSEL = 0b111;
	IC1CON1bits.ICI = 0b00;
	IC1CON1bits.ICM = 0b011;
	IC1CON2bits.IC32 = 0b0;
	IC1CON2bits.ICTRIG = 0b0;
	IC1CON2bits.SYNCSEL = 0b00000;

    // InputCapture2 Settings.
	IC2CON1bits.ICSIDL = 0b1;
	IC2CON1bits.ICTSEL = 0b111;
	IC2CON1bits.ICI = 0b00;
	IC2CON1bits.ICM = 0b011;
	IC2CON2bits.IC32 = 0b0;
	IC2CON2bits.ICTRIG = 0b0;
	IC2CON2bits.SYNCSEL = 0b00000;

    // InputCapture3 Settings.
	IC3CON1bits.ICSIDL = 0b1;
	IC3CON1bits.ICTSEL = 0b111;
	IC3CON1bits.ICI = 0b00;
	IC3CON1bits.ICM = 0b011;
	IC3CON2bits.IC32 = 0b0;
	IC3CON2bits.ICTRIG = 0b0;
	IC3CON2bits.SYNCSEL = 0b00000;

    // InputCapture4 Settings.
	IC4CON1bits.ICSIDL = 0b1;
	IC4CON1bits.ICTSEL = 0b111;
	IC4CON1bits.ICI = 0b00;
	IC4CON1bits.ICM = 0b011;
	IC4CON2bits.IC32 = 0b0;
	IC4CON2bits.ICTRIG = 0b0;
	IC4CON2bits.SYNCSEL = 0b00000;

    // OutputCompare1 Settings.
    OC1CON1bits.OCSIDL = 0b1;
    OC1CON1bits.OCTSEL = 0b111;
    OC1CON1bits.ENFLT0 = 0b0;
    OC1CON1bits.TRIGMODE = 0b0;
    OC1CON1bits.OCM = 0b110;
    OC1CON2bits.FLTMD = 0b0;
    OC1CON2bits.FLTOUT = 0b0;
    OC1CON2bits.FLTTRIEN = 0b0;
    OC1CON2bits.OCINV = 0b0;
    OC1CON2bits.OC32 = 0b0;
    OC1CON2bits.OCTRIG = 0b0;
    OC1CON2bits.OCTRIS = 0b0;
    OC1CON2bits.SYNCSEL = 0b11111;
    OC1RS = MOTOR_PWM_FREQUENCY;
    OC1R = MOTOR_PWM_DUTY_CYCLE_STOP;

    // OutputCompare2 Settings.
    OC2CON1bits.OCSIDL = 0b1;
    OC2CON1bits.OCTSEL = 0b111;
    OC2CON1bits.ENFLT0 = 0b0;
    OC2CON1bits.TRIGMODE = 0b0;
    OC2CON1bits.OCM = 0b110;
    OC2CON2bits.FLTMD = 0b0;
    OC2CON2bits.FLTOUT = 0b0;
    OC2CON2bits.FLTTRIEN = 0b0;
    OC2CON2bits.OCINV = 0b0;
    OC2CON2bits.OC32 = 0b0;
    OC2CON2bits.OCTRIG = 0b0;
    OC2CON2bits.OCTRIS = 0b0;
    OC2CON2bits.SYNCSEL = 0b11111;
    OC2RS = MOTOR_PWM_FREQUENCY;
    OC2R = MOTOR_PWM_DUTY_CYCLE_STOP;

    // OutputCompare3 Settings.
    OC3CON1bits.OCSIDL = 0b1;
    OC3CON1bits.OCTSEL = 0b111;
    OC3CON1bits.ENFLT0 = 0b0;
    OC3CON1bits.TRIGMODE = 0b0;
    OC3CON1bits.OCM = 0b110;
    OC3CON2bits.FLTMD = 0b0;
    OC3CON2bits.FLTOUT = 0b0;
    OC3CON2bits.FLTTRIEN = 0b0;
    OC3CON2bits.OCINV = 0b0;
    OC3CON2bits.OC32 = 0b0;
    OC3CON2bits.OCTRIG = 0b0;
    OC3CON2bits.OCTRIS = 0b0;
    OC3CON2bits.SYNCSEL = 0b11111;
    OC3RS = MOTOR_PWM_FREQUENCY;
    OC3R = MOTOR_PWM_DUTY_CYCLE_STOP;

    // OutputCompare4 Settings.
    OC4CON1bits.OCSIDL = 0b1;
    OC4CON1bits.OCTSEL = 0b111;
    OC4CON1bits.ENFLT0 = 0b0;
    OC4CON1bits.TRIGMODE = 0b0;
    OC4CON1bits.OCM = 0b110;
    OC4CON2bits.FLTMD = 0b0;
    OC4CON2bits.FLTOUT = 0b0;
    OC4CON2bits.FLTTRIEN = 0b0;
    OC4CON2bits.OCINV = 0b0;
    OC4CON2bits.OC32 = 0b0;
    OC4CON2bits.OCTRIG = 0b0;
    OC4CON2bits.OCTRIS = 0b0;
    OC4CON2bits.SYNCSEL = 0b11111;
    OC4RS = MOTOR_PWM_FREQUENCY;
    OC4R = MOTOR_PWM_DUTY_CYCLE_STOP;

    // Motors Stop & Brakes Release.
    MOTOR_STOP_ENGAGE;
    MOTOR_BRAKE_RELEASE;

    // Encoders Enable.
    ENCODER_M12_ENABLE;
    ENCODER_M34_ENABLE;

    while(1){
        if(!SWITCH1){
            __delay_us(5);
            MOTOR_STOP_RELEASE;
            MOTOR_PWM_FRONT_LEFT = 20;
            LED1_ON;
        }else{
            LED1_OFF;
            MOTOR_PWM_FRONT_LEFT = 410;
        }
        if(!SWITCH2){
            __delay_us(5);
            MOTOR_STOP_RELEASE;
            MOTOR_PWM_REAR_LEFT = 20;
            LED2_ON;
        }else{
            LED2_OFF;
            MOTOR_PWM_REAR_LEFT = 410;
        }
        if(!SWITCH3){
            __delay_us(5);
            MOTOR_STOP_RELEASE;
            MOTOR_PWM_FRONT_RIGHT = 20;
            LED3_ON;
        }else{
            LED3_OFF;
            MOTOR_PWM_FRONT_RIGHT = 410;
        }
        if(!SWITCH4){
            __delay_us(5);
            MOTOR_STOP_RELEASE;
            MOTOR_PWM_REAR_RIGHT = 20;
            LED4_ON;
        }else{
            LED4_OFF;
            MOTOR_PWM_REAR_RIGHT = 410;
        }
    }
    return(0);
}