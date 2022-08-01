#pragma once
#include "Core.hpp"
#include <stdlib.h>
#include <iostream>


namespace Types {

    /** support types for serialization/deserialization **/
    enum class WrapType : int8_t {
        Text = 1,
        I8,
        I16,
        I32,
        I64,
        Float,
        Double
    };

    /** size in bytes for each type **/
    uint64_t wrapTypeSize(WrapType wtype)
    {
        switch(wtype) {
            case WrapType::Text:    return sizeof(char);
            case WrapType::I8:      return sizeof(int8_t);
            case WrapType::I16:     return sizeof(int16_t);
            case WrapType::I32:     return sizeof(int32_t);
            case WrapType::I64:     return sizeof(int64_t);
            case WrapType::Float:   return sizeof(float);
            case WrapType::Double:  return sizeof(double);
            default: exit(EXIT_FAILURE);
        }
    };

    class Primitive {
    protected: /** encapsulated constructor **/
        explicit Primitive(WrapType wtype, void * data, uint64_t length = 1)
            : _wtype(wtype), _size(wrapTypeSize(wtype)), _length(length), _data(data)
        {}
    public: /** destructor and virtual methods **/
        Primitive() = delete;
        Primitive(const Primitive & pr) = delete;
        virtual ~Primitive() {
            switch(_wtype) {
                case WrapType::Text:    delete [] ((char *)_data); break;
                case WrapType::I8:      delete ((int8_t *)_data); break;
                case WrapType::I16:     delete ((int16_t *)_data); break;
                case WrapType::I32:     delete ((int32_t *)_data); break;
                case WrapType::I64:     delete ((int64_t *)_data); break;
                case WrapType::Float:   delete ((float *)_data); break;
                case WrapType::Double:  delete ((double *)_data); break;
                default: exit(EXIT_FAILURE);
            }
            _size = 0;
            _length = 0;
            _data = nullptr;
        }
        virtual void pack(Buffer & buffer) = 0;
        virtual void unpuck(Buffer & buffer) = 0;
        void printData() {
            switch(_wtype) {
                case WrapType::Text:
                {
                    char * _data = (char *)data();
                    for(uint64_t idx = 0; idx < length(); idx++)
                        std::cout << _data[idx];
                    std::cout << std::endl;
                    break;
                }
                case WrapType::I8:      std::cout << +(*(int8_t *)data()) << std::endl; break;
                case WrapType::I16:     std::cout << *(int16_t *)data() << std::endl; break;
                case WrapType::I32:     std::cout << *(int32_t *)data() << std::endl; break;
                case WrapType::I64:     std::cout << *(int64_t *)data() << std::endl; break;
                case WrapType::Float:   std::cout << *(float *)data() << std::endl; break;
                case WrapType::Double:  std::cout << *(double *)data() << std::endl; break;
                default: exit(EXIT_FAILURE);
            }
        };
    protected:
        void packMeta(Buffer & buffer)
        {
            Core::encode((int8_t)wtype(), buffer);
            Core::encode(*(int64_t *)(&size()), buffer);
            Core::encode(*(int64_t *)(&length()), buffer);
        }
        void unpackMeta(Buffer & buffer)
        {
            this->~Primitive();
            Core::decode((int8_t *)(&wtype()), buffer);
            Core::decode((int64_t *)(&size()), buffer);
            Core::decode((int64_t *)(&length()), buffer);
        }
    public: /** general getters/setters **/
        WrapType &  wtype()     { return _wtype; }
        uint64_t &  size()      { return _size; }
        void *      data()      { return _data; }
        uint64_t &  length()    { return _length; }
    protected:
        void        setData(void * data) { _data = data; }
    private: /** all fields **/
        WrapType    _wtype;
        uint64_t    _size;
        uint64_t    _length;
        void *      _data = nullptr;
    };

