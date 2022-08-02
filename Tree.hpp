#pragma once
#include "Types.hpp"
#include <vector>
#include <queue>

namespace Tree {

    class NodeTree {
    public:
        explicit    NodeTree(const Types::Primitive & primitive)
        {
            _clonePrimitive(primitive);
            _bytes = _calcBytes();
        }
        explicit    NodeTree(const NodeTree & node)
        {
            _bytes      = node._bytes;
            _clonePrimitive(*(node._primitive));
            _buffer     = node._buffer;
            for(auto i = node._childs.begin(); i != node._childs.end(); i++)
                _childs.push_back(*i);
        }
        ~NodeTree() { delete _primitive; }
    private:
        void _clonePrimitive(const Types::Primitive & primitive)
        {
            switch(primitive.wtype())
            {
                case Types::WrapType::Text:
                {
                    _primitive = new Types::Text(std::string((char const *)primitive.data()));
                    break;
                }
                case Types::WrapType::I8:
                {
                    int8_t * value = (int8_t *)primitive.data();
                    _primitive = new Types::I8(*value);
                    break;
                }
                case Types::WrapType::I16:
                {
                    int16_t * value = (int16_t *)primitive.data();
                    _primitive = new Types::I16(*value);
                    break;
                }
                case Types::WrapType::I32:
                {
                    int32_t * value = (int32_t *)primitive.data();
                    _primitive = new Types::I32(*value);
                    break;
                }
                case Types::WrapType::I64:
                {
                    int64_t * value = (int64_t *)primitive.data();
                    _primitive = new Types::I64(*value);
                    break;
                }
                case Types::WrapType::Float:
                {
                    float * value = (float *)primitive.data();
                    _primitive = new Types::Float(*value);
                    break;
                }
                case Types::WrapType::Double:
                {
                    double * value = (double *)primitive.data();
                    _primitive = new Types::Double(*value);
                    break;
                }
                default: exit(EXIT_FAILURE);
            }
        }
        size_t _calcBytes() const
        {
            return sizeof(uint64_t) + sizeof(uint64_t) + (_primitive ? _primitive->bytes() : 0);
        }
    public:
        void        pack(Buffer & buffer)
        {
            for(size_t idx = 0; idx < _buffer.top(); idx++)
                buffer.push8bit(_buffer[idx]);
        }
        void        unpack(const Buffer & buffer)
        {
            _buffer = buffer;
        }
        void        save()
        {
            _bytes = _calcBytes();
            _buffer = Buffer(_bytes);
            uint64_t cntChilds = _childs.size();
            Core::encode(*(int64_t *)(&_bytes), _buffer);
            Core::encode(*(int64_t *)(&cntChilds), _buffer);
            _primitive->pack(_buffer);
        }
        void        retrieve()
        {
            _buffer.updateTop();
            if(_buffer.size() < _calcBytes()) return;
            Core::decode((int64_t *)(&_bytes), _buffer);
            uint64_t cntChilds;
            Core::decode((int64_t *)(&cntChilds), _buffer);
            _primitive->unpuck(_buffer);
        }
        void        pushChild(const NodeTree & node)
        {
            _childs.push_back(NodeTree(node));
        }
        void        pushChild(const Types::Primitive & primitive)
        {
            _childs.push_back(NodeTree(primitive));
        }
        void        printData() const
        {
            _primitive->printData();
        }
        void        printBuffer() const
        {
            _buffer.printConsole();
        }
    public:
        size_t      getChildSize() const    { return _childs.size();    }
        NodeTree &  getChild(size_t idx)    { return _childs.at(idx);   }
        NodeTree &  operator[](size_t idx)  { return _childs.at(idx);   }
        size_t      bytes() const           { return _bytes;            }
    private:
        size_t                  _bytes;
        Types::Primitive *      _primitive;
        Buffer                  _buffer;
        std::vector<NodeTree>   _childs;
    };



    typedef void (*callback_t)(NodeTree & node, void * arg);

    void calc_save_print_tree(NodeTree & node, void * arg)
    {
        node.printData();
        if(arg != nullptr)
        {
            uint64_t * bytes = static_cast<uint64_t *>(arg);
            (*bytes) = (*bytes) + node.bytes();
        }
        node.save();
        //node.printBuffer();
    }
    void printTreeData(NodeTree & node, void * arg)
    {
        node.printData();
    }
    void printTreeBuffers(NodeTree & node, void * arg)
    {
        node.printBuffer();
    }
    void packTree(NodeTree & node, void * arg)
    {
        Buffer * buffer = static_cast<Buffer *>(arg);
        node.pack(*buffer);
    }
    void traversalTree(NodeTree & root, callback_t cb, void * arg)
    {
        std::queue<NodeTree *> container;
        container.push(&root);

        while(!container.empty())
        {
            NodeTree * tmpp = container.front();
            container.pop();

            cb(*tmpp, arg);
            for(size_t idx = 0; idx < tmpp->getChildSize(); idx++)
                container.push(&(*tmpp)[idx]);
        }
    }

    namespace {
        Tree::NodeTree * _bufferToNodeTree(Buffer && buffer, Types::WrapType wtype)
        {
            uint64_t offset = 4*sizeof(uint64_t) + sizeof(int8_t);
            uint64_t dataSize = buffer.size() - offset;
            void * data = static_cast<void *>(((char *)(buffer.buffer())) + offset);
            Tree::NodeTree * retValue = nullptr;
            buffer.updateTop(offset);
            switch(wtype) {
                case Types::WrapType::Text:
                {
                    char * dataWithNull = new char[dataSize + 1];
                    char * dataWithoutNull = (char *)data;
                    for(size_t idx = 0; idx < dataSize; idx++)
                        dataWithNull[idx] = dataWithoutNull[idx];
                    dataWithNull[dataSize] = '\0';
                    retValue = new Tree::NodeTree(Types::Text(std::string(dataWithNull)));
                    break;
                }
                case Types::WrapType::I8:
                {
                    int8_t value;
                    Core::decode(&value, buffer);
                    retValue = new Tree::NodeTree(Types::I8(value));
                    break;
                }
                case Types::WrapType::I16:
                {
                    int16_t value;
                    Core::decode(&value, buffer);
                    retValue = new Tree::NodeTree(Types::I16(value));
                    break;
                }
                case Types::WrapType::I32:
                {
                    int32_t value;
                    Core::decode(&value, buffer);
                    retValue = new Tree::NodeTree(Types::I32(value));
                    break;
                }
                case Types::WrapType::I64:
                {
                    int64_t value;
                    Core::decode(&value, buffer);
                    retValue = new Tree::NodeTree(Types::I64(value));
                    break;
                }
                case Types::WrapType::Float:
                {
                    float value;
                    Core::decode(&value, buffer);
                    retValue = new Tree::NodeTree(Types::Float(value));
                    break;
                }
                case Types::WrapType::Double:
                {
                    double value;
                    Core::decode(&value, buffer);
                    retValue = new Tree::NodeTree(Types::Double(value));
                    break;
                }
                default: exit(EXIT_FAILURE);
            }
            retValue->save();
            return retValue;
        }
    }

    void retrieveTree(Tree::NodeTree ** retrieved_root, Buffer & buffer)
    {
        if(!buffer.size()) return;

        std::vector<uint64_t> vbytes;
        std::vector<uint64_t> vcntChilds;
        std::vector<Types::WrapType> vwtype;
        std::vector<Tree::NodeTree *> vnodes;

        int64_t bytes;
        int64_t cntChilds;
        Types::WrapType wtype;

        buffer.updateTop(0);
        size_t prevTop = 0;
        while(buffer.top() < buffer.size())
        {
            Core::decode((int64_t *)(&bytes), buffer);
            Core::decode((int64_t *)(&cntChilds), buffer);
            Core::decode((int8_t *)(&wtype), buffer);

            vbytes.push_back(bytes);
            vcntChilds.push_back(cntChilds);
            vwtype.push_back(wtype);

            vnodes.push_back(
                _bufferToNodeTree(Buffer(buffer, prevTop, prevTop + bytes), wtype)
            );

            buffer.updateTop(prevTop + bytes);
            prevTop = prevTop + bytes;
        }

        uint64_t offset = 1;
        uint64_t itemNumber = 0;
        std::queue<NodeTree *> container;
        container.push(vnodes[0]);

        while(!container.empty())
        {
            NodeTree * newRoot = container.front();
            container.pop();

            for(size_t idx = offset; idx < offset + vcntChilds[itemNumber]; idx++)
                newRoot->pushChild(*vnodes[idx]);

            for(size_t idx = 0; idx < vcntChilds[itemNumber]; idx++)
            {
                NodeTree * child = &((*newRoot)[idx]);
                container.push(child);
            }

            offset += vcntChilds[itemNumber];
            itemNumber++;
        }

        (*retrieved_root) = vnodes[0];
    }
}
