#include <stdint.h>
#include "sl_component_catalog.h"
#include "rail.h"
#include "sl_rail_util_init.h"
#include "app_process.h"
#include "sl_simple_led_instances.h"
#include "app_log.h"
#include "sl_rail_util_init_inst0_config.h"
#include "sl_rail_util_protocol_types.h"
#include "rail_config.h"
#if defined(RAIL0_CHANNEL_GROUP_1_PROFILE_WISUN_OFDM)
#include "sl_rail_util_pa_config.h"
#include "rail_chip_specific.h"
#endif

#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "app_task_init.h"
#endif


static void validation_check(void);

/******************************************************************************
 * The function is used for some basic initialization related to the app.
 *****************************************************************************/
RAIL_Handle_t app_init(void)
{
  validation_check();

  // Get RAIL handle, used later by the application
  RAIL_Handle_t rail_handle = sl_rail_util_get_handle(SL_RAIL_UTIL_HANDLE_INST0);

  RAIL_SetTxPowerDbm(rail_handle, RAIL_TX_POWER_MAX);
  set_up_tx_fifo(rail_handle);

  // Turn OFF LEDs
  sl_led_turn_off(&sl_led_led0);
#if defined(SL_CATALOG_LED1_PRESENT)
  sl_led_turn_off(&sl_led_led1);
#endif
  // Start reception
  RAIL_Status_t status = RAIL_StartRx(rail_handle, CHANNEL, NULL);
  if (status != RAIL_STATUS_NO_ERROR) {
    app_log_warning("After initialization RAIL_StartRx() result:%d ", status);
  }

#if !BOARD_IS_SENSOR
  app_log_info("BASE STATION\n");
#else
  app_log_info("SENSOR\n");
#endif
  return rail_handle;
}

// -----------------------------------------------------------------------------
//                          Static Function Definitions
// -----------------------------------------------------------------------------
/*****************************************************************************
* Checks phy setting to avoid errors at packet sending
*****************************************************************************/
static void validation_check(void)
{
  _Static_assert(SL_RAIL_UTIL_INIT_PROTOCOL_INST0_DEFAULT == SL_RAIL_UTIL_PROTOCOL_PROPRIETARY,
                 "Please use the Flex (RAIL) - Simple TRX Standards sample app instead, which is designed to show the protocol usage.");
#if defined(RAIL0_CHANNEL_GROUP_1_PROFILE_WISUN_OFDM) && !defined(HARDWARE_BOARD_HAS_EFF)
  _Static_assert(SL_RAIL_UTIL_PA_SELECTION_SUBGHZ == RAIL_TX_POWER_MODE_OFDM_PA,
                 "Please use the OFDM PA settings in the sl_rail_util_pa_config.h for OFDM phys.");
#endif
#if defined(RAIL0_CHANNEL_GROUP_1_PROFILE_WISUN_OFDM) && RAIL_SUPPORTS_EFF && defined(HARDWARE_BOARD_HAS_EFF)
  _Static_assert(SL_RAIL_UTIL_PA_SELECTION_SUBGHZ >= RAIL_TX_POWER_MODE_OFDM_PA_EFF_30DBM,
                 "Please use the OFDM PA for EFF settings in the sl_rail_util_pa_config.h for OFDM phys.");
#endif
}
