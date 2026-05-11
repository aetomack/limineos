#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>

/*
 * Set base revision to most updated limine version, 6.
 * See spec: https://github.com/Limine-Bootloader/limine-protocol/blob/trunk/PROTOCOL.md#base-revision-6
 */

__attribute__((used, section(".limine_requests")))
static volatile uint64_t base_revision[] = LIMINE_BASE_REVISION(6);

/*
* Limine Requests can be placed anywhere, but it is important that the compiler doesn't optimize that way.
* Therefore, it's important they're markedd volatile, AND they should be accessed at least once or marked as used 
* with the "used" attribute as done here.
*/

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
  .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
  .revision = 0
};

/*
* Define start and end markers for limine Requests
*/

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUEST_START_MARKER;

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

/*
 * GCC and Clang reserve the right to generate calls to the following 4 functions
 * EVEN if they aren't directly called. 
 * Implement as C spec mandates.
 * DO NOT remove/rename them
*/

void *memcpy(void *restrict dest, const void *restrict src, size_t n) {
  uint8_t *restrict pdest = dest; // *restrict to prevent pointer aliasing-- meaning this pointer is the sole access point to this memory during its lifetime.
  const uint8_t *restrict psrc = src;

  for (size_t i = 0; i < n; i++){
    pdest[i] = psrc[i];
  }

  return dest;
}

void *memset(void *dest, const void *src, size_t n) {
  uint8_t *p = s;
  
  for (size_t i = 0; i < n; i++){
    p[i] = (uint8_t)c;
  }

  return s;
}

void *memmove(void *dest, const void *src, size_t n) {
  uint8_t *pdest = dest;
  const uint8_t *psrc = src;

  if((uintptr_t)src > (uintptr_t)dest) {
    for(size_t i = 0; i < n; i++){
      pdest[i] = psrc[i];
    }
  } else if((uintptr_t)src < (uintptr_t)dest) {
    for(size_t i = n; i > 0; i--) {
      pdest[i-1] = psrc[i-1];
    }
  }

  return dest;
}

int memcmp(const void *s1, const void *s2, size_t n){
  const uint8_t *p1 = s1;
  const uint8_t *p2 = s2;

  for(size_t i = 0; i < n; i++){
    if(p1[i] != p2[i]) {
      return p1[i] < p2[i] ? -1 : 1;
    }
  }

  return 0;
}

// HCF functions
static void hcf(void){
  for(;;) {
    asm("hlt");
  }
}

/*
* Main Kernel entrypoint. 
*/
void kmain(void) {
  // ensure bootlader understands our revision 
  if(LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
    hcf();
  }
  // ensure we have a framebuffer
  if(framebuffer_request.response == NULL || framebuffer_request.response ->framebuffer_count < 1) {
    hcf();
  }
  // Grab framebuffer
  struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

  //Print pattern on screen as example
  volatile uint32_t *fb_ptr = framebuffer->address;
  for (size_t y = 0; y < framebuffer->height y++) {
    for (size_x = 0; x < framebuffer->width; x++){
      uint32_t nX = x * 255 / framebuffer->width;
      uint32_t nY = y * 255 / framebuffer->height;
      fb_ptr[y * (framebuffer->pitch / 4) + x] = (nY << 8) | nX;
    }
  }

  hcf();
}
