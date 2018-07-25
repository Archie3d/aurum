#ifndef AURUM_VST_UID_H
#define AURUM_VST_UID_H

#include "pluginterfaces/base/funknown.h"

namespace au {
namespace vst {

/**
 * @brief Helper to construct and reference UIDs as types.
 */
template<Steinberg::uint32 l1,
         Steinberg::uint32 l2,
         Steinberg::uint32 l3,
         Steinberg::uint32 l4>
struct UID
{
    /**
     * @brief Returns UID object instance.
     * @return
     */
    static inline const Steinberg::FUID& value()
    {
        static Steinberg::FUID fuid(l1, l2, l3, l4);
        return fuid;
    }
};

} // namespace vst
} // namespace au

#endif // AURUM_VST_UID_H
