#pragma once

#include <vector>
#include <MACAddress.h>

/**
 * "Set" to collect all target devices
 */
class TargetsSet  
{
private: 
	std::vector<MACAddress> *targets;
	int size = 0;
	int capacity;
	static const int NEW_DEVICE_WAS_ADDED = 0;
	static const int DEVICE_WAS_REMOVED = 1;
	
	void remove(MACAddress & addr);
	void remove(int pos);
	void add(MACAddress addr);
public: 
	TargetsSet();
	TargetsSet(int initialCapacity);
	~TargetsSet();

	int find(MACAddress& addr);
	int addOrRemove(MACAddress& addr);
};
