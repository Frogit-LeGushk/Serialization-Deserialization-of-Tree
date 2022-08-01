#pragma once
#include "Errors.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <inttypes.h>


/** container for data **/
class Buffer {
public: /** constructors, destructor and operator= **/
    Buffer() = default;
    explicit    Buffer(size_t size)
    {
        if(size == 0) return;
        _size = size;
        _buffer = new uint8_t[size];
    }
    explicit    Buffer(const Buffer & buf)
    {
        _size = buf._size;
        _top = buf._top;
        _buffer = new uint8_t[_size];
        for(size_t idx = 0; idx < _top; idx++) _buffer[idx] = buf._buffer[idx];
    }
    explicit    Buffer(Buffer && buf)
    {
        buf.swap(*this);
    }
    Buffer &    operator=(const Buffer & buf)
    {
        if(this != &buf) Buffer(buf).swap(*this);
        return *this;
    }
    Buffer &    operator=(Buffer && buf)
    {
        if(this != &buf) buf.swap(*this);
        return *this;
    }
    ~Buffer()
    {
        delete [] _buffer;
        _buffer = nullptr;
        _top = 0;
        _size = 0;
    }
private:
    void        swap(Buffer & buf) {
        std::swap(buf._top, _top);
        std::swap(buf._size, _size);
        std::swap(buf._buffer, _buffer);
    }
public: /** work with input/output operations **/
    void        saveInFile(std::string filename) const
    {
        std::ofstream out(filename, std::ios::out | std::ios::binary);
        for(size_t idx = 0; idx < _top; idx++) out << _buffer[idx];
    }
    void        loadFromFile(std::string filename)
    {
        std::ifstream in(filename, std::ios::in | std::ios::binary);
        this->~Buffer();
        in.seekg(0, std::ios::end);
        _size = in.tellg();
        _buffer = new uint8_t[_size];
        in.seekg(0, std::ios::beg);
        char byte;
        while(in.get(byte)) this->push8bit((uint8_t)byte);
    }
    void        printConsole() const
    {
        std::cout << "Buffer[" << _top << "] <";
        for(size_t idx = 0; idx < _top; idx++)
        {
            std::cout << " ";
            if(_buffer[idx] < 16) std::cout << "0";
            std::cout << std::hex << std::uppercase << +_buffer[idx];
        }
        std::cout << " >" << std::endl;
    }
public: /** adding and deleting methods **/
    void        push8bit(uint8_t value)
    {
        if(this->full()) throw NSErr::Error("Overflow buffer", NSErr::ErrEnum::OVERFLOW);
        _buffer[_top++] = value;
    }
    void        pop8bit()
    {
        if(_top == 0) throw NSErr::Error("Empty buffer", NSErr::ErrEnum::EMPTY);
        _top--;
    }
public: /** access element methods **/
    bool        empty() const           { return (_top == 0); }
    bool        full() const            { return (_top == _size); }
    uint8_t &   operator[](size_t idx)  { return _buffer[idx]; }
    uint8_t &   at(size_t idx)
    {
        if(idx >= _top) throw NSErr::Error("Range out index", NSErr::ErrEnum::RANGE_OUT);
        return _buffer[idx];
    }
public: /** getters and setters **/
    void *      buffer()        { return reinterpret_cast<void *>(_buffer); }
    size_t      top() const     { return _top;  }
    size_t      size() const    { return _size; }
    size_t      next()
    {
        if(_top == _size) throw NSErr::Error("Range out index", NSErr::ErrEnum::RANGE_OUT);
        return _top++;
    };
    void        updateTop(size_t newTop = 0)
    {
        if(newTop >= _size) throw NSErr::Error("Range out index", NSErr::ErrEnum::RANGE_OUT);
        _top = newTop;
    }
private:
    uint8_t *   _buffer = nullptr;
    size_t      _top = 0;
    size_t      _size = 0;
};
