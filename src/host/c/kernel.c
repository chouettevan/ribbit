#include "kernel.h"
#include "multiboot.h"
#include <stdarg.h>
#define DEFAULT_COLOR 0xf0
#define IS_VALID_MEMORY_MAP (1 << 6)
#define ASSEMBLE_64BIT(low,high) (((multiboot_uint64_t)high) + (((multiboot_uint64_t)low) << 32))
void vga_write(char c, uint8_t color, int index) {
  uint16_t *vga = (uint16_t *)0xb8000;
  vga[index] = c | color << 8;
}
int errno = 0;
int position = 0;
char kernel_heap[1024 * 1024 * 32];
static char scancode_to_ascii[128] =
    "&&1234567890-=\b\tqwertyuiop[]\n&asdfghjkl;'`\xff\\zxcvbnm,./\xff*& ";
static char scancode_to_ascii_shift[128] =
    "&&!@#$%^&*()_+\b\tQWERTYUIOP{}\n&ASDFGHJKL:\"~\xff|ZXCVBNM<>?\xff*& ";
struct heap_chunk *heap = (struct heap_chunk *)kernel_heap;

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
  outw(0x3D4, 0x0A);
  outw(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
  outw(0x3D4, 0x0B);
  outw(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void update_cursor(uint16_t pos) {

  outw(0x3D4, 0x0F);
  outw(0x3D5, (uint8_t)(pos & 0xFF));
  outw(0x3D4, 0x0E);
  outw(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void kernel_main(multiboot_info_t *info, unsigned magic,void* end) {
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    printf("wrong magic %x\n",magic);
    exit(-1);
  }
  if (!(info->flags & IS_VALID_MEMORY_MAP)) {
    printf("invalid memory map.\n");
    exit(-1);
  }
  printf("%d KB of memory available\n",info->mem_lower + info->mem_upper);
  printf("elf end at 0x%lx \n",end);
  for (int i=0;i < info->mmap_length;i += sizeof(multiboot_memory_map_t)) {
    multiboot_memory_map_t *map = (void*)(info->mmap_addr + i);
    if (map->type == MULTIBOOT_MEMORY_AVAILABLE) {
      multiboot_uint64_t len = ASSEMBLE_64BIT(map->len_low,map->len_high);
      multiboot_uint64_t addr = ASSEMBLE_64BIT(map->addr_low,map->addr_high);
      printf("%llu bytes of memory at %llu \n",len,addr);
      printf("LOW:%u bytes of memory at %u \n",map->len_low,map->addr_low);
      printf("HIGH:%x bytes of memory at %u \n",map->len_high,map->addr_high);
      printf("%llx\n",ASSEMBLE_64BIT(0x1,0x1));
//      multiboot_uint64_t *end = (void*)(addr + len - 8);
  //    printf("bytes 0x%llx at the end, which is address 0x%llx\n",*end,(multiboot_uint64_t)end);
    }
  }
  heap->next = NULL;
  heap->size = sizeof(kernel_heap);
  init();
}

void *malloc(size_t size) {
  struct heap_chunk *local = heap;
  struct heap_chunk *prev = heap;
  while (local != NULL) {
    if (local->size > size) {
      if (local->size - size > 0x20) {
        struct heap_chunk *fragment = (void *)(local) + size;
        prev->next = fragment;
        fragment->next = local->next;
        fragment->size = local->size - size;
        local->size = size;
        return (void *)(local) + sizeof(local->size);
      } else {
        prev->next = local->next;
        return (void *)(local) + sizeof(local->size);
      }
    }
    prev = local;
    local = local->next;
  }

  //  printf("malloc failed: request size:%d\n", size);
  return NULL;
}

void free(void *a) {
  struct heap_chunk *new = (void *)(a - sizeof(heap->size));
  struct heap_chunk *local = heap;
  struct heap_chunk *prev = heap;
  while (local != NULL) {
    if ((void *)(local) + local->size == new) {
      local->size += new->size;
      return;
    }
    if ((void *)(new) + new->size == local) {
      prev->next = new;
      new->next = local->next;
      new->size += local->size;
      return;
    }
    prev = local;
    local = local->next;
  }
  prev->next = new;
}

void exit(int code) {
  puts("\nexit");
  while (1)
    ;
}

int putchar(char c) {
  if (position == 80 * 25) {
    uint16_t *vga = (uint16_t *)0xb8000;
    for (int i = 80; i < 80 * 25; i++)
      vga[i - 80] = vga[i];
    for (int i = 80 * 24; i < 80 * 25; i++)
      vga_write('c', 0x00, i);
    position -= 80;
  }
  switch (c) {
  case '\n':
    position += 80 - (position % 80);
    break;
  default:
    vga_write(c, DEFAULT_COLOR, position);
    position++;
  }
  return 1;
}

size_t read(char *txt, size_t size) {
  int shift = 0;
  for (int i = 0; i < size; i++) {
    short c = receive_scancode();
    if (c == 0x2a) {
      shift = 1;
      i--;
      continue;
    } else if (c == 0xaa) {
      shift = 0;
      i--;
      continue;
    } else if (c > 127 || c < 0) {
      i--;
      continue;
    }
    if (shift) {
      txt[i] = scancode_to_ascii_shift[c];
    } else {
      txt[i] = scancode_to_ascii[c];
    }
    putchar(txt[i]);
  }
  return size;
}
