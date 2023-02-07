// ----------------------------------------------------------------------
// Main.cpp
// ----------------------------------------------------------------------

#include "Tester.hpp"

TEST(Test, imgstat) {
    Neasc::Tester tester;
    tester.imgstat();
}

TEST(Test, l1) {
    Neasc::Tester tester;
    tester.l1();
}

TEST(Test, coadd) {
    Neasc::Tester tester;
    tester.coadd();
}

TEST(Test, detect) {
    Neasc::Tester tester;
    tester.detect();
}

TEST(Test, crop) {
    Neasc::Tester tester;
    tester.crop();
}

TEST(Test, mask) {
    Neasc::Tester tester;
    tester.mask();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
