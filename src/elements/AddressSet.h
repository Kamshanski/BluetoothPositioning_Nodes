#pragma once
#ifndef ESP_BD_ADDR_LEN
#define ESP_BD_ADDR_LEN 6
#endif

#include <string>
#include <sstream>
#include <iomanip>
#include <string.h>
#include <stdio.h>

#include <MACAddresses.h>
/**
 * "Set" to collect all target devices
 */
class AddressSet  
{
private: 
	uint8_t *targets;
	int size = 0;
	int capacity;
	
	void resize(int newCapacity);
public: 
	static const int NEW_DEVICE_WAS_ADDED = 0;
	static const int DEVICE_WAS_REMOVED = 1;

	AddressSet();
	AddressSet(int initialCapacity);
	~AddressSet();

	int find(uint8_t *addr);
	void remove(uint8_t *addr);
	void remove(int pos);
	void add(uint8_t *addr);
	int addOrRemove(uint8_t addr[ESP_BD_ADDR_LEN]);

	std::string getAddress(int pos);
	int getSize();

	std::string toString();
	void getAddrString(int pos, char * buff);

};
