#include <pthread.h>
#include "caltrain.h"



void
station_init(struct station *station)
{
	station->waittingPassengers = 0;
	station->passengersOnBoard = 0;
	pthread_mutex_init(&(station->mutex), NULL);
	pthread_cond_init(&(station->conditionTrainFull), NULL);
	pthread_cond_init(&(station->conditionTrainArrive), NULL);
}

void
station_load_train(struct station *station, int count)
{
	station->freeSets = count;
	pthread_cond_broadcast(&(station->conditionTrainArrive));
	pthread_mutex_lock(&(station->mutex));
	while((station->freeSets !=0 && station->waittingPassengers != 0) || station->passengersOnBoard != 0)
		pthread_cond_wait(&(station->conditionTrainFull), &(station->mutex));
	station->freeSets = 0;
	pthread_mutex_unlock(&(station->mutex));
}

void
station_wait_for_train(struct station *station)
{
	pthread_mutex_lock(&(station->mutex));
	station->waittingPassengers += 1;
	while(station->freeSets == 0) {
		pthread_cond_wait(&(station->conditionTrainArrive), &(station->mutex));
	}
	station->freeSets -= 1;
	station->passengersOnBoard += 1;
	station->waittingPassengers -= 1;
	pthread_mutex_unlock(&(station->mutex));
}

void
station_on_board(struct station *station)
{
	pthread_mutex_lock(&(station->mutex));
	station->passengersOnBoard -= 1;
	if(station->passengersOnBoard == 0)
		pthread_cond_signal(&(station->conditionTrainFull));
	pthread_mutex_unlock(&(station->mutex));
}
