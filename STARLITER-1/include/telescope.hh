#ifndef EVENT_STRUCT
#define EVENT_STRUCT
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4ios.hh"

struct event_t
{
	double energy[24][8];
	double theta[24][8];
	int ring[24];
	int sector[8];
};
#endif

