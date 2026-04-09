/*******************************************************************************
<<<<<<<< HEAD:src/config/default/peripheral/adc/plib_adc.h
  Analog-to-Digital Converter(ADC) PLIB
========
  Timer/Counter(TC5) PLIB
>>>>>>>> development:src/config/default/peripheral/tc/plib_tc5.h

  Company
    Microchip Technology Inc.

  File Name
<<<<<<<< HEAD:src/config/default/peripheral/adc/plib_adc.h
    plib_adc.h

  Summary
    ADC PLIB Header File.

  Description
    This file defines the interface to the ADC peripheral library. This
========
    plib_tc5.h

  Summary
    TC5 PLIB Header File.

  Description
    This file defines the interface to the TC peripheral library. This
>>>>>>>> development:src/config/default/peripheral/tc/plib_tc5.h
    library provides access to and control of the associated peripheral
    instance.

  Remarks:
    None.

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

<<<<<<<< HEAD:src/config/default/peripheral/adc/plib_adc.h
#ifndef PLIB_ADC_H      // Guards against multiple inclusion
#define PLIB_ADC_H
========
#ifndef PLIB_TC5_H      // Guards against multiple inclusion
#define PLIB_TC5_H
>>>>>>>> development:src/config/default/peripheral/tc/plib_tc5.h

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/

#include "device.h"
<<<<<<<< HEAD:src/config/default/peripheral/adc/plib_adc.h
#include "plib_adc_common.h"
========
#include "plib_tc_common.h"
>>>>>>>> development:src/config/default/peripheral/tc/plib_tc5.h

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
/* The following data type definitions are used by the functions in this
    interface and should be considered part it.
*/

<<<<<<<< HEAD:src/config/default/peripheral/adc/plib_adc.h
// *****************************************************************************


========
>>>>>>>> development:src/config/default/peripheral/tc/plib_tc5.h
// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************
/* The following functions make up the methods (set of possible operations) of
    this interface.
*/

<<<<<<<< HEAD:src/config/default/peripheral/adc/plib_adc.h
void ADC_Initialize( void );

void ADC_Enable( void );

void ADC_Disable( void );

void ADC_ChannelSelect( ADC_POSINPUT positiveInput, ADC_NEGINPUT negativeInput );

void ADC_ConversionStart( void );

uint16_t ADC_ConversionResultGet( void );

void ADC_ComparisonWindowSet(uint16_t low_threshold, uint16_t high_threshold);

void ADC_WindowModeSet(ADC_WINMODE mode);

void ADC_InterruptsClear(ADC_STATUS interruptMask);

void ADC_InterruptsEnable(ADC_STATUS interruptMask);

void ADC_InterruptsDisable(ADC_STATUS interruptMask);


void ADC_CallbackRegister( ADC_CALLBACK callback, uintptr_t context );



========
// *****************************************************************************

void TC5_TimerInitialize( void );

void TC5_TimerStart( void );

void TC5_TimerStop( void );

uint32_t TC5_TimerFrequencyGet( void );


void TC5_Timer16bitPeriodSet( uint16_t period );

uint16_t TC5_Timer16bitPeriodGet( void );

uint16_t TC5_Timer16bitCounterGet( void );

void TC5_Timer16bitCounterSet( uint16_t count );




void TC5_TimerCallbackRegister( TC_TIMER_CALLBACK callback, uintptr_t context );


void TC5_TimerCommandSet(TC_COMMAND command);

>>>>>>>> development:src/config/default/peripheral/tc/plib_tc5.h

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END

<<<<<<<< HEAD:src/config/default/peripheral/adc/plib_adc.h
#endif /* PLIB_ADC_H */
========
#endif /* PLIB_TC5_H */
>>>>>>>> development:src/config/default/peripheral/tc/plib_tc5.h
