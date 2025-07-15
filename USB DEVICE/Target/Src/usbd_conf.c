/**
  ******************************************************************************
  * @file    usbd_conf_template.c
  * @author  MCD Application Team
  * @brief   USB Device configuration and interface file
  *          This template should be copied to the user folder,
  *          renamed and customized following user needs.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"
#include "usbd_msc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_DRD_FS;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
                       LL Driver Callbacks (PCD -> USB Device Library)
*******************************************************************************/

/**
  * @brief  SetupStage callback.
  * @param  hpcd: PCD handle
  * @retval None
  */

void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd)
{
	USBD_LL_SetupStage((USBD_HandleTypeDef*)hpcd->pData, (uint8_t *)hpcd->Setup);
}

/**
  * @brief  DataOut Stage callback.
  * @param  hpcd: PCD handle
  * @param  epnum: Endpoint Number
  * @retval None
  */

void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
	USBD_LL_DataOutStage((USBD_HandleTypeDef*)hpcd->pData, epnum, hpcd->OUT_ep[epnum].xfer_buff);
}

/**
  * @brief  DataIn Stage callback.
  * @param  hpcd: PCD handle
  * @param  epnum: Endpoint Number
  * @retval None
  */
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
	USBD_LL_DataInStage((USBD_HandleTypeDef*)hpcd->pData, epnum, hpcd->IN_ep[epnum].xfer_buff);
}

/**
  * @brief  SOF callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd)
{
	USBD_LL_SOF((USBD_HandleTypeDef*)hpcd->pData);
}

/**
  * @brief  Reset callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd)
{
	USBD_SpeedTypeDef speed = USBD_SPEED_FULL;
	/* Set Speed. */
	USBD_LL_SetSpeed((USBD_HandleTypeDef*)hpcd->pData, speed);
	/* Reset Device. */
	USBD_LL_Reset((USBD_HandleTypeDef*)hpcd->pData);
}

/**
  * @brief  ConnectCallback callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)
{
	USBD_LL_DevConnected((USBD_HandleTypeDef*)hpcd->pData);
}

/**
  * @brief  Disconnect callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)
{
	USBD_LL_DevDisconnected((USBD_HandleTypeDef*)hpcd->pData);
}

/*******************************************************************************
                       LL Driver Interface (USB Device Library --> PCD)
*******************************************************************************/

/**
  * @brief  Initializes the Low Level portion of the Device driver.
  * @param  pdev: Device handle
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef *pdev)
{
	hpcd_USB_DRD_FS.Instance = USB_DRD_FS;
	hpcd_USB_DRD_FS.Init.dev_endpoints = 8;
	hpcd_USB_DRD_FS.Init.speed = PCD_SPEED_FULL;
	hpcd_USB_DRD_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
	hpcd_USB_DRD_FS.Init.Sof_enable = DISABLE;
	hpcd_USB_DRD_FS.Init.low_power_enable = DISABLE;
	hpcd_USB_DRD_FS.Init.lpm_enable = DISABLE;
	hpcd_USB_DRD_FS.Init.battery_charging_enable = DISABLE;
	hpcd_USB_DRD_FS.Init.vbus_sensing_enable = DISABLE;
	hpcd_USB_DRD_FS.Init.bulk_doublebuffer_enable = DISABLE;
	hpcd_USB_DRD_FS.Init.iso_singlebuffer_enable = DISABLE;
	HAL_PCD_Init(&hpcd_USB_DRD_FS) ;
	/* Link the driver to the stack */
	pdev->pData  = &hpcd_USB_DRD_FS;
	hpcd_USB_DRD_FS.pData = pdev;


	/* Control Endpoints */
	HAL_PCDEx_PMAConfig((PCD_HandleTypeDef*)pdev->pData , 0x00 , PCD_SNG_BUF, 0x20);
	HAL_PCDEx_PMAConfig((PCD_HandleTypeDef*)pdev->pData , 0x80 , PCD_SNG_BUF, 0x40);

	/* HID Endpoints */
	HAL_PCDEx_PMAConfig((PCD_HandleTypeDef*)pdev->pData , HID_EPIN_ADDR , PCD_SNG_BUF, 0x50);

	/* MSC Endpoints */
	HAL_PCDEx_PMAConfig((PCD_HandleTypeDef*)pdev->pData , MSC_EPIN_ADDR , PCD_SNG_BUF, 0x80);
    HAL_PCDEx_PMAConfig((PCD_HandleTypeDef*)pdev->pData , MSC_EPOUT_ADDR , PCD_SNG_BUF, 0x80);
	return USBD_OK;
}

