#include "DequeTest.h"
int main(int argc, char const *argv[])
{
    // constructTest();
    deque<int> deq = addElementsTest();
    printTest("deq", deq);

    iteratorTest(deq);

    removeElementTest(deq);

    return 0;
}
