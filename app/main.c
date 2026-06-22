int main(void)
{
    volatile int app_running = 1234;

    while(1){
        __asm volatile("nop");
    }
}