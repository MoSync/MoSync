/*
#include <windows.h>
#include <SDL.h>
*/
#include <maapi.h>

#include "shared.h"

#define SOUND_FREQUENCY    11025
int timer_count = 0;
int old_timer_count = 0;
int paused = 0;
int frame_count = 0;
int frameticker = 0;
int joynum = 0;
int makb = 0;

int update_input(void);
byte *keystate;
byte buf[0x24000];

byte soundbuffer[16][3840];
int mixbuffer   = 0;
int playbuffer  = 0;

byte log_error   = 0;
byte debug_on    = 0;
byte turbo_mode  = 0;
byte use_sound   = 1;

int audio_len;

#include "md_ntsc.h"
#include "sms_ntsc.h"

/*** NTSC Filters ***/
md_ntsc_setup_t md_setup;
md_ntsc_t md_ntsc;
sms_ntsc_setup_t sms_setup;
sms_ntsc_t sms_ntsc;

void assert(int a)
{
}

void my_malloc_handler(int size) {

	//char temp[2048];
	//sprintf("malloc(%d) failed\n", temp);	
	//maWriteLog(temp, strlen(temp));
	maReportCallStack();
	maPanic(0, "malloc failed");
}

unsigned int fps_callback(unsigned int interval)
{
	if(paused) return 1000/vdp_rate;
	timer_count++;
  frameticker ++;
  
 	if(timer_count % vdp_rate == 0)
	{ 
// 		int fps = frame_count + 1; 
		frame_count = 0;
	}	
  /*
	if(timer_count % vdp_rate == 0)
	{
		int fps = frame_count + 1;
		char caption[100];
    char region[10];
    if (region_code == REGION_USA) sprintf(region,"USA");
    else if (region_code == REGION_EUROPE) sprintf(region,"EUR");
    else sprintf(region,"JAP");
    sprintf(caption, "Genesis Plus/SDL - %s (%s) - %d fps - %d bytes", rominfo.international, region, fps, audio_len);
		SDL_WM_SetCaption(caption, NULL);
		frame_count = 0;
    
	}
	*/
	
	return 1000/vdp_rate;
}

/*
static void sdl_sound_callback(void *userdata, Uint8 *stream, int len)
{
  audio_len = len;
  memcpy(stream, soundbuffer[playbuffer], len);

  // increment soundbuffers index
  playbuffer++;
  playbuffer &= 0xf;
  if (playbuffer == mixbuffer)
  {
    playbuffer--;
    if ( playbuffer < 0 ) playbuffer = 15;
  }
}
*/

/*
static int sdl_sound_init()
{
  SDL_AudioSpec audio;
  
  if(SDL_Init(SDL_INIT_AUDIO) < 0)
  {
    char caption[256];
		sprintf(caption, "SDL audio can't initialize");
		MessageBox(NULL, caption, "Error", 0);
    return 0;
  }

  audio.freq      = SOUND_FREQUENCY;
  audio.format    = AUDIO_S16LSB;
  audio.channels  = 2;
  audio.samples   = snd.buffer_size;
  audio.callback  = sdl_sound_callback;

  if(SDL_OpenAudio(&audio, NULL) == -1)
  {
    char caption[256];
		sprintf(caption, "SDL can't open audio");
		MessageBox(NULL, caption, "Error", 0);
    return 0;
  }

	memset(soundbuffer, 0, 16 * 3840);
  mixbuffer = 0;
  playbuffer = 0;
  return 1;
}
*/

