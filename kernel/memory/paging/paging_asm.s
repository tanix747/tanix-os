    .globl load_page_directory
load_page_directory:
    mov 4(%esp), %eax
    mov %eax, %cr3
    ret

    .globl enable_paging
enable_paging:
    mov %cr0, %eax
    or $0x80000000, %eax
    mov %eax, %cr0
    ret
    
    .globl flush_tlb
flush_tlb:
    mov %cr3, %eax
    mov %eax, %cr3
    ret


