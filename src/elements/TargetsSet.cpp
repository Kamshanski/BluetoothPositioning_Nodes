#include "TargetsSet.h"  

TargetsSet::TargetsSet() : TargetsSet::TargetsSet(10) {}

TargetsSet::TargetsSet(int initialCapacity) {
    capacity = initialCapacity;
    targets = new std::vector<MACAddress>(initialCapacity);
}

TargetsSet::~TargetsSet() {
    delete targets;
}

int TargetsSet::addOrRemove(MACAddress & addr) {
    int pos = find(addr);
    if (pos < 0) {
        add(addr);
        return NEW_DEVICE_WAS_ADDED;
    } else {
        remove(pos);
        return DEVICE_WAS_REMOVED;
    }
}

void TargetsSet::add(MACAddress addr) {
    if (size == capacity) {
        capacity+=10;
        targets->resize(capacity);
    }
    (*targets)[size] = addr;
    size++;

}

int TargetsSet::find(MACAddress& addr) {
    for (int i = 0; i < size; i++) {
        if ((*targets)[i].equals(&addr)) {
            return i;
        }
    }
    return -1;
}

void TargetsSet::remove(MACAddress& addr) {
    int pos = find(addr);
    delete &((*targets)[pos]);
    remove(pos);
}

void TargetsSet::remove(int pos) {
    (*targets)[pos] = (*targets)[size - 1];
    size--;
}