    class Text  : public Primitive {
    public:
        explicit Text(std::string text)
            : Primitive(WrapType::Text, (void *)(new char[text.size()]), (uint64_t)text.size())
        {
            for(uint64_t idx = 0; idx < text.size(); idx++)
                ((char *)data())[idx] = text.c_str()[idx];
        }
    public:
        void pack(Buffer & buffer) override
        {
            packMeta(buffer);
            int8_t *    _data   = (int8_t *)data();
            for(uint64_t idx = 0; idx < length(); idx++)
                Core::encode(_data[idx], buffer);
        }
        void unpuck(Buffer & buffer) override
        {
            unpackMeta(buffer);
            setData(new char[length()]);
            int8_t *    _data   = (int8_t *)data();
            for(uint64_t idx = 0; idx < length(); idx++)
                Core::decode(&_data[idx], buffer);
        }
    };

    class I8    : public Primitive {
    public:
        explicit I8(int8_t value)
            : Primitive(WrapType::I8, (void *)(new int8_t(value)))
        {}
    public:
        void pack(Buffer & buffer) override
        {
            packMeta(buffer);
            int8_t * _data   = (int8_t *)data();
            Core::encode(_data[0], buffer);
        }
        void unpuck(Buffer & buffer) override
        {
            unpackMeta(buffer);
            setData(new int8_t());
            int8_t * _data   = (int8_t *)data();
            Core::decode(_data, buffer);
        }
    };
    class I16   : public Primitive {
    public:
        explicit I16(int16_t value)
            : Primitive(WrapType::I16, (void *)(new int16_t(value)))
        {}
    public:
        void pack(Buffer & buffer) override
        {
            packMeta(buffer);
            int16_t * _data   = (int16_t *)data();
            Core::encode(_data[0], buffer);
        }
        void unpuck(Buffer & buffer) override
        {
            unpackMeta(buffer);
            setData(new int16_t());
            int16_t * _data   = (int16_t *)data();
            Core::decode(_data, buffer);
        }
    };
    class I32   : public Primitive {
    public:
        explicit I32(int32_t value)
            : Primitive(WrapType::I32, (void *)(new int32_t(value)))
        {}
    public:
        void pack(Buffer & buffer) override
        {
            packMeta(buffer);
            int32_t * _data   = (int32_t *)data();
            Core::encode(_data[0], buffer);
        }
        void unpuck(Buffer & buffer) override
        {
            unpackMeta(buffer);
            setData(new int32_t());
            int32_t * _data   = (int32_t *)data();
            Core::decode(_data, buffer);
        }
    };
    class I64   : public Primitive {
    public:
        explicit I64(int64_t value)
            : Primitive(WrapType::I64, (void *)(new int64_t(value)))
        {}
    public:
        void pack(Buffer & buffer) override
        {
            packMeta(buffer);
            int64_t * _data   = (int64_t *)data();
            Core::encode(_data[0], buffer);
        }
        void unpuck(Buffer & buffer) override
        {
            unpackMeta(buffer);
            setData(new int64_t());
            int64_t * _data   = (int64_t *)data();
            Core::decode(_data, buffer);
        }
    };
    class Float : public Primitive {
    public:
        explicit Float(float value)
            : Primitive(WrapType::Float, (void *)(new float(value)))
        {}
    public:
        void pack(Buffer & buffer) override
        {
            packMeta(buffer);
            float * _data   = (float *)data();
            Core::encode(_data[0], buffer);
        }
        void unpuck(Buffer & buffer) override
        {
            unpackMeta(buffer);
            setData(new float());
            float * _data   = (float *)data();
            Core::decode(_data, buffer);
        }
    };
    class Double: public Primitive {
    public:
        explicit Double(double value)
            : Primitive(WrapType::Double, (void *)(new double(value)))
        {}
    public:
        void pack(Buffer & buffer) override
        {
            packMeta(buffer);
            double * _data   = (double *)data();
            Core::encode(_data[0], buffer);
        }
        void unpuck(Buffer & buffer) override
        {
            unpackMeta(buffer);
            setData(new double());
            double * _data   = (double *)data();
            Core::decode(_data, buffer);
        }
    };


}
