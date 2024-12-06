/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <common.h>
#include <device/map.h>
#include <SDL2/SDL.h>

enum {
  reg_freq,
  reg_channels,
  reg_samples,
  reg_sbuf_size,
  reg_init,
  reg_count,
  nr_reg
};

static uint8_t *sbuf = NULL;
static uint32_t *audio_base = NULL;

static uint32_t sbuf_idex = 0;

void sdl_audio_callback(void *userdata, Uint8 *stream, int len) {
  SDL_memset(stream, 0, len);
  uint32_t used_cnt = audio_base[reg_count];
  len = len > used_cnt ? used_cnt : len;

  uint32_t sbuf_size = audio_base[reg_sbuf_size];

  if ((sbuf_idex + len) > sbuf_size) {
    SDL_MixAudio(stream, sbuf + sbuf_idex, sbuf_size - sbuf_idex, SDL_MIX_MAXVOLUME);
    SDL_MixAudio(stream + (sbuf_size - sbuf_idex), sbuf + (sbuf_size - sbuf_idex), len - (sbuf_size - sbuf_idex), SDL_MIX_MAXVOLUME);
  }
  else {
    SDL_MixAudio(stream, sbuf + sbuf_idex, len, SDL_MIX_MAXVOLUME);
  }

  sbuf_idex = (sbuf_idex + len) % sbuf_size;
  audio_base[reg_count] -= len;


}

static void init_sound() {
  SDL_AudioSpec s = {};
  s.format = AUDIO_S16SYS;  
  s.userdata = NULL;        
  s.freq = audio_base[reg_freq];
  s.channels = audio_base[reg_channels];
  s.samples = audio_base[reg_samples];
  s.callback = sdl_audio_callback;
  SDL_InitSubSystem(SDL_INIT_AUDIO);
  SDL_OpenAudio(&s, NULL);
  SDL_PauseAudio(0);
}

static void audio_io_handler(uint32_t offset, int len, bool is_write) {
  if (audio_base[reg_init] == 1) {
    init_sound();
    audio_base[reg_init] = 0;
  }
}

void init_audio() {
  uint32_t space_size = sizeof(uint32_t) * nr_reg;
  audio_base = (uint32_t *)new_space(space_size);
  audio_base[reg_sbuf_size] = CONFIG_SB_SIZE;
#ifdef CONFIG_HAS_PORT_IO
  add_pio_map ("audio", CONFIG_AUDIO_CTL_PORT, audio_base, space_size, audio_io_handler);
#else
  add_mmio_map("audio", CONFIG_AUDIO_CTL_MMIO, audio_base, space_size, audio_io_handler);
#endif

  sbuf = (uint8_t *)new_space(CONFIG_SB_SIZE);
  add_mmio_map("audio-sbuf", CONFIG_SB_ADDR, sbuf, CONFIG_SB_SIZE, NULL);
}
