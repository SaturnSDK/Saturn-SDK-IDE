#ifndef STATEMACHINEHELPER_H
#define STATEMACHINEHELPER_H

#include <stack>

template <class T>
class StateMachineData: public T
{
    public:
        int state;
        void CopyFrom(const T& src);
        void CopyTo(T& dst);
        StateMachineData() : state(0) {};
        StateMachineData(const T& src)
        {
            CopyFrom(src);
        };
};

template <class T>
class StateMachineHelper: public StateMachineData<T>
{
    public:
        bool m_StackError,m_IsJump,m_Finished;
        StateMachineHelper() : m_StackError(false),m_IsJump(false),m_Finished(false) {};
        void clear();
        void Go_to(int newstate);
        void Go_sub(int substate,const T &param);
        void Ret();
        void End();
        int GetNextState();

    protected:
        std::stack< StateMachineData<T> > CallStack;
};

template <class T>
void StateMachineHelper<T>::clear()
{
    this->state = 0;
    m_Finished = false;
    m_StackError = false;
    while(CallStack.size()) CallStack.pop();
}

template <class T>
void StateMachineHelper<T>::Go_to(int newstate)
{
    this->state = newstate; // Change state
    m_IsJump = true;
}

template <class T>
void StateMachineHelper<T>::Go_sub(int substate,const T &param)
{
    m_StackError = false;
    StateMachineData<T>* tmp = this;
    CallStack.push(*tmp); // Save local variables in stack
    m_IsJump = true;
    CopyFrom(param); // Get function parameters
    this->state = substate; // Change state
}

template <class T>
void StateMachineHelper<T>::Ret()
{
    if(!CallStack.size())
    {
        m_StackError = true;
        return;
    }

    CopyFrom(CallStack.top());
    this->state = CallStack.top().state;
    CallStack.pop();
}

template <class T>
void StateMachineHelper<T>::End()
{
    m_Finished = true;
}

template <class T>
int StateMachineHelper<T>::GetNextState()
{
    if(!m_IsJump)
        this->state++;
    m_IsJump = false;
    return this->state;
}
#endif
