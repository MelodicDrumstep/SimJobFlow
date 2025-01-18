#include <gtest/gtest.h>
#include <fstream>
#include <nlohmann/json.hpp>

#include "greedy_scheduler.hpp"

#include "basic_utils_in_one_header.hpp"

using namespace SJF;

using json = nlohmann::json;

TEST(GreedySchedulerTest, BasicOperations) 
{
    NanoLog::setLogFile("./BasicOperations.log");
    NanoLog::setLogLevel(DEBUG);
    constexpr int64_t num_of_machines = 2;
    std::vector<IdenticalMachine> machines(num_of_machines);
    for(int i = 0; i < num_of_machines; i++)
    {
        machines[i].machineId_ = i;
    }

    GreedyScheduler<Machine_Model::Identical, Release_Model::Real_Time_Arrival> sched;
    sched.initialize(num_of_machines, machines);

    std::vector<NormalJob> jobs_for_this_turn;
    for(int i = 0; i < num_of_machines; i++)
    {
        jobs_for_this_turn.emplace_back(0, 4, i);
    }
    std::vector<ScheduleStep> schedule_steps1 = sched.schedule(jobs_for_this_turn, machines, 0);

    EXPECT_EQ(schedule_steps1.size(), 2);

    NANO_LOG(DEBUG, "[GreedySchedulerTest::BasicOperations], printing the schedule steps:");
    for(int i = 0; i < schedule_steps1.size(); i++)
    {
        NANO_LOG(DEBUG, "schedule step [%d] : %s", i, schedule_steps1[i].toString().c_str());
    }

    EXPECT_EQ(machines[0].remaining_time_, 4);
    EXPECT_EQ(machines[1].remaining_time_, 4);

    sched.updateMachineState(machines, 1);

    EXPECT_EQ(machines[0].remaining_time_, 3);
    EXPECT_EQ(machines[1].remaining_time_, 3);

    jobs_for_this_turn.clear();
    jobs_for_this_turn.emplace_back(1, 8, num_of_machines);
    std::vector<ScheduleStep> schedule_steps2 = sched.schedule(jobs_for_this_turn, machines, 1);

    auto & job_heap1 = sched.getAccumulatedJobs();
    EXPECT_EQ(job_heap1.size(), 1);

    EXPECT_EQ(schedule_steps2.size(), 0);

    sched.updateMachineState(machines, 3);
    EXPECT_EQ(machines[0].remaining_time_, Invalid_Remaining_Time);
    EXPECT_EQ(machines[1].remaining_time_, Invalid_Remaining_Time);    

    auto & job_heap2 = sched.getAccumulatedJobs();
    EXPECT_EQ(job_heap2.size(), 1);

    auto & machine_free_list = sched.getMachineFreeList();
    EXPECT_EQ(machine_free_list.size(), 2);

    jobs_for_this_turn.clear();
    std::vector<ScheduleStep> schedule_steps3 = sched.schedule(jobs_for_this_turn, machines, 4);
    EXPECT_EQ(schedule_steps3.size(), 1);

    NANO_LOG(DEBUG, "[GreedySchedulerTest::BasicOperations], printing the schedule steps:");
    for(int i = 0; i < schedule_steps1.size(); i++)
    {
        NANO_LOG(DEBUG, "schedule step [%d] : %s", i, schedule_steps3[i].toString().c_str());
    }

}