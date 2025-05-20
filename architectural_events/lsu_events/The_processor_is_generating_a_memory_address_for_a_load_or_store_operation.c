int main(void)
{
    volatile int array[4] = {0, 1, 2, 3};
    volatile int value;

    /* Access memory with a load to provoke dbus_adr_o signal generation */
    value = array[2];

    /* Access memory with a store to provoke dbus_adr_o signal generation */
    array[1] = value;

    return 0;
}