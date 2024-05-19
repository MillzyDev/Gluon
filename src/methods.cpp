#include "methods.hpp"

#include "gc.hpp"
#include "il2cpp-object-internals.h"

namespace Gluon::Methods {
    Il2CppObject *createManual(const Il2CppClass *klass) noexcept {
        if (!klass) {
            // cannot create manual on null class
            return nullptr;
        }

        if (!klass->initialized) {
            // cannot create an object from uninitalised klass
            return nullptr;
        }

        auto *object = reinterpret_cast<Il2CppObject *>(Gluon::Gc::gcAllocSpecific(klass->instance_size));
        if (!object) {
            // failed to allocate
            return nullptr;
        }

        object->klass = const_cast<Il2CppClass *>(klass);

        if (klass->has_cctor && !klass->cctor_finished_or_no_cctor && !klass->cctor_started) {
            object->klass->cctor_started = true;
            // TODO: Find and run cctor
            object->klass->cctor_finished_or_no_cctor = true;
        }

        return object;
    }

    Il2CppObject *createManualThrow(const Il2CppClass *klass) {

    }
}