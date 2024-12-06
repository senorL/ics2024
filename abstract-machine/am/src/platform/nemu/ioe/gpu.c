#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)
#define N 32

void __am_gpu_init() {
  // int i;
  // int w = 400;
  // int h = 300;
  // uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  // for (i = 0; i < w * h; i ++) fb[i] = i;
  // outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  uint32_t screan_wh = inl(VGACTL_ADDR); 
  uint32_t h = screan_wh & 0xffff;
  uint32_t w = (screan_wh >> 16) & 0xffff;

  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = w, .height = h,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  if(!ctl->sync &&(ctl->w == 0 || ctl->h == 0)) return;

  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  uint32_t *pixels = (uint32_t *)ctl->pixels;
  uint32_t sw = inl(VGACTL_ADDR) >> 16;

  for (int j = ctl->y; j < ctl->y + ctl->h; j++) {
    for (int i = ctl->x; i < ctl->x + ctl->w; i++) {
      fb[j * sw + i] = pixels[(j - ctl->y) * ctl->w + (i - ctl->x)];
    }
  }
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