//int main (int argc, char **argv)
int MAMain()
{
	int running = 1;
//  int sym;

  /*
  SDL_Rect viewport, display;
  SDL_Surface *bmp, *screen;
  SDL_Event event;
*/

  error_init();

  /* Print help if no game specified */
  /*
	if(argc < 2)
	{
		char caption[256];
    sprintf(caption, "Genesis Plus\\SDL by Charles MacDonald\nWWW: http://cgfm2.emuviews.com\nusage: %s gamename\n", argv[0]);
    MessageBox(NULL, caption, "Information", 0);
		exit(1);
	}
*/

	set_malloc_handler(my_malloc_handler);

	/* Load game */
	cart_rom = malloc(CART_ROM_SIZE);
	//memset(cart_rom, 0, 0xA00000);
 if(!load_rom("sonic.bin"))
	{
		//char caption[256];
		//sprintf(caption, "Error loading file `%s'.", argv[1]);
		//MessageBox(NULL, caption, "Error", 0);
		//exit(1);
		maPanic(1, "Error loading file");
	}
        
	/* load BIOS */
	/*
  memset(bios_rom, 0, sizeof(bios_rom));
	FILE *f = fopen("./BIOS.bin", "rb");
	if (f!=NULL)
	{
		fread(&bios_rom, 0x800,1,f);
		fclose(f);
		int i;
		for(i = 0; i < 0x800; i += 2)
		{
			uint8 temp = bios_rom[i];
			bios_rom[i] = bios_rom[i+1];
			bios_rom[i+1] = temp;
		}
		config.bios_enabled |= 2;
  }
	else 
	*/
	config.bios_enabled = 0;

  /* initialize SDL */
  /*
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
  {
		char caption[256];
		sprintf(caption, "SDL initialization failed");
		MessageBox(NULL, caption, "Error", 0);
    exit(1);
  }

  SDL_WM_SetCaption("Genesis Plus/SDL", NULL);
  SDL_ShowCursor(0);
*/
  /* initialize SDL video */
  /*
	viewport.x = 0;
	viewport.y = 0;
  viewport.w = 640;
  viewport.h = 480;

  display.x = 0;
  display.y = 0;
	display.w = 256;
	display.h = 224;

  bmp = SDL_CreateRGBSurface(SDL_HWSURFACE, 360, 576, 16, 0xF800, 0x07E0, 0x001F, 0x0000);
  screen = SDL_SetVideoMode(viewport.w, viewport.h, 16,  SDL_HWSURFACE);
  if (!bmp || !screen)
  {
		char caption[256];
		sprintf(caption, "Video initialization failed");
		MessageBox(NULL, caption, "Error", 0);
		exit(1);
  }
  */

  MAFrameBufferInfo frameBufferInfo;
  maFrameBufferGetInfo(&frameBufferInfo);
  
  printf("size of framebuffer: %d", frameBufferInfo.sizeInBytes);
  byte *frameBuffer = (byte*)malloc(frameBufferInfo.sizeInBytes);
  maFrameBufferInit(frameBuffer);
  
  /* initialize Genesis display */
  memset(&bitmap, 0, sizeof(t_bitmap));
  bitmap.width  = frameBufferInfo.width;
  bitmap.height = frameBufferInfo.height;
  bitmap.depth  = frameBufferInfo.bitsPerPixel; //16
  bitmap.granularity = frameBufferInfo.bytesPerPixel; //2;
  bitmap.pitch = frameBufferInfo.pitch; //(bitmap.width * bitmap.granularity);
  bitmap.data   = (unsigned char *)frameBuffer;//bmp->pixels;
  bitmap.viewport.w = 256;
  bitmap.viewport.h = 224;
  bitmap.viewport.x = 0;
  bitmap.viewport.y = 0;
  bitmap.remap = 1;

  /* set default config */
  set_config_defaults();
  input.system[0] = SYSTEM_GAMEPAD;
//  input.system[1] = SYSTEM_MOUSE;

	/* initialize emulation */
  system_init();
  //audio_init(SOUND_FREQUENCY);
  sound_init(SOUND_FREQUENCY);
	
  /* initialize SDL audio */
//  if (use_sound) use_sound = sdl_sound_init();

  /* load SRAM */
/*
  f = fopen("./game.srm", "rb");
  if (f!=NULL)
  {
    fread(&sram.sram,0x10000,1, f);
	  fclose(f);
  }
*/

  /* reset emulation */
  system_reset();

  /* start emulation loop */
//  SDL_SetTimer(1000/vdp_rate, fps_callback);
//  if (use_sound) SDL_PauseAudio(0);
MAEvent e;

	int update = 0;

	while(running)
	{
		while (maGetEvent(&e)) 
		{
			switch(e.type) 
			{
				case EVENT_TYPE_CLOSE: /* Windows was closed */
					running = 0;
					break;
				case EVENT_TYPE_KEY_PRESSED:
				case EVENT_TYPE_KEY_RELEASED:
					makb = maGetKeys();
					break;
			}
		}
		
    if(!paused)
    {
/*
		if (frameticker > 1)
		{
			// Frame skipping 
			frameticker--;
			system_frame (1);
		}
		else
		{	
			// Delay 
			//while (!frameticker && !turbo_mode) SDL_Delay(1);
			system_frame (0);
			frame_count++;
		}
		frameticker--;
*/
	
//	if((update&0x3) == 0) {
		system_frame (0);
		maUpdateScreen();
		maResetBacklight();
//	} else {
//		system_frame (1);
//	}
				
	update++;
	  /*
      if(bitmap.viewport.changed)
      {
        bitmap.viewport.changed = 0;
        display.w = (bitmap.viewport.w + 2*bitmap.viewport.x);
        display.h = (bitmap.viewport.h + 2 * bitmap.viewport.y) << ((config.render && interlaced) ? 1:0);
        viewport.w = bitmap.viewport.w + 2*bitmap.viewport.x;
        viewport.h = bitmap.viewport.h + 2*bitmap.viewport.y;
        viewport.x = (640 - viewport.w)/2;
        viewport.y = (480 - viewport.h)/2;
      }

      SDL_BlitSurface(bmp, &display, screen, &viewport);
      SDL_UpdateRect(screen, viewport.x, viewport.y, viewport.w, viewport.h);
	  */
    }
  }

  /* save SRAM */
  /*
  f = fopen("./game.srm", "wb");
  if (f!=NULL)
  {
    fwrite(&sram.sram,0x10000,1, f);
	  fclose(f);
  }
*/

 // SDL_PauseAudio(1);
 // SDL_CloseAudio();
 // SDL_FreeSurface(bmp);
 // SDL_FreeSurface(screen);
 // SDL_Quit();
  system_shutdown();
  error_shutdown();
  free(cart_rom);

  return 0;
}

