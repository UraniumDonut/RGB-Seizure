#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <switch.h>

#define FB_WIDTH  1280
#define FB_HEIGHT 720

int main(int argc, char* argv[])
{
	svcOutputDebugString("Prepare your eyes", 18);
	svcOutputDebugString("Things are about to get flashy", 31);

    NWindow* win = nwindowGetDefault();

    Framebuffer fb;
    framebufferCreate(&fb, win, FB_WIDTH, FB_HEIGHT, PIXEL_FORMAT_RGBA_8888, 2);
    framebufferMakeLinear(&fb);

    u32 cnt = 0;

    while (appletMainLoop())
    {
        hidScanInput();

        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS)
            break;

        u32 stride;
        u32* framebuf = (u32*) framebufferBegin(&fb, &stride);

        if (cnt != 60)
            cnt = cnt + 12;
        else
            cnt = 0;

        for (u32 y = 0; y < FB_HEIGHT; y ++)
        {
            for (u32 x = 0; x < FB_WIDTH; x ++)
            {
                u32 pos = y * stride / sizeof(u32) + x;

                if (cnt <= 20)
					framebuf[pos] = 0xFF0000FF;
				else if (cnt <= 40)
					framebuf[pos] = 0xFF00FF00;
				else
					framebuf[pos] = 0xFFFF0000;
            }
        }

        framebufferEnd(&fb);
    }

    framebufferClose(&fb);
    return 0;
}
