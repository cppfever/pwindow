#include "all-tests.h"


int main(int argc, char *argv[])
{    
    std::cout << "Running vgui tests." << std::endl;

    //testing::GTEST_FLAG(repeat) = 100;
    testing::GTEST_FLAG(color) = "yes";
    testing::GTEST_FLAG(print_time) = 0;

    //Test filter
    testing::GTEST_FLAG(filter) = "unique_vector_test.insertTest:"
                                  "unique_vector_test.eraseTest:"
                                  "unique_vector_test.indexTest:"
                                  "unique_vector_test.TryZeroIndex:"
                                  "unique_vector_test.NothingShouldChange:"
                                  "unique_vector_test.LookItsMoving:"
                                  "Exception_test.ExceptionInfoMacro:";

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
