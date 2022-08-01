#include "Types.hpp"



int main(void)
{
    Buffer buffer1(100);
    Buffer buffer2(100);
    Buffer buffer3(100);
    Buffer buffer4(100);
    Buffer buffer5(100);
    Buffer buffer6(100);
    Buffer buffer7(100);

    Types::Text     entity1(std::string("Hello, world"));
    Types::I8       entity2(8);
    Types::I16      entity3(16);
    Types::I32      entity4(32);
    Types::I64      entity5(64);
    Types::Float    entity6(3.14);
    Types::Double   entity7(6.28);

    Types::Primitive & prim1 = entity1;
    Types::Primitive & prim2 = entity2;
    Types::Primitive & prim3 = entity3;
    Types::Primitive & prim4 = entity4;
    Types::Primitive & prim5 = entity5;
    Types::Primitive & prim6 = entity6;
    Types::Primitive & prim7 = entity7;

    prim1.pack(buffer1);
    prim2.pack(buffer2);
    prim3.pack(buffer3);
    prim4.pack(buffer4);
    prim5.pack(buffer5);
    prim6.pack(buffer6);
    prim7.pack(buffer7);

    buffer1.printConsole();
    buffer2.printConsole();
    buffer3.printConsole();
    buffer4.printConsole();
    buffer5.printConsole();
    buffer6.printConsole();
    buffer7.printConsole();

    buffer1.updateTop();
    buffer2.updateTop();
    buffer3.updateTop();
    buffer4.updateTop();
    buffer5.updateTop();
    buffer6.updateTop();
    buffer7.updateTop();

    prim1.unpuck(buffer1);
    prim2.unpuck(buffer2);
    prim3.unpuck(buffer3);
    prim4.unpuck(buffer4);
    prim5.unpuck(buffer5);
    prim6.unpuck(buffer6);
    prim7.unpuck(buffer7);

    prim1.printData();
    prim2.printData();
    prim3.printData();
    prim4.printData();
    prim5.printData();
    prim6.printData();
    prim7.printData();


    return 0;
}
