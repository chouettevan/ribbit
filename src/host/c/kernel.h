typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef unsigned size_t;
#define NULL (void *)0
extern void enable_paging(void);
struct huge_page_addr {
  unsigned pat : 1;
  unsigned addr_end : 7;
  unsigned reserved : 1;
  unsigned addr_start : 9;
};

struct heap_chunk {
  size_t size;
  struct heap_chunk *next;
};

struct page_directory_entry {
  unsigned present : 1;
  unsigned writable : 1;
  unsigned supervisor : 1;
  unsigned cache_write_mode : 1;
  unsigned no_cache : 1;
  unsigned accessed : 1;
  unsigned dirty : 1;
  unsigned is_large : 1;
  unsigned global : 1;
  unsigned free_bits : 2;
  union {
    unsigned page_table_addr : 20;
    struct huge_page_addr huge;
  };
};

struct page_table_entry {
  unsigned present : 1;
  unsigned writable : 1;
  unsigned supervisor : 1;
  unsigned cache_write_mode : 1;
  unsigned no_cache : 1;
  unsigned accessed : 1;
  unsigned dirty : 1;
  unsigned is_large : 1;
  unsigned global : 1;
  unsigned free_bits : 2;
  unsigned page_addr : 20;
};

typedef struct {
  struct page_directory_entry entries[1024];
} page_directory;

typedef struct {
  size_t (*write)(char *txt, size_t len);
  size_t (*read)(char *buf, size_t len);
} FILE;
extern int errno;
extern char kernel_heap[];
extern struct heap_chunk *heap;

extern void set_page_directory(page_directory *page);
extern void vga_write(char c, uint8_t color, int index);
extern void exit(int code);
extern void fclose(FILE *file);
extern FILE *fopen(const char *filename, const char *mode);
extern void init();
extern int puts(const char *txt);
extern void *memcpy(char *, const char *, size_t);
extern void write(const char *txt, size_t len);
extern void perror(const char *s);
extern int fflush(FILE *stream);
extern void binary_print(int);
extern char getchar(void);
extern size_t read(char *txt, size_t size);
extern FILE *fdopen(int fd, const char *modes);
extern size_t fread(void *ptr, size_t size, size_t n, FILE *file);
extern size_t fwrite(const void *ptr, size_t size, size_t n, FILE *file);
extern char inb(unsigned short port);
extern void outw(unsigned short, unsigned short);
extern int putchar(char);
extern char receive_scancode(void);
// TODO:implement these
extern void *malloc(size_t size);
extern void free(void *ptr);
extern int printf(const char *format, ...);
