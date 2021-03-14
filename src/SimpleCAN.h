#pragma once

#include "Arduino.h"
#include "stm32f4xx_hal_can.h"


// Value needed for prescaler. depends 
// on CLK configuration
enum CanSpeed
{
	BAUD_1M = 0,
	BAUD_500K,
	BAUD_250K,
	BAUD_125K,
	BAUD_100K,
	BAUD_50K,
	BAUD_20K,
};

struct can_timing {
	uint32_t Prescaler;
	uint32_t TimeSeg1;
	uint32_t TimeSeg2;
}

// these timings assume 42MHz PCKL1 which is common for 168Mhz HCKL
const can_timings[] {
	{.Prescaler=3,.TimeSeg1=11,.TimeSeg2=2}, // BAUD_1M
	{.Prescaler=6,.TimeSeg1=11,.TimeSeg2=2}, // BAUD_500K
	{.Prescaler=12,.TimeSeg1=11,.TimeSeg2=2}, // BAUD_250K
	{.Prescaler=21,.TimeSeg1=13,.TimeSeg2=2}, // BAUD_125K
	{.Prescaler=28,.TimeSeg1=12,.TimeSeg2=2}, // BAUD_100K
	{.Prescaler=56,.TimeSeg1=12,.TimeSeg2=2}, // BAUD_50K
	{.Prescaler=140,.TimeSeg1=12,.TimeSeg2=2}, // BAUD_20K
};

enum CanMode
{
	NormalCAN = CAN_MODE_NORMAL,
	// SilentCAN = CAN_MODE_SILENT,
	LoopBackCan = CAN_MODE_LOOPBACK,
	// SilentLoopBackCAN = CAN_MODE_SILENT_LOOPBACK
};

typedef enum
{
  CAN_OK       = 0x00U,
  CAN_ERROR    = 0x01U,
  CAN_BUSY     = 0x02U,
  CAN_TIMEOUT  = 0x03U
} CAN_Status;


typedef struct{
  uint8_t dlc;
  uint32_t msgID;
  bool isRTR;
  bool isStandard;
  uint8_t data[8];
} CanMessage;

static CAN_FilterTypeDef FILTER_ACCEPT_ALL = {
    .FilterMaskIdHigh = 0,
    .FilterMaskIdLow = 0,
    .FilterFIFOAssignment = CAN_FILTER_FIFO0,
    .FilterMode = CAN_FILTERMODE_IDMASK,
    .FilterScale= CAN_FILTERSCALE_32BIT,
    .FilterActivation = ENABLE,
};


CanMessage createMessage(void);

class SimpleCAN
{
public:

	SimpleCAN();
	CAN_Status init(int rx_pin, int tx_pin, CanSpeed speed = BAUD_500K, CanMode mode = NormalCAN);
	CAN_Status filter(CAN_FilterTypeDef *filter);
	CAN_Status filterAcceptAll();
	CAN_Status subscribe(void (*_receive) (CanMessage * message) = nullptr);
	CAN_Status unsubscribe();
	CAN_Status begin();
	CAN_Status stop();
	CAN_Status transmit(CanMessage* txMessage);
	CAN_Status receive(CanMessage* rxMessage);
	static void _receive(CanMessage* rxMessage);
	static void(*receiveCallback)(CanMessage* rxMessage);
	static CAN_HandleTypeDef* _hcan;
private: 
	void createCanHandle(CanSpeed speed, CanMode mode);
};
