#include "..\common.h"

int main(){
	String text = readFile("input.txt");
	
	char* position = text.data;

	long arrivalTime = strtol(position, &position, 10);
	printf("Arrival time: %ld\n", arrivalTime);
	position++;

	long earliestBusID = -1;
	long earliestWaitingTime = LONG_MAX;
	while ((position - text.data) < text.size){
		if(*position != 'x'){
			long busID = strtol(position, &position, 10);
			printf("%ld\n", busID);
			long waitingTime = (arrivalTime % busID);
			if(waitingTime != 0){
				waitingTime = busID - waitingTime;
			}

			if(waitingTime < earliestWaitingTime){
				earliestBusID = busID;
				earliestWaitingTime = waitingTime;
			}
		} else {
			position++;
		}
		position++;
	}
	printf("Earliest bus ID: %ld\n", earliestBusID);
	printf("Waiting time: %ld\n", earliestWaitingTime);
	printf("ID * Waiting time = %ld\n", earliestBusID * earliestWaitingTime);
	return 0;
}