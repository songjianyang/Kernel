
#include "paging.h"
#include "kheap.h"

page_directory_t *kernel_directory = 0;
page_directory_t *current_directory = 0;

u32 *frames;
u32 nframes;

extern u32 placement_address;

#define INDEX_FROM_BIT(a) (a / (8 * 4))
#define OFFSET_FROM_BIT(a) (a % (8 * 4))

static void set_frame(u32 frame_addr)
{
    u32 frame = frame_addr / 0x1000;
    u32 idx = INDEX_FROM_BIT(frame);
    u32 off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
}

static void clear_frame(u32 frame_addr)
{
    u32 frame = frame_addr / 0x1000;
    u32 idx = INDEX_FROM_BIT(frame);
    u32 off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}

static u32 test_frame(u32 frame_addr)
{
    u32 frame = frame_addr / 0x1000;
    u32 idx = INDEX_FROM_BIT(frame);
    u32 off = OFFSET_FROM_BIT(frame);
    return (frames[idx] & (0x1 << off));
}

static u32 first_frame()
{
    u32 i, j;
    for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
    {

        if (frames[i] != 0xFFFFFFFF)
        {
            for (j = 0; j < 32; j++)
            {
                u32 toTest = 0x1 << j;
                if (!(frames[i] & toTest))
                {
                    return i * 4 * 8 + j;
                }
            }
        }
    }
}

void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
    if (page->frame != 0)
    {
        return;
    }
    else
    {
        u32 idx = first_frame();
        if (idx == (u32)-1)
        {
            printk("No free frames!");
            return;
        }
        set_frame(idx * 0x1000);
        page->present = 1;
        page->rw = (is_writeable) ? 1 : 0;
        page->user = (is_kernel) ? 0 : 1;
        page->frame = idx;
    }
}

// Function to deallocate a frame.
void free_frame(page_t *page)
{
    u32 frame;
    if (!(frame = page->frame))
    {
        return;
    }
    else
    {
        clear_frame(frame);
        page->frame = 0x0;
    }
}

void initialise_paging()
{
    u32 mem_end_page = 0x1000000;

    nframes = mem_end_page / 0x1000;
    frames = (u32 *)alloc_address(INDEX_FROM_BIT(nframes));
    memset(frames, 0, INDEX_FROM_BIT(nframes));

    kernel_directory = (page_directory_t *)alloc_address_a(sizeof(page_directory_t));
    memset(kernel_directory, 0, sizeof(page_directory_t));
    current_directory = kernel_directory;

    int i = 0;
    //printk("placement_address : %p\n",placement_address);
    while (i < mem_end_page)
    {
        alloc_frame(get_page(i, 1, kernel_directory), 0, 1);
        i += 0x1000;
    }

    register_interrupt_handler(14, page_fault);

    switch_page_directory(kernel_directory);
}

void switch_page_directory(page_directory_t *dir)
{
    current_directory = dir;
    asm volatile("mov %0, %%cr3" ::"r"(&dir->tablesPhysical));
    u32 cr0;
    asm volatile("mov %%cr0, %0"
                 : "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0" ::"r"(cr0));
}

page_t *get_page(u32 address, int make, page_directory_t *dir)
{
    // Turn the address into an index.
    address /= 0x1000;
    // Find the page table containing this address.
    u32 table_idx = address / 1024;
    if (dir->tables[table_idx]) // If this table is already assigned
    {
        return &dir->tables[table_idx]->pages[address % 1024];
    }
    else if (make)
    {
        u32 tmp;
        dir->tables[table_idx] = (page_table_t *)alloc_address_ap(sizeof(page_table_t), &tmp);
        memset(dir->tables[table_idx], 0, 1024 * 4);
        dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
        return &dir->tables[table_idx]->pages[address % 1024];
    }
    else
    {
        return 0;
    }
}

void page_fault(registers_t regs)
{
    u32 faulting_address;

    asm volatile("mov %%cr2, %0"
                 : "=r"(faulting_address));

    int present = !(regs.err_code & 0x1);
    int rw = regs.err_code & 0x2;
    int us = regs.err_code & 0x4;
    int reserved = regs.err_code & 0x8; // Overwritten CPU-reserved bits of page entry?
    int id = regs.err_code & 0x10;      // Caused by an instruction fetch?

    // Output an error message.
    printk("Page fault! ( ");
    if (present)
    {
        printk("present ");
    }
    if (rw)
    {
        printk("read-only ");
    }
    if (us)
    {
        printk("user-mode ");
    }
    if (reserved)
    {
        printk("reserved ");
    }
    printk(") at 0x");
    printk("%p", faulting_address);
    printk("\n");
    asm volatile("cli");
    asm volatile("hlt");
}
