#include "pm.h"

#include <stdint.h>

#include "hello_tiles.h"

uint8_t free_time;

int main(void)
{
    uint8_t i, keys;
    
    PRC_MODE = COPY_ENABLE|SPRITE_ENABLE|MAP_ENABLE|MAP_16X12;
    PRC_RATE = RATE_36FPS;

    // Hack for now, todo: build proper background
    PRC_MAP = hello_tiles;
    PRC_SPR = hello_tiles;

    // Hack for now, todo: build proper background
    /*for (i=0; i<16*12; i++) {
        TILEMAP[i] = i;
    }*/
    
    for(;;) {
        TMR1_OSC = 0x11; // Use Oscillator 2 (31768Hz)
        TMR1_SCALE = 0x08 | 0x02; // Scale 2 (8192 Hz)
        TMR1_CTRL = 0x06; // Enable timer 2 at 0
        wait_vsync();
        TMR1_CTRL = 0; // Pause timer
        free_time = 255-TMR1_CNT_L;
        
        keys = ~KEY_PAD;

        if (keys & KEY_A) {
            OAM[0].x = 16 * 6;
            OAM[0].y = 16 * 2 + 8;
            OAM[0].tile = 0;
            OAM[0].ctrl = OAM_INVERT | OAM_ENABLE;
        } else {
            OAM[0].x = 16 * 6;
            OAM[0].y = 16 * 2 + 8;
            OAM[0].tile = 0;
            OAM[0].ctrl = 0;
        }

        if (keys & KEY_B) {
            OAM[1].x = 16 * 5;
            OAM[1].y = 16 * 3 + 8;
            OAM[1].tile = 0;
            OAM[1].ctrl = OAM_INVERT | OAM_ENABLE;
        } else {
            OAM[1].x = 16 * 5;
            OAM[1].y = 16 * 3 + 8;
            OAM[1].tile = 0;
            OAM[1].ctrl = 0;
        }

        if (keys & KEY_C) {
            OAM[2].x = 16 * 6;
            OAM[2].y = 16;
            OAM[2].tile = 0;
            OAM[2].ctrl = OAM_INVERT | OAM_ENABLE;
        } else {
            OAM[2].x = 16 * 6;
            OAM[2].y = 16;
            OAM[2].tile = 0;
            OAM[2].ctrl = 0;
        }

        if (keys & KEY_UP) {
            OAM[3].x = 16 * 2;
            OAM[3].y = 16 * 2 + 8;
            OAM[3].tile = 0;
            OAM[3].ctrl = OAM_INVERT | OAM_ENABLE;
        } else {
            OAM[3].x = 16 * 2;
            OAM[3].y = 16 * 2 + 8;
            OAM[3].tile = 0;
            OAM[3].ctrl = 0;
        }

        if (keys & KEY_DOWN) {
            OAM[4].x = 16 * 2;
            OAM[4].y = 16 * 4 - 8;
            OAM[4].tile = 0;
            OAM[4].ctrl = OAM_INVERT | OAM_ENABLE;
        } else {
            OAM[4].x = 16 * 2;
            OAM[4].y = 16 * 4 - 8;
            OAM[4].tile = 0;
            OAM[4].ctrl = 0;
        }

        if (keys & KEY_LEFT) {
            OAM[5].x = 16 + 8;
            OAM[5].y = 16 * 3;
            OAM[5].tile = 0;
            OAM[5].ctrl = OAM_INVERT | OAM_ENABLE;
        } else {
            OAM[5].x = 16 + 8;
            OAM[5].y = 16 * 3;
            OAM[5].tile = 0;
            OAM[5].ctrl = 0;
        }

        if (keys & KEY_RIGHT) {
            OAM[6].x = 16 * 3 - 8;
            OAM[6].y = 16 * 3;
            OAM[6].tile = 0;
            OAM[6].ctrl = OAM_INVERT | OAM_ENABLE;
        } else {
            OAM[6].x = 16 * 3 - 8;
            OAM[6].y = 16 * 3;
            OAM[6].tile = 0;
            OAM[6].ctrl = 0;
        }

        if (keys & KEY_POWER) {
            OAM[7].x = 16 * 4 - 8;
            OAM[7].y = 16 * 2;
            OAM[7].tile = 0;
            OAM[7].ctrl = OAM_INVERT | OAM_ENABLE;
        } else {
            OAM[7].x = 16 * 4;
            OAM[7].y = 16 * 2;
            OAM[7].tile = 0;
            OAM[7].ctrl = 0;
        }
    }
}
