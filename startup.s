.global _start
_start:
    ldr sp, =0x00100000  /* 將堆疊設在 1MB 處，離程式碼和 FB 都遠一點 */
    bl main
    b .