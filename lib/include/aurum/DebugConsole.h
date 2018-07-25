#ifndef AURUM_DEBUG_CONSOLE_H
#define AURUM_DEBUG_CONSOLE_H

#include <memory>

namespace au {

/**
 * @brief This is a debug console.
 *
 * The console can be used to display debug messages using
 * usual means (e.g. printf or std::cout).
 */
class DebugConsole
{
public:

    DebugConsole();
    ~DebugConsole();

    void writeLine(const char *str);

private:
    struct Impl;
    std::unique_ptr<Impl> d;
};

} // namespace au

#endif // AURUM_DEBUG_CONSOLE_H
