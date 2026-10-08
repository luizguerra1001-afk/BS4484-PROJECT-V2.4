#pragma once


#include "Quaternion.h"


Quaternion GetRotationToLocation(Vector3 targetLocation, float y_bias, Vector3 myLoc){
    return Quaternion::LookRotation((targetLocation + Vector3(0, y_bias, 0)) - myLoc, Vector3(0, 1, 0));
}


// โครงสร้าง Entry ของ Dictionary (64-bit)
template <typename K, typename V>
struct monoEntry {
    int32_t hashCode;
    int32_t next;
    K key;
    V value;
};

// โครงสร้าง Array พื้นฐานใน Unity
template <typename T>
struct monoArray {
    void* klass;
    void* monitor;
    void* bounds;
    int   max_length;
    T vector[1]; // ข้อมูลจริงจะเริ่มที่ตำแหน่งนี้

    int getLength() {
        return (this && max_length > 0 && max_length < 10000) ? max_length : 0;
    }
};

// โครงสร้าง Dictionary ของ .NET
template <typename K, typename V>
struct monoDictionary {
    void* klass;
    void* monitor;
    monoArray<int>* buckets;
    monoArray<monoEntry<K, V>>* entries;
    int count;
    int freeList;
    int freeCount;
    int version;

    int getCapacity() {
        if (!this || !entries) return 0;
        return entries->max_length;
    }

    monoEntry<K, V>& getEntry(int i) {
        return entries->vector[i];
    }
};

template <typename T>
struct monoList {
    void *unk0;
    void *unk1;
    monoArray<T> *items;
    int size;
    int version;

    T getItems(){
        return items->getPointer();
    }

    int getSize(){
        return size;
    }

    int getVersion(){
        return version;
    }
};

typedef struct _monoString
{
    void* klass;
    void* monitor;
    int length;    
    char chars[1];   
    int getLength()
    {
      return length;
    }
    char* getChars()
    {
        return chars;
    }
}monoString;


/*
Get the real value of an ObscuredInt.
Parameters:
    - location: the location of the ObscuredInt
*/


