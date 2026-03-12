#include "pmm.h"

#define FRAME_SIZE 4096
#define MAX_MEMORY (128 * 1024 * 1024) // 128MB
#define MAX_FRAMES (MAX_MEMORY / FRAME_SIZE)

static uint32_t frame_bitmap[MAX_FRAMES / 32];

static uint32_t total_frames;

static void set_frame(uint32_t frame)
{
    frame_bitmap[frame / 32] |= (1 << (frame % 32));
}

static void clear_frame(uint32_t frame)
{
    frame_bitmap[frame / 32] &= ~(1 << (frame % 32));
}

static int test_frame(uint32_t frame)
{
    return frame_bitmap[frame / 32] & (1 << (frame % 32));
}

void pmm_init(uint32_t mem_size)
{
    total_frames = mem_size / FRAME_SIZE;

    for (uint32_t i = 0; i < total_frames; i++)
        set_frame(i);  // mark all used initially

    // Free first 128MB (for now naive approach)
    for (uint32_t i = 0; i < total_frames; i++)
        clear_frame(i);

    // But reserve first frame (null page)
    set_frame(0);
}

uint32_t pmm_alloc_frame()
{
    for (uint32_t i = 0; i < total_frames; i++)
    {
        if (!test_frame(i))
        {
            set_frame(i);
            return i * FRAME_SIZE;
        }
    }

    return 0; // Out of memory
}

void pmm_free_frame(uint32_t addr)
{
    uint32_t frame = addr / FRAME_SIZE;
    clear_frame(frame);
}
