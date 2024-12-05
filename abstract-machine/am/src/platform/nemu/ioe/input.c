#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  int k = inl(KBD_ADDR);

  kbd->keydown = (k & KEYDOWN_MASK ? true : false);
  if (kbd->keydown == true)
    kbd->keycode = k & ~KEYDOWN_MASK;
  else
    kbd->keycode = AM_KEY_NONE;
}
