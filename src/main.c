#include "pm.h"

#include <stdint.h>

#include "hello_tiles.h"

int main(void)
{
    uint8_t i, keys;
    
    PRC_MODE = COPY_ENABLE|SPRITE_ENABLE|MAP_ENABLE|MAP_16X12;
    PRC_RATE = RATE_36FPS;

    // Hack for now, todo: build proper background
    PRC_MAP = background_tiles;
    PRC_SPR = sprite_tiles;

    // Hack for now, todo: build proper background
    /*for (i=0; i<16*12; i++) {
        TILEMAP[i] = i;
    }*/
   
    // A
    OAM[0].x = 16 * 6 - 8;
    OAM[0].y = 16 * 2 + 8;
    OAM[0].tile = 0;
    OAM[0].ctrl = OAM_ENABLE | OAM_INVERT;
    
    // B
    OAM[1].x = 16 * 5 - 8;
    OAM[1].y = 16 * 3 + 8;
    OAM[1].tile = 1;
    OAM[1].ctrl = OAM_ENABLE | OAM_INVERT;
    
    // C
    OAM[2].x = 16 * 6 - 4;
    OAM[2].y = 16 + 4;
    OAM[2].tile = 2;
    OAM[2].ctrl = OAM_ENABLE | OAM_INVERT;
    
    // UP
    OAM[3].x = 16 * 2;
    OAM[3].y = 16 * 2 + 8;
    OAM[3].tile = 3;
    OAM[3].ctrl = OAM_ENABLE | OAM_INVERT;
    
    // DOWN
    OAM[4].x = 16 * 2;
    OAM[4].y = 16 * 4 - 8;
    OAM[4].tile = 4;
    OAM[4].ctrl = OAM_ENABLE | OAM_INVERT;
    
    // LEFT
    OAM[5].x = 16 + 8;
    OAM[5].y = 16 * 3;
    OAM[5].tile = 5;
    OAM[5].ctrl = OAM_ENABLE | OAM_INVERT;

    // RIGHT
    OAM[6].x = 16 * 3 - 8;
    OAM[6].y = 16 * 3;
    OAM[6].tile = 6;
    OAM[6].ctrl = OAM_ENABLE | OAM_INVERT;
    
    // POWER
    OAM[7].x = 16 * 4 - 8;
    OAM[7].y = 16 * 2;
    OAM[7].tile = 7;
    OAM[7].ctrl = OAM_ENABLE | OAM_INVERT;
    
	// Initialize PTM_C in 16-bit mode
	TMR3_CTRL = 0x0082;
	TMR3_SCALE = 0x08;
	TMR3_OSC = 0;

	// Initialize volume
	AUD_CTRL &= 0xFC; // this is the default
	AUD_VOL = 2; // 50% volume

	// Generate square wave at A4, ~440 Hz
	TMR3_PRE = 4544;
	TMR3_PVT = 2272; // half of PRE = 50% pulse width

	// Enable timer
	TMR3_CTRL_L = 0x04;

    for(;;) {
        wait_vsync();
	    TMR1_OSC = 0;
        
        keys = ~KEY_PAD;

        // Each sprite in the OAM is indexed by the bit position of the key pad enum
        // So simply set the invert flag on the object which matches the set bit..
        for (i = 0; i < 8; i++) {
            if (keys & (1 << i)) {
                if ((OAM[i].ctrl & OAM_INVERT)) {
                    // Play sound
	                TMR1_OSC |= 0x20;

                    // Show sprite
                    OAM[i].ctrl &= ~OAM_INVERT;

                    // Wait one frame so we can hear the sound
                    wait_vsync();
                }
            } else {
                OAM[i].ctrl |= OAM_INVERT;
            }
        }
    }
}