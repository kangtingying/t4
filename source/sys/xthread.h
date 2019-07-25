#ifndef XTHREAD_H
#define XTHREAD_H

#include <QThread>
#include <functional>
#include <QVariant>

class XThread : public QThread
{
    Q_OBJECT

public:
    typedef std::function<void(void)> XTHREAD_VOID_FUNC;
    typedef std::function<void(int &)> XTHREAD_INT_FUNC;
    typedef std::function<void(QString &)> XTHREAD_STRING_FUNC;
    typedef std::function<void( void *para)> XTHREAD_PTR_FUNC;

    static int count();

public:
    XThread(QObject *parent = NULL);

    XThread(XTHREAD_VOID_FUNC func, QObject *parent = NULL);
    XThread(XTHREAD_INT_FUNC func, QObject *parent = NULL);
    XThread(XTHREAD_STRING_FUNC func, QObject *parent = NULL);
    XThread(XTHREAD_PTR_FUNC func, void *ptr=NULL, QObject *parent = NULL);

    ~XThread();

protected:
    void init();

signals:
//    void signalFinishResult(int);
//    void signalFinishResult(QString);

    void signalFinishResult( QVariant var );

public:
    void setCallback(XTHREAD_VOID_FUNC  func);
    void setCallback(XTHREAD_INT_FUNC   func);
    void setCallback(XTHREAD_STRING_FUNC func);

protected:
    virtual void run();

private:
    static int m_count;

    void *m_pPtr;

    XTHREAD_VOID_FUNC   m_funcVoid;
    XTHREAD_INT_FUNC    m_funcInt;
    XTHREAD_STRING_FUNC m_funcString;
    XTHREAD_PTR_FUNC m_funcPtr;
};

#endif // XTHREAD_H
