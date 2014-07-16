/*
 * fifoBuffer.h
 *
 *  Created on: 16.07.2014
 *      Author: user
 */

#ifndef FIFOBUFFER_H_
#define FIFOBUFFER_H_

template <typename T, int N>
class fifoBuffer {

	// add a new element to end of queue
	// returns true on success and false if queue is full
	bool addElement(T element) {

		if (increaseIndex(lastIndex) == firstIndex) {
			return false; //buffer full
		} else {
			lastIndex = increaseIndex(lastIndex);
			buffer[lastIndex] = element;
			return true;
		}
	}

	// read the oldest element from queue
	// returns true on success and false if queue is empty
	bool getNextElement(T& element) {
		if (firstIndex == lastIndex) {
			return false; //empty
		} else {
			element = buffer[firstIndex];
			firstIndex = increaseIndex(firstIndex);
			return true;
		}
	}



private:

	inline uint8_t increaseIndex(uint8_t thisIndex){
		if (thisIndex < N-1) return thisIndex++;
		else return 0;
	}

	T buffer[N];

	int8_t firstIndex;
	int8_t lastIndex;
};


#endif /* FIFOBUFFER_H_ */
