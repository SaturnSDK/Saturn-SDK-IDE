class A
{
    void good()
    {
        int flyGo;
    }
};

int flyGo;

int main()
{
    int flyGo = 0;
    int testLocal = ++flyGo;
    testLocal += flyGo;
    ++flyGo;
    ++testLocal;
    return 0;
}
