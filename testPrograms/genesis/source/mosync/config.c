
#include "osd.h"

t_config config;


void set_config_defaults(void)
{
  /* sound options */
  config.psg_preamp   = 1.5;
  config.fm_preamp    = 1.0;
  config.boost        = 1;
  config.filter       = 1;
  config.hq_fm        = 1;
  config.fm_core      = 0;

  /* system options */
  config.region_detect  = 0;
  config.force_dtack    = 0;
  config.bios_enabled   = 0;

  /* display options */
  config.aspect   = 0;
  config.overscan = 0;
  config.render   = 1;

  /* controllers options */
  config.gun_cursor   = 1;
  config.invert_mouse = 0;
  
  config.ntsc = 0;
}

