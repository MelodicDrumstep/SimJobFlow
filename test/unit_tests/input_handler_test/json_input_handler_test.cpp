#include <gtest/gtest.h>
#include <fstream>
#include <nlohmann/json.hpp>

#include "json_input_handler.hpp"


TEST(JsonInputHandlerTest, ConstructorAndGetJobs) 
{
    JsonInputHandler<NormalJob, 2> handler("../../../../assets/json/job/sample_job.json");

    // Test assert method
    EXPECT_TRUE(handler.checkValidity("Normal"));

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

TEST(JsonInputHandlerTest, InvalidJobType) 
{
    JsonInputHandler<NormalJob, 2> handler("../../../../assets/json/job/sample_job.json");

    // Test assert method with invalid job type
    EXPECT_THROW(handler.checkValidity("InvalidType"), std::runtime_error);
}