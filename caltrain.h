#include <pthread.h>

struct station {
	int freeSets;
	int waittingPassengers;
	int passengersOnBoard;
	pthread_mutex_t mutex;
	pthread_cond_t conditionTrainFull;
	pthread_cond_t conditionTrainArrive;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
