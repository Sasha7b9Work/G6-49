int main()
{
    volatile static int i = 0;
    
    while(1)
    {
        ++i;
    }
}
