#ifndef PWMGEN_H
#define PWMGEN_H

#include <cstdint>
#include <string>
#include <sys/errno.h>

#include "fsl_iomuxc.h"
#include "fsl_qtmr.h"

#include "../module.h"
#include "../../drivers/pin/pin.h"

#include "extern.h"

#define QTMR_BASEADDR              	TMR3
#define QTMR_PRIMARY_SOURCE       	(kQTMR_ClockDivide_2)
#define QTMR_CLOCK_SOURCE_DIVIDER 	(2U)

void createPwmGen(void);

class PwmGen : public Module
{
private:

    int mask;

    uint8_t      	            jointNumber;            // LinuxCNC joint number
    uint8_t                     pwm;                    // PWM number
    uint16_t 	                pwmFreqHz;              // PWM frequency
    uint8_t                     minDutyCycle;           // min duty cycle
    uint8_t                     maxDutyCycle;           // max duty cycle

    volatile int32_t            *ptrFrequencyCommand; 	// pointer to the data source where to get the frequency command
    volatile int32_t            *ptrFeedback;       	// pointer where to put the feedback
    volatile uint8_t            *ptrJointEnable;

    uint32_t                    pwmMask;				// PWM pin location in DR_TOGGLE register
    uint32_t                    dirMask;				// Direction pin location in DR_TOGGLE register

    bool                        isEnabled;        		// flag to enable the PWM generator

    int32_t                     frequencyCommand;       // the joint frequency command generated by LinuxCNC
    float                       dutyCycle;              // current duty cycle
    bool                        direction;              // current direction
    int32_t                     period;       	        // period of the PWM signal

    TMR_Type                    *base = QTMR_BASEADDR;                  // Timer base address
    qtmr_channel_selection_t    channel;                // Timer channel

    bool                        outputPolarity;         // Timer output polarity
    uint32_t                    srcClock_Hz;            // Timer source clock
        
    uint32_t                    periodCount;            // Timer period count
    uint32_t                    highCount;              // Timer high count
    uint32_t                    lowCount;               // Timer low count
    uint16_t                    reg;                    // Timer register

public:
    PwmGen(uint8_t, uint8_t, uint16_t, uint8_t, uint8_t, volatile int32_t&, volatile uint8_t&);  // constructor

    Pin *dirPin;		            // class object member - Pin object

    virtual void update(void);           // Module default interface
    virtual void updatePost(void);
    virtual void slowUpdate(void);
    void makePulses();
    void stopPulses();
    void setEnabled(bool);
};

#endif
