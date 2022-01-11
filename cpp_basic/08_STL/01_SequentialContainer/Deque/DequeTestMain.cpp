#include "DequeTest.h"
int main(int argc, char const *argv[])
{
    // constructTest();
    deque<int> deq = addElementsTest();
    printTest("deq", deq);

    iteratorTest(deq);

    accessDeleteTest(deq);

    return 0;
}