/**
  * @brief  De-Initializes the Low Level portion of the Device driver.
  * @param  pdev: Device handle
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_DeInit(USBD_HandleTypeDef *pdev)
{
	HAL_PCD_DeInit(pdev->pData);
	return USBD_OK;
}

/**
  * @brief  Starts the Low Level portion of the Device driver.
  * @param  pdev: Device handle
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef *pdev)
{
	HAL_PCD_Start(pdev->pData);
	return USBD_OK;
}

/**
  * @brief  Stops the Low Level portion of the Device driver.
  * @param  pdev: Device handle
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef *pdev)
{
	HAL_PCD_Stop(pdev->pData);
	return USBD_OK;
}

/**
  * @brief  Opens an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @param  ep_type: Endpoint Type
  * @param  ep_mps: Endpoint Max Packet Size
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_OpenEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr,
                                  uint8_t ep_type, uint16_t ep_mps)
{
	HAL_PCD_EP_Open(pdev->pData, ep_addr, ep_mps, ep_type);
	return USBD_OK;
}

/**
  * @brief  Closes an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
   HAL_PCD_EP_Close(pdev->pData, ep_addr);
   return USBD_OK;
}

/**
  * @brief  Flushes an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
	HAL_PCD_EP_Flush(pdev->pData, ep_addr);
	return USBD_OK;
}

/**
  * @brief  Sets a Stall condition on an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
   HAL_PCD_EP_SetStall(pdev->pData, ep_addr);
   return USBD_OK;
}

/**
  * @brief  Clears a Stall condition on an endpoint of the Low Level Driver.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef *pdev,
                                        uint8_t ep_addr)
{
	HAL_PCD_EP_ClrStall(pdev->pData, ep_addr);
	return USBD_OK;
}

/**
  * @brief  Returns Stall condition.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval Stall (1: Yes, 0: No)
  */
uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
	PCD_HandleTypeDef *hpcd = pdev->pData;

	if((ep_addr & 0x80) == 0x80)
	  {
	    return hpcd->IN_ep[ep_addr & 0x7F].is_stall;
	  }
	else
	  {
	    return hpcd->OUT_ep[ep_addr & 0x7F].is_stall;
	  }
}

/**
  * @brief  Assigns a USB address to the device.
  * @param  pdev: Device handle
  * @param  dev_addr: Endpoint Number
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef *pdev,
                                         uint8_t dev_addr)
{
  HAL_PCD_SetAddress(pdev->pData, dev_addr);
  return USBD_OK;
}

/**
  * @brief  Transmits data over an endpoint.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @param  pbuf: Pointer to data to be sent
  * @param  size: Data size
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef *pdev, uint8_t ep_addr,
                                    uint8_t *pbuf, uint32_t size)
{
	HAL_PCD_EP_Transmit(pdev->pData, ep_addr, pbuf, size);
	return USBD_OK;
}

/**
  * @brief  Prepares an endpoint for reception.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @param  pbuf: Pointer to data to be received
  * @param  size: Data size
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef *pdev,
                                          uint8_t ep_addr, uint8_t *pbuf,
                                          uint32_t size)
{
	HAL_PCD_EP_Receive(pdev->pData, ep_addr, pbuf, size);
	return USBD_OK;
}

/**
  * @brief  Returns the last transferred packet size.
  * @param  pdev: Device handle
  * @param  ep_addr: Endpoint Number
  * @retval Received Data Size
  */
uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
	return HAL_PCD_EP_GetRxCount(pdev->pData, ep_addr);
}

#ifdef USBD_HS_TESTMODE_ENABLE
/**
  * @brief  Set High speed Test mode.
  * @param  pdev: Device handle
  * @param  testmode: test mode
  * @retval USBD Status
  */
USBD_StatusTypeDef USBD_LL_SetTestMode(USBD_HandleTypeDef *pdev, uint8_t testmode)
{
  UNUSED(pdev);
  UNUSED(testmode);

  return USBD_OK;
}
#endif /* USBD_HS_TESTMODE_ENABLE */

/**
  * @brief  Static single allocation.
  * @param  size: Size of allocated memory
  * @retval None
  */
void *USBD_static_malloc(uint32_t size)
{
  UNUSED(size);
  static uint32_t mem[(sizeof(USBD_MSC_BOT_HandleTypeDef) / 4) + 1]; /* On 32-bit boundary */
  return mem ;
}

/**
  * @brief  Dummy memory free
  * @param  p: Pointer to allocated  memory address
  * @retval None
  */
void USBD_static_free(void *p)
{
  UNUSED(p);
}

/**
  * @brief  Delays routine for the USB Device Library.
  * @param  Delay: Delay in ms
  * @retval None
  */
void USBD_LL_Delay(uint32_t Delay)
{
	HAL_Delay(Delay);
}
