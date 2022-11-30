

#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool sched_helper( DailySchedule& sched, int num_days, int d_needed, const AvailabilityMatrix& avail, int n, int d, const size_t maxShifts);


bool isValid(DailySchedule& sched, int num_days, int d_needed, const AvailabilityMatrix& avail, const size_t maxShifts);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    int n = (int)avail.size();

    //fill sched with invalid ID in each spot
    for(int i = 0; i < n; i++){
        std::vector<std::vector<bool>> AvailabilityMatrix;
        std::vector<Worker_T> day;
        for(int j = 0; j < (int)dailyNeed; j++){
            day.push_back(INVALID_ID);
        }
        sched.push_back(day);
    }
		//call helper
		return sched_helper(sched, 0, 0, avail, n, (int)dailyNeed, maxShifts);

}

bool sched_helper(DailySchedule& sched, int num_days, int d_needed, const AvailabilityMatrix& avail, int n, int d, const size_t maxShifts)
{
	//if we made a complete schedule, return true, should be valid
	if(num_days == n -1 && d_needed == d){
		return true;
	}

	//if we have gone through the cols, now recurse to next row
	if(d_needed == d){
		return sched_helper(sched, num_days+1, 0, avail, n, d, maxShifts);
	}
	//if the current place is empty, try each id and check if it is a valid sched
	//if it is, keep recursing to next spot and if those are valid, return true 
	//until schedule is full
	if(sched[num_days][d_needed] == INVALID_ID){
		for(int v = 0; v < (int)avail[0].size(); v++){
				sched[num_days][d_needed] = v;
			if(isValid(sched, num_days, d_needed, avail, maxShifts)){
				if(sched_helper(sched, num_days, d_needed+1, avail, n, d, maxShifts) == true){
					return true;
				}
			}
		}
		//reset changes
		sched[num_days][d_needed] = INVALID_ID;
		return false;
	}
	//otherwise recurse to next position
	else{
			return sched_helper(sched, num_days, d_needed+1, avail, n, d, maxShifts);
	}
}

//determines if current state is a valid schedule
bool isValid(DailySchedule& sched, int num_days, int d_needed, const AvailabilityMatrix& avail, const size_t maxShifts)
{
	//get the current value
	int value = sched[num_days][d_needed];

	//if the person we added to the schedule is not available not valid
	if(avail[num_days][value] == 0){
		return false;
	}


	//if they are scheduled multiple times in one day not valid
	for(int i = 0; i < d_needed; i++){
		if((int)sched[num_days][i] == value){
			return false;
		}
	}

	int counter = 0;

	int d = sched[0].size();
	int n = sched.size();

	//count how many times they are scheduled
	for(int i = 0; i < d; i++){
		for(int j = 0; j < n; j++){
			if((int)sched[j][i] == value){
				counter++;
			}
		}

	}

	//if scheduled more than max shifts allows, not valid
	if(counter > (int)maxShifts){
		return false;
	}

	//otherwise should be valid
	return true;
	
}
