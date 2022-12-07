#include "doctest.h"
#include "../source/core/Timer.h"

TEST_CASE("Timer tests") {
    Timer timer {};
    SUBCASE("Timer should start with value being 0") {
        CHECK(timer.getValue() == 0);
    }

    SUBCASE("Decreasing timer that is already at 0 should work correctly") {
        timer.tick();
        CHECK(timer.getValue() == 0);
    }

    SUBCASE("Setting timer value should work") {
        timer.setValue(3);
        CHECK(timer.getValue() == 3);
    }

    SUBCASE("Ticking the timer should work") {
        timer.setValue(3);
        timer.tick();
        CHECK(timer.getValue() == 2);
        timer.tick();
        CHECK(timer.getValue() == 1);
        timer.tick();
        CHECK(timer.getValue() == 0);
        timer.tick();
        CHECK(timer.getValue() == 0);
    }

    SUBCASE("isOn() should work correctly") {
        timer.setValue(2);
        CHECK(timer.isOn() == true);
        timer.tick();
        CHECK(timer.isOn() == true);
        timer.tick();
        CHECK(timer.isOn() == false);
    }
}

