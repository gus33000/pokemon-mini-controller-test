#include "pm.h"

#include <stdint.h>

#include "hello_tiles.h"

#define A_BUTTON_SPRITE_TILE_ID 0
#define B_BUTTON_SPRITE_TILE_ID 1
#define C_BUTTON_SPRITE_TILE_ID 2
#define UP_BUTTON_SPRITE_TILE_ID 3
#define DOWN_BUTTON_SPRITE_TILE_ID 4
#define LEFT_BUTTON_SPRITE_TILE_ID 5
#define RIGHT_BUTTON_SPRITE_TILE_ID 6
#define POWER_BUTTON_SPRITE_TILE_ID 7

void setup_oam_sprites()
{
    // A
    OAM[A_BUTTON_SPRITE_TILE_ID].x = 16 * 6 - 8;
    OAM[A_BUTTON_SPRITE_TILE_ID].y = 16 * 2 + 8;
    OAM[A_BUTTON_SPRITE_TILE_ID].tile = A_BUTTON_SPRITE_TILE_ID;
    OAM[A_BUTTON_SPRITE_TILE_ID].ctrl = OAM_ENABLE | OAM_INVERT;

    // B
    OAM[B_BUTTON_SPRITE_TILE_ID].x = 16 * 5 - 8;
    OAM[B_BUTTON_SPRITE_TILE_ID].y = 16 * 3 + 8;
    OAM[B_BUTTON_SPRITE_TILE_ID].tile = B_BUTTON_SPRITE_TILE_ID;
    OAM[B_BUTTON_SPRITE_TILE_ID].ctrl = OAM_ENABLE | OAM_INVERT;

    // C
    OAM[C_BUTTON_SPRITE_TILE_ID].x = 16 * 6 - 4;
    OAM[C_BUTTON_SPRITE_TILE_ID].y = 16 + 4;
    OAM[C_BUTTON_SPRITE_TILE_ID].tile = C_BUTTON_SPRITE_TILE_ID;
    OAM[C_BUTTON_SPRITE_TILE_ID].ctrl = OAM_ENABLE | OAM_INVERT;

    // UP
    OAM[UP_BUTTON_SPRITE_TILE_ID].x = 16 * 2;
    OAM[UP_BUTTON_SPRITE_TILE_ID].y = 16 * 2 + 8;
    OAM[UP_BUTTON_SPRITE_TILE_ID].tile = UP_BUTTON_SPRITE_TILE_ID;
    OAM[UP_BUTTON_SPRITE_TILE_ID].ctrl = OAM_ENABLE | OAM_INVERT;

    // DOWN
    OAM[DOWN_BUTTON_SPRITE_TILE_ID].x = 16 * 2;
    OAM[DOWN_BUTTON_SPRITE_TILE_ID].y = 16 * 4 - 8;
    OAM[DOWN_BUTTON_SPRITE_TILE_ID].tile = DOWN_BUTTON_SPRITE_TILE_ID;
    OAM[DOWN_BUTTON_SPRITE_TILE_ID].ctrl = OAM_ENABLE | OAM_INVERT;

    // LEFT
    OAM[LEFT_BUTTON_SPRITE_TILE_ID].x = 16 + 8;
    OAM[LEFT_BUTTON_SPRITE_TILE_ID].y = 16 * 3;
    OAM[LEFT_BUTTON_SPRITE_TILE_ID].tile = LEFT_BUTTON_SPRITE_TILE_ID;
    OAM[LEFT_BUTTON_SPRITE_TILE_ID].ctrl = OAM_ENABLE | OAM_INVERT;

    // RIGHT
    OAM[RIGHT_BUTTON_SPRITE_TILE_ID].x = 16 * 3 - 8;
    OAM[RIGHT_BUTTON_SPRITE_TILE_ID].y = 16 * 3;
    OAM[RIGHT_BUTTON_SPRITE_TILE_ID].tile = RIGHT_BUTTON_SPRITE_TILE_ID;
    OAM[RIGHT_BUTTON_SPRITE_TILE_ID].ctrl = OAM_ENABLE | OAM_INVERT;

    // POWER
    OAM[POWER_BUTTON_SPRITE_TILE_ID].x = 16 * 4 - 8;
    OAM[POWER_BUTTON_SPRITE_TILE_ID].y = 16 * 2;
    OAM[POWER_BUTTON_SPRITE_TILE_ID].tile = POWER_BUTTON_SPRITE_TILE_ID;
    OAM[POWER_BUTTON_SPRITE_TILE_ID].ctrl = OAM_ENABLE | OAM_INVERT;
}

void initialize_audio_and_timer()
{
    // Initialize PTM_C in 16-bit mode
    TMR3_CTRL = 0x0082;
    TMR3_SCALE = 0x08;
    TMR3_OSC = 0;

    // Initialize volume
    AUD_CTRL &= 0xFC; // this is the default
    AUD_VOL = 2;      // 50% volume

    // Generate square wave at A4, ~440 Hz
    TMR3_PRE = 4544;
    TMR3_PVT = 2272; // half of PRE = 50% pulse width

    // Enable timer
    TMR3_CTRL_L = 0x04;
}

int main(void)
{
    uint8_t i, keys;

    PRC_MODE = COPY_ENABLE | SPRITE_ENABLE | MAP_ENABLE | MAP_16X12;
    PRC_RATE = RATE_36FPS;

    // Hack for now, todo: build proper background
    PRC_MAP = background_tiles;
    PRC_SPR = sprite_tiles;

    // Hack for now, todo: build proper background
    /*for (i=0; i<16*12; i++) {
        TILEMAP[i] = i;
    }*/

    setup_oam_sprites();
    initialize_audio_and_timer();

    for (;;)
    {
        wait_vsync();
        TMR1_OSC = 0;

        keys = ~KEY_PAD;

        // Each sprite in the OAM is indexed by the bit position of the key pad enum
        // So simply set the invert flag on the object which matches the set bit..
        for (i = 0; i < 8; i++)
        {
            if (keys & (1 << i))
            {
                if ((OAM[i].ctrl & OAM_INVERT))
                {
                    // Play sound
                    TMR1_OSC |= 0x20;

                    // Show sprite
                    OAM[i].ctrl &= ~OAM_INVERT;

                    // Wait one frame so we can hear the sound
                    wait_vsync();
                }
            }
            else
            {
                OAM[i].ctrl |= OAM_INVERT;
            }
        }
    }
}