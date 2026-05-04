project(c_basic VERSION 1.0.0)

add_executable(${PROJECT_NAME}-pointer 01_Pointer/pointer.c)

add_executable(${PROJECT_NAME}-array
        02_VariableArray/fullyFlexibleVariableArray.c
        02_VariableArray/VariableArray.c
        02_VariableArray/main.c
)

add_executable(${PROJECT_NAME}-list
        03_LinkList/CircleLinkList.c
        03_LinkList/EnterpriseList.c
        03_LinkList/LinkList.c
        03_LinkList/main.c
)

add_executable(${PROJECT_NAME}-stack
        04_Stack/linkStack.c
        04_Stack/seqStack.c
        04_Stack/main.c
)

add_executable(${PROJECT_NAME}-queue
        05_Queue/linkQueue.c
        05_Queue/seqQueue.c
        05_Queue/main.c
)

add_executable(${PROJECT_NAME}-sort
        07_Sort/sort.c
        07_Sort/main.c
)