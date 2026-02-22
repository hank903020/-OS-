// 硬體位址定義
#define PL110_LCD_BASE 0x10120000
#define PL110_LCD_UPPER_PANEL (PL110_LCD_BASE + 0x10)
#define PL110_LCD_CONTROL (PL110_LCD_BASE + 0x18)

volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;
void print_uart_simple(const char *s) {
    while (*s != '\0') {
        *UART0DR = (unsigned int)(*s); // 直接暴力寫入，不檢查狀態
        s++;
    }
}

// Framebuffer 設在 2MB 位址，解析度 640x480
volatile unsigned short * const FRAMEBUFFER = (unsigned short *)0x00200000;

// 字型定義：字母 'A' 的 8x8 點陣
unsigned char font_A[8] = {
    0x18, 0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x00  //           
};

void lcd_init() {
    // 必要的時序設定 (QEMU VersatilePB 專用)
    *(volatile unsigned int *)(PL110_LCD_BASE + 0x00) = 0x3F1F3F9C;
    *(volatile unsigned int *)(PL110_LCD_BASE + 0x04) = 0x090B61DF;
    
    // 設定 Framebuffer 起始點
    *(volatile unsigned int *)PL110_LCD_UPPER_PANEL = (unsigned int)FRAMEBUFFER;

    // 開啟 LCD: 16bpp (BGR 565), Power On, Enable
    *(volatile unsigned int *)PL110_LCD_CONTROL = 0x821 | (4 << 1); 
}

// 在 (x, y) 座標畫出一個 8x8 字元
void draw_char(unsigned char *font, int x, int y, unsigned short color) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            // 檢查該 row 的第 col 位元是否為 1 (從左邊開始算)
            if (font[row] & (0x80 >> col)) {
                // 每個 Row 有 640 個像素，16-bit 模式下每個像素佔 1 個 unsigned short
                FRAMEBUFFER[(y + row) * 640 + (x + col)] = color;
            }
        }
    }
}

int main() {
    lcd_init();

    // 1. 先背景塗成深藍色 (0xFFFF) 以區別白色字體
    for (int i = 0; i < 640 * 480; i++) {
        FRAMEBUFFER[i] = 0xFFFF; 
    }

    // 2. 畫出白色的字母 'A' (0x0010)
    draw_char(font_A, 100, 100, 0x0010);
	
	print_uart_simple("LCD should be white now.\r\n");
	
    while (1); // 進入死迴圈
    return 0;
}