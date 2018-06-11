
class Averager {
	static const int numReadings = 5;

	int readings[numReadings];      // the readings from the analog input
	int readIndex = 0;              // the index of the current reading
	int total = 0;                  // the running total
	int average = 0;                // the average

	public:

	Averager(){
		for (int thisReading = 0; thisReading < numReadings; thisReading++) {
  		readings[thisReading] = 0;
  	}
	}

	void push(float newVal){
		total = total - readings[readIndex];
		readings[readIndex] = newVal;
		total = total + readings[readIndex];
		readIndex = readIndex + 1;

		// if we're at the end of the array...
  	if (readIndex >= numReadings) {
    	// ...wrap around to the beginning:
    	readIndex = 0;
  	}

  	average = total / numReadings;
	}

	float getAverage(){
		return average;
	}

};
