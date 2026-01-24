    global loader                   ; the entry symbol for ELF
    global receive_scancode
    global inb
    global outw

    MAGIC_NUMBER equ 0x1BADB002     ; define the magic number constant
    FLAGS        equ 0x0            ; multiboot flags
    CHECKSUM     equ -MAGIC_NUMBER  ; calculate the checksum
                                    ; (magic number + checksum + flags should equal 0)


    section .boot.text
    align 4                         ; the code must be 4 byte aligned
        dd MAGIC_NUMBER             ; write the magic number to the machine code,
        dd FLAGS                    ; the flags,
        dd CHECKSUM                 ; and the checksum

    extern kernel_main
    loader:                         ; the loader label (defined as entry point in linker script)
       mov edx,eax
       mov ax,0xf001
       out 0x60,ax
       mov esp, kernel_stack + KERNEL_STACK_SIZE 
       lea eax, end_of_mem
       push eax
       push edx
       push ebx
       call kernel_main
    .loop:
        jmp .loop                   ; loop forever
    enable_paging:
       mov eax, cr0
       or eax, 0x80000001
       mov cr0, eax
       ret
    receive_scancode:
      in al,64h
      test al,1
      je receive_scancode
      in al,60h
      ret
    inb:
      mov edx,[esp+4]
      in al,dx
      ret
    outw:
      mov edx,[esp+4]
      mov eax,[esp+8]
      out dx,ax
      ret
    outb:
      mov edx,[esp+4]
      mov eax,[esp+8]
      out dx,al
      ret
    set_page_directory:
      pop eax
      mov cr3,eax
      ret

    KERNEL_STACK_SIZE equ 16384; size of stack in bytes
    KERNEL_HEAP_SIZE equ 1048576; size of heap in bytes
    section .bss
    align 4                                     ; align at 4 bytes
    kernel_stack:                               ; label points to beginning of memory
        resb KERNEL_STACK_SIZE
    section .fini
    end_of_mem:


