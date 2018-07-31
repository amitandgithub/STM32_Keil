

#ifndef DRIVER_GPIO_H_
#define DRIVER_GPIO_H_

#ifdef  __cplusplus
extern "C"
{
#endif

#include "Driver_Common.h"
#include "stm32f10x_gpio.h"

#define ARM_GPIO_API_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(2,3)  /* API version */


/****** GPIO Control Codes *****/

#define ARM_GPIO_OWN_ADDRESS             (0x01)      ///< Set Own Slave Address; arg = address 
#define ARM_GPIO_BUS_SPEED               (0x02)      ///< Set Bus Speed; arg = speed
#define ARM_GPIO_BUS_CLEAR               (0x03)      ///< Execute Bus clear: send nine clock pulses
#define ARM_GPIO_ABORT_TRANSFER          (0x04)      ///< Abort Master/Slave Transmit/Receive

/*----- GPIO Bus Speed -----*/
#define ARM_GPIO_BUS_SPEED_STANDARD      (0x01)      ///< Standard Speed (100kHz)
#define ARM_GPIO_BUS_SPEED_FAST          (0x02)      ///< Fast Speed     (400kHz)
#define ARM_GPIO_BUS_SPEED_FAST_PLUS     (0x03)      ///< Fast+ Speed    (  1MHz)
#define ARM_GPIO_BUS_SPEED_HIGH          (0x04)      ///< High Speed     (3.4MHz)


/****** GPIO Address Flags *****/

#define ARM_GPIO_ADDRESS_10BIT           (0x0400)    ///< 10-bit address flag
#define ARM_GPIO_ADDRESS_GC              (0x8000)    ///< General Call flag


/**
\brief GPIO Status
*/
typedef volatile struct _ARM_GPIO_STATUS {
  uint32_t on             : 1;        ///< Busy flag
  uint32_t off             : 1;        ///< Mode: 0=Slave, 1=Master
  uint32_t direction        : 1;        ///< Direction: 0=Transmitter, 1=Receiver
  uint32_t reserved         : 29;
} ARM_GPIO_STATUS;


/****** GPIO Event *****/
#define ARM_GPIO_EVENT_ON       (1UL << 0)  ///< Master/Slave Transmit/Receive finished
#define ARM_GPIO_EVENT_OFF      (1UL << 1)  ///< Master/Slave Transmit/Receive incomplete transfer



// Function documentation
/**
  \fn          ARM_DRIVER_VERSION ARM_GPIO_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION

  \fn          ARM_GPIO_CAPABILITIES ARM_GPIO_GetCapabilities (void)
  \brief       Get driver capabilities.
  \return      \ref ARM_GPIO_CAPABILITIES

  \fn          int32_t ARM_GPIO_Initialize (ARM_GPIO_SignalEvent_t cb_event)
  \brief       Initialize GPIO Interface.
  \param[in]   cb_event  Pointer to \ref ARM_GPIO_SignalEvent
  \return      \ref execution_status

  \fn          int32_t ARM_GPIO_Uninitialize (void)
  \brief       De-initialize GPIO Interface.
  \return      \ref execution_status

  \fn          int32_t ARM_GPIO_PowerControl (ARM_POWER_STATE state)
  \brief       Control GPIO Interface Power.
  \param[in]   state  Power state
  \return      \ref execution_status

  \fn          int32_t ARM_GPIO_MasterTransmit (uint32_t addr, const uint8_t *data, uint32_t num, bool xfer_pending)
  \brief       Start transmitting data as GPIO Master.
  \param[in]   addr          Slave address (7-bit or 10-bit)
  \param[in]   data          Pointer to buffer with data to transmit to GPIO Slave
  \param[in]   num           Number of data bytes to transmit
  \param[in]   xfer_pending  Transfer operation is pending - Stop condition will not be generated
  \return      \ref execution_status

  \fn          int32_t ARM_GPIO_MasterReceive (uint32_t addr, uint8_t *data, uint32_t num, bool xfer_pending)
  \brief       Start receiving data as GPIO Master.
  \param[in]   addr          Slave address (7-bit or 10-bit)
  \param[out]  data          Pointer to buffer for data to receive from GPIO Slave
  \param[in]   num           Number of data bytes to receive
  \param[in]   xfer_pending  Transfer operation is pending - Stop condition will not be generated
  \return      \ref execution_status

  \fn          int32_t ARM_GPIO_SlaveTransmit (const uint8_t *data, uint32_t num)
  \brief       Start transmitting data as GPIO Slave.
  \param[in]   data  Pointer to buffer with data to transmit to GPIO Master
  \param[in]   num   Number of data bytes to transmit
  \return      \ref execution_status

  \fn          int32_t ARM_GPIO_SlaveReceive (uint8_t *data, uint32_t num)
  \brief       Start receiving data as GPIO Slave.
  \param[out]  data  Pointer to buffer for data to receive from GPIO Master
  \param[in]   num   Number of data bytes to receive
  \return      \ref execution_status

  \fn          int32_t ARM_GPIO_GetDataCount (void)
  \brief       Get transferred data count.
  \return      number of data bytes transferred; -1 when Slave is not addressed by Master

  \fn          int32_t ARM_GPIO_Control (uint32_t control, uint32_t arg)
  \brief       Control GPIO Interface.
  \param[in]   control  Operation
  \param[in]   arg      Argument of operation (optional)
  \return      \ref execution_status

  \fn          ARM_GPIO_STATUS ARM_GPIO_GetStatus (void)
  \brief       Get GPIO status.
  \return      GPIO status \ref ARM_GPIO_STATUS

  \fn          void ARM_GPIO_SignalEvent (uint32_t event)
  \brief       Signal GPIO Events.
  \param[in]   event  \ref GPIO_events notification mask
*/

typedef void (*ARM_GPIO_SignalEvent_t) (uint32_t event);  ///< Pointer to \ref ARM_GPIO_SignalEvent : Signal GPIO Event.


/**
\brief GPIO Driver Capabilities.
*/
typedef struct _ARM_GPIO_CAPABILITIES {
  uint32_t address_10_bit : 1;          ///< supports 10-bit addressing
  uint32_t reserved       : 31;         ///< Reserved (must be zero)
} ARM_GPIO_CAPABILITIES;

typedef GPIO_TypeDef* GPIO_PORT;

/**
\brief Access structure of the GPIO Driver.
*/
typedef struct _ARM_DRIVER_GPIO {
  ARM_DRIVER_VERSION    (*GetVersion)     (void);                                                                ///< Pointer to \ref ARM_GPIO_GetVersion : Get driver version.
  ARM_GPIO_CAPABILITIES (*GetCapabilities)(void);                                                                ///< Pointer to \ref ARM_GPIO_GetCapabilities : Get driver capabilities.
  int32_t               (*Initialize)     (ARM_GPIO_SignalEvent_t cb_event);                                      ///< Pointer to \ref ARM_GPIO_Initialize : Initialize GPIO Interface.
  int32_t               (*Uninitialize)   (void);                                                                ///< Pointer to \ref ARM_GPIO_Uninitialize : De-initialize GPIO Interface.
  int32_t               (*PowerControl)   (ARM_POWER_STATE state);                                               ///< Pointer to \ref ARM_GPIO_PowerControl : Control GPIO Interface Power.
  int32_t               (*On)             (GPIO_PORT Port, uint32_t Pin);                          ///< Turn on the gpio 
  int32_t               (*Off)            (GPIO_PORT Port, uint32_t Pin);                        ///< Turn off the gpio
  int32_t               (*Toggle)         (GPIO_PORT Port, uint32_t Pin);                     ///< Toggle the gpio
  int32_t               (*Control)        (uint32_t control, uint32_t arg);                                      ///< Pointer to \ref ARM_GPIO_Control : Control GPIO Interface.
  ARM_GPIO_STATUS       (*GetStatus)      (void);                                                                ///< Pointer to \ref ARM_GPIO_GetStatus : Get GPIO status.
} const ARM_DRIVER_GPIO;

#ifdef  __cplusplus
}
#endif

#endif /* DRIVER_GPIO_H_ */