int lastmakb = -1;

int update_input(void)
{
	if(lastmakb == makb) return 1;
	lastmakb = makb;
/*
  keystate = SDL_GetKeyState(NULL);

  while (input.dev[joynum] == NO_DEVICE)
{
    joynum ++;
    if (joynum > MAX_DEVICES - 1) joynum = 0;
  }

 // reset input
  input.pad[joynum] = 0;

  // keyboard 
  if(keystate[SDLK_UP])     input.pad[joynum] |= INPUT_UP;
  else
  if(keystate[SDLK_DOWN])   input.pad[joynum] |= INPUT_DOWN;
  if(keystate[SDLK_LEFT])   input.pad[joynum] |= INPUT_LEFT;
  else
  if(keystate[SDLK_RIGHT])  input.pad[joynum] |= INPUT_RIGHT;

  if(keystate[SDLK_a])      input.pad[joynum] |= INPUT_A;
  if(keystate[SDLK_s])      input.pad[joynum] |= INPUT_B;
  if(keystate[SDLK_d])      input.pad[joynum] |= INPUT_C;
  if(keystate[SDLK_f])      input.pad[joynum] |= INPUT_START;
  if(keystate[SDLK_z])      input.pad[joynum] |= INPUT_X;
  if(keystate[SDLK_x])      input.pad[joynum] |= INPUT_Y;
  if(keystate[SDLK_c])      input.pad[joynum] |= INPUT_Z;
  if(keystate[SDLK_v])      input.pad[joynum] |= INPUT_MODE;
*/
	  while (input.dev[joynum] == NO_DEVICE)
{
	    joynum ++;
	    if (joynum > MAX_DEVICES - 1) joynum = 0;
	  }

	 // reset input
	  input.pad[joynum] = 0;

	if(makb&MAKB_UP) input.pad[joynum] = INPUT_UP;
	if(makb&MAKB_LEFT) input.pad[joynum] = INPUT_LEFT;
	if(makb&MAKB_DOWN) input.pad[joynum] = INPUT_DOWN;
	if(makb&MAKB_RIGHT) input.pad[joynum] = INPUT_RIGHT;
	if(makb&MAKB_1) input.pad[joynum] = INPUT_A;
	if(makb&MAKB_2) input.pad[joynum] = INPUT_B;
	if(makb&MAKB_3) input.pad[joynum] = INPUT_C;
	if(makb&MAKB_4) input.pad[joynum] = INPUT_X;
	if(makb&MAKB_5) input.pad[joynum] = INPUT_Y;
	if(makb&MAKB_6) input.pad[joynum] = INPUT_Z;
	if(makb&MAKB_SOFTLEFT) input.pad[joynum] = INPUT_START;
	if(makb&MAKB_SOFTRIGHT) input.pad[joynum] = INPUT_MODE;
				
  /*
  extern uint8 pico_current;
  if (input.dev[joynum] == DEVICE_LIGHTGUN)
  {
    // get mouse (absolute values) 
    int x,y;
    int state = SDL_GetMouseState(&x,&y);

    // Calculate X Y axis values 
    input.analog[joynum - 4][0] = (x * bitmap.viewport.w) / 640;
    input.analog[joynum - 4][1] = (y * bitmap.viewport.h) / 480;

    // Map mouse buttons to player #1 inputs 
    if(state & SDL_BUTTON_MMASK) input.pad[joynum] |= INPUT_C;
    if(state & SDL_BUTTON_RMASK) input.pad[joynum] |= INPUT_B;
    if(state & SDL_BUTTON_LMASK) input.pad[joynum] |= INPUT_A;
  }
  else if (input.dev[joynum] == DEVICE_MOUSE)
{
    // get mouse (relative values) 
    int x,y;
    int state = SDL_GetRelativeMouseState(&x,&y);

    // Sega Mouse range is -256;+256 
    input.analog[2][0] = x;
    input.analog[2][1] = y;

   // Vertical movement is upsidedown    
	if (!config.invert_mouse) input.analog[2][1] = 0 - input.analog[2][1];

    // Map mouse buttons to player #1 inputs 
    if(state & SDL_BUTTON_MMASK) input.pad[joynum] |= INPUT_C;
    if(state & SDL_BUTTON_RMASK) input.pad[joynum] |= INPUT_B;
    if(state & SDL_BUTTON_LMASK) input.pad[joynum] |= INPUT_A;
  }
  else if (system_hw == SYSTEM_PICO)
  {
  // get mouse (absolute values) 
    int x,y;
    int state = SDL_GetMouseState(&x,&y);

    // Calculate X Y axis values 
    input.analog[0][0] = 0x3c  + (x * (0x17c-0x03c+1)) / 640;
    input.analog[0][1] = 0x1fc + (y * (0x2f7-0x1fc+1)) / 480;
 
    // Map mouse buttons to player #1 inputs 
    if(state & SDL_BUTTON_MMASK) pico_current++;
    if(state & SDL_BUTTON_RMASK) input.pad[joynum] |= INPUT_B;
    if(state & SDL_BUTTON_LMASK) input.pad[joynum] |= INPUT_A;
  }
  */


  /* options */
  return (1);
}
