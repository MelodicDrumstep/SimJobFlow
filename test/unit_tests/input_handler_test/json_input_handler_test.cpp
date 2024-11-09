#include <gtest/gtest.h>
#include <fstream>
#include <nlohmann/json.hpp>

#include "json_input_handler.hpp"
#include "model.hpp"

using namespace SJF;

// #define DEBUG_JSON_INPUT_TEST

TEST(JsonInputHandlerTest, ConstructorAndGetJobs1) 
{
    JsonInputHandler<Model::Identical> handler("../../../../assets/json/job/normal_job1.json");

    // Test assert method
    EXPECT_TRUE(handler.checkValidity(2));
    EXPECT_THROW(handler.checkValidity(3), NumberOfMachinesMismatch);

    // Test getJobs method
    auto jobs = handler.getJobs(0);
    ASSERT_TRUE(jobs.has_value());
    EXPECT_EQ(jobs->size(), 1);
    EXPECT_EQ(jobs->at(0).timestamp_, 0);
    EXPECT_EQ(jobs->at(0).workload_, 1);

    jobs = handler.getJobs(1);
    ASSERT_TRUE(jobs.has_value());
    EXPECT_EQ(jobs->size(), 1);
    EXPECT_EQ(jobs->at(0).timestamp_, 1);
    EXPECT_EQ(jobs->at(0).workload_, 1);

    // Test done method
    EXPECT_TRUE(handler.done());
}

TEST(JsonInputHandlerTest, ConstructorAndGetJobs2) 
{
    JsonInputHandler<Model::Related> handler("../../../../assets/json/job/normal_job2.json");

    #ifdef DEBUG_JSON_INPUT_TEST
        auto job_array = handler.getJobArray();
        for(auto & job : job_array)
        {
            std::cout << job.toString() << std::endl;
        }
    #endif

    // Test assert method
    EXPECT_TRUE(handler.checkValidity(3));

    // Test getJobs method
    auto jobs = handler.getJobs(0);
    ASSERT_TRUE(jobs.has_value());
    EXPECT_EQ(jobs->size(), 1);
    EXPECT_EQ(jobs->at(0).timestamp_, 0);
    EXPECT_EQ(jobs->at(0).workload_, 1);
    EXPECT_FALSE(handler.done());

    jobs = handler.getJobs(1);
    ASSERT_TRUE(jobs.has_value());
    EXPECT_EQ(jobs->size(), 1);
    EXPECT_EQ(jobs->at(0).timestamp_, 1);
    EXPECT_EQ(jobs->at(0).workload_, 1);
    EXPECT_FALSE(handler.done());

    jobs = handler.getJobs(3);
    ASSERT_TRUE(jobs.has_value());
    EXPECT_EQ(jobs->size(), 3);
    EXPECT_EQ(jobs->at(0).timestamp_, 2);
    EXPECT_EQ(jobs->at(0).workload_, 3);
    EXPECT_EQ(jobs->at(1).timestamp_, 2);
    EXPECT_EQ(jobs->at(1).workload_, 5);
    EXPECT_EQ(jobs->at(2).timestamp_, 3);
    EXPECT_EQ(jobs->at(2).workload_, 3);

    // Test done method
    EXPECT_TRUE(handler.done());
}

TEST(JsonInputHandlerTest, UnrelatedConstructorAndGetJobs1) 
{
    JsonInputHandler<Model::Unrelated> handler("../../../../assets/json/job/unrelated_job1.json");

    #ifdef DEBUG_JSON_INPUT_TEST
        auto job_array = handler.getJobArray();
        for(auto & job : job_array)
        {
            std::cout << job.toString() << std::endl;
        }
    #endif

    // Test assert method
    EXPECT_TRUE(handler.checkValidity(2));

    // Test getJobs method
    auto jobs = handler.getJobs(0);
    ASSERT_TRUE(jobs.has_value());
    EXPECT_EQ(jobs->size(), 1);
    EXPECT_EQ(jobs->at(0).timestamp_, 0);
    EXPECT_EQ(jobs->at(0).processing_time_.size(), 2);
    EXPECT_EQ(jobs->at(0).processing_time_.at(0), 3);
    EXPECT_EQ(jobs->at(0).processing_time_.at(1), 2);
    EXPECT_FALSE(handler.done());

    jobs = handler.getJobs(3);
    ASSERT_TRUE(jobs.has_value());
    EXPECT_EQ(jobs->size(), 2);
    EXPECT_EQ(jobs->at(0).timestamp_, 1);
    EXPECT_EQ(jobs->at(0).processing_time_.size(), 2);
    EXPECT_EQ(jobs->at(0).processing_time_.at(0), 5);
    EXPECT_EQ(jobs->at(0).processing_time_.at(1), 4);

    EXPECT_EQ(jobs->at(1).timestamp_, 2);
    EXPECT_EQ(jobs->at(1).processing_time_.size(), 2);
    EXPECT_EQ(jobs->at(1).processing_time_.at(0), 1);
    EXPECT_EQ(jobs->at(1).processing_time_.at(1), 2);

    // Test done method
    EXPECT_TRUE(handler.done());
}
