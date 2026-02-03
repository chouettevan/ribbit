    global loader                   ; the entry symbol for ELF
    global receive_scancode
    global inb
    global outw
    global outb
    global lidt
    global idtr_generic
    global irq_handlers
    extern handle_irq


    MAGIC_NUMBER equ 0x1BADB002     ; define the magic number constant
    FLAGS        equ 0x0            ; multiboot flags
    CHECKSUM     equ -MAGIC_NUMBER  ; calculate the checksum
                                    ; (magic number + checksum + flags should equal 0)


%macro irq_handler 1
  irq_%1:
    pushad 
    push %1
    cld
    call handle_irq
    pop eax
    popad
    iret
%endmacro
%macro irq_entry 1
  dd irq_%1
%endmacro

    section .boot.text
    align 4                         ; the code must be 4 byte aligned
        dd MAGIC_NUMBER             ; write the magic number to the machine code,
        dd FLAGS                    ; the flags,
        dd CHECKSUM                 ; and the checksum

    extern kernel_main
    extern c_irq
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
    lidt:
      mov eax,[esp+4]
      lidt [eax]
      ret
    idtr_generic:
      cli
      pushad
      call c_irq
      popad
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
section .text
%assign i 0
%rep 256
  irq_handler i
%assign i i+1
%endrep
irq_handlers:
%assign i 0
%rep 256
  irq_entry i
%assign i i+1
%endrep

    KERNEL_STACK_SIZE equ 16384; size of stack in bytes
    section .bss
    align 4                                     ; align at 4 bytes
    kernel_stack:                               ; label points to beginning of memory
        resb KERNEL_STACK_SIZE
    section .fini
    end_of_mem:


