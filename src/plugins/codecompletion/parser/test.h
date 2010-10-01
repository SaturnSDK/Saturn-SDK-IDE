template <typename T>
class SmartPtr
{
public:
    SmartPtr(T* p) : _p(p) {}
    ~SmartPtr() { delete _p; }
    T* operator->() { return _p; }
    T& operator*() { return *_p; }

private:
    T* _p;
};

class Dummy
{
public:
    void Hello() {};
    int m_dummy;
};

typedef SmartPtr<Dummy> DummyPtr; // value-tip shoud be SmartPtr<Dummy>

int main()
{
    DummyPtr ptrDummy = DummyPtr(new Dummy());
    SmartPtr<Dummy> test(new Dummy());
    test->Hello();
    ptrDummy-> // works well now!
    return 0;
}
