有几个bug

在双击st2的时候，我原本是
```cpp
    if(can == false){
        can = true;
        list->clear();
        list = list2;
        size = list->size();
    }
```
但是这个list和list2都是指针，会导致第二次clear()的时候把list2给clear()了
```cpp
    if(can == false){
        can = true;
        list->clear();
        *list = *list2;
        size = list->size();
    }
```
这样修改之后就没有bug了

但是成品没有修改太麻烦了，只在源码中进行了修改
