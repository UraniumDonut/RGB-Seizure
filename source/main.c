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

    u32 cnt = 0;
    u32 stride;
    u32 color;

    while (appletMainLoop())
    {
        hidScanInput();

        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS)
            break;

        u32* framebuffer = (u32*) framebufferBegin(&fb, &stride);

        cnt++;

        if ((cnt % 3) == 0)
	            color = 0xFF0000FF;
        else if ((cnt % 2) == 0)
                color = 0xFF00FF00;
        else
                color = 0xFFFF0000;

        u32 size = FB_HEIGHT * stride;
        
        for (u32 pos = 0; pos < size; pos++) {
            framebuffer[pos] = color;
        }

        framebufferEnd(&fb);
    }

    framebufferClose(&fb);
    return 0;
}
