#pragma once


// noncopyable 被继承以后，派生类可以正常的构造和析构，但是
//派生类对象无法进行拷贝构造和赋值操作
class noncopyable
{
public:
    noncopyable(const noncopyable&)=delete;
    noncopyable& operator=(const noncopyable&)=delete;
protected:
    noncopyable() = default;
    ~noncopyable() =default;
};