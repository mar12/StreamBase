/**
 * An example class to be registered by the server and used by clients
 *
 * This is a timer that increases counter value at each call of `tick` method
 * if the timer is started. In the opposite case, `tick` has no effect.
 * The timer can be started and stopped by calling `start` and `stop` methods.
 * Also, two internal integer variables (attributes), `enabled` and `count`,
 * can be readen from or written to without any limitations.
 *
 */
#ifndef TESTS_SAMPLECLASSTIMER_H_
#define TESTS_SAMPLECLASSTIMER_H_

namespace Tests {

class SampleClassTimer {
public:
    void start() {
        m_enabled = 1;
    }

    void stop() {
        m_enabled = 0;
    }

    void reset() {
        m_count = 0;
    }

    void tick() {
        if (m_enabled) {
            ++m_count;
        }
    }

    int m_count = 0;
    int m_enabled = 0;

};

}

#endif // TESTS_SAMPLECLASSTIMER_H_