ENTRY(_start)        /* 程式的進入點是 _start 標籤 */
SECTIONS
{
    . = 0x10000;      /* QEMU 的 VersatilePB 開發板從這個位址載入核心 */
    .text : {
        startup.o(.text) /* 確保啟動代碼放在最前面 */
        *(.text)         /* 剩下的程式碼 */
    }
    .data : { *(.data) } /* 全域變數 */
    .bss : { *(.bss) }   /* 未初始化的變數 */
    . = . + 0x1000;      /* 保留 4KB 給堆疊 (Stack) */
    stack_top = .;      /* 標記堆疊頂端位址 */
}