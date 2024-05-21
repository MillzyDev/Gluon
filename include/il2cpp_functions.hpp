#ifndef GLUON_IL2CPP_FUNCTIONS_HPP_
#define GLUON_IL2CPP_FUNCTIONS_HPP_

#include <type_traits>

#include "gluon_config.hpp"
#include "il2cpp-api-types.h"
#include "il2cpp-metadata.h"
#include "il2cpp-class-internals.h"

#define IL2CPP_FUNC(rt, name, ...)              \
static rt (*il2cpp_##name)__VA_ARGS__;          \
template<class... TArgs>                        \
static rt name(TArgs &&...args) {               \
    if (!il2cpp_##name) {                       \
        /* TODO: abort */                       \
    }                                           \
    if constexpr (std::is_same_v<rt, void>) {   \
        il2cpp_##name(args...);                 \
    }                                           \
    else {                                      \
        return il2cpp_##name(args...);          \
    }                                           \
}

namespace Gluon {
    class GLUON_API Il2CppFunctions {
    public:
#pragma region IL2CPP Exports
        IL2CPP_FUNC(int, init, (const char* domain_name));
        IL2CPP_FUNC(int, init_utf16, (const Il2CppChar * domain_name));
        IL2CPP_FUNC(void, shutdown, ());
        IL2CPP_FUNC(void, set_config_dir, (const char *config_path));
        IL2CPP_FUNC(void, set_data_dir, (const char *data_path));
        IL2CPP_FUNC(void, set_temp_dir, (const char *temp_path));
        IL2CPP_FUNC(void, set_commandline_arguments, (int argc, const char* const argv[], const char* basedir));
        IL2CPP_FUNC(void, set_commandline_arguments_utf16, (int argc, const Il2CppChar * const argv[], const char* basedir));
        IL2CPP_FUNC(void, set_config_utf16, (const Il2CppChar * executablePath));
        IL2CPP_FUNC(void, set_config, (const char* executablePath));
        IL2CPP_FUNC(void, set_memory_callbacks, (Il2CppMemoryCallbacks * callbacks));
        IL2CPP_FUNC(const Il2CppImage*, get_corlib, ());
        IL2CPP_FUNC(void, add_internal_call, (const char* name, Il2CppMethodPointer method));
        IL2CPP_FUNC(Il2CppMethodPointer, resolve_icall, (const char* name));
        IL2CPP_FUNC(void*, alloc, (size_t size));
        IL2CPP_FUNC(void, free, (void* ptr));
        IL2CPP_FUNC(Il2CppClass*, array_class_get, (Il2CppClass * element_class, uint32_t rank));
        IL2CPP_FUNC(uint32_t, array_length, (Il2CppArray * array));
        IL2CPP_FUNC(uint32_t, array_get_byte_length, (Il2CppArray * array));
        IL2CPP_FUNC(Il2CppArray*, array_new, (Il2CppClass * elementTypeInfo, il2cpp_array_size_t length));
        IL2CPP_FUNC(Il2CppArray*, array_new_specific, (Il2CppClass * arrayTypeInfo, il2cpp_array_size_t length));
        IL2CPP_FUNC(Il2CppArray*, array_new_full, (Il2CppClass * array_class, il2cpp_array_size_t * lengths, il2cpp_array_size_t * lower_bounds));
        IL2CPP_FUNC(Il2CppClass*, bounded_array_class_get, (Il2CppClass * element_class, uint32_t rank, bool bounded));
        IL2CPP_FUNC(int, array_element_size, (const Il2CppClass * array_class));
        IL2CPP_FUNC(const Il2CppImage*, assembly_get_image, (const Il2CppAssembly * assembly));
        IL2CPP_FUNC(void, class_for_each, (void(*klassReportFunc)(Il2CppClass* klass, void* userData), void* userData));
        IL2CPP_FUNC(const Il2CppType*, class_enum_basetype, (Il2CppClass * klass));
        IL2CPP_FUNC(bool, class_is_generic, (const Il2CppClass * klass));
        IL2CPP_FUNC(bool, class_is_inflated, (const Il2CppClass * klass));
        IL2CPP_FUNC(bool, class_is_assignable_from, (Il2CppClass * klass, Il2CppClass * oklass));
        IL2CPP_FUNC(bool, class_is_subclass_of, (Il2CppClass * klass, Il2CppClass * klassc, bool check_interfaces));
        IL2CPP_FUNC(bool, class_has_parent, (Il2CppClass * klass, Il2CppClass * klassc));
        IL2CPP_FUNC(Il2CppClass*, class_from_il2cpp_type, (const Il2CppType * type));
        IL2CPP_FUNC(Il2CppClass*, class_from_name, (const Il2CppImage * image, const char* namespaze, const char *name));
        IL2CPP_FUNC(Il2CppClass*, class_from_system_type, (Il2CppReflectionType * type));
        IL2CPP_FUNC(Il2CppClass*, class_get_element_class, (Il2CppClass * klass));
        IL2CPP_FUNC(const EventInfo*, class_get_events, (Il2CppClass * klass, void* *iter));
        IL2CPP_FUNC(FieldInfo*, class_get_fields, (Il2CppClass * klass, void* *iter));
        IL2CPP_FUNC(Il2CppClass*, class_get_nested_types, (Il2CppClass * klass, void* *iter));
        IL2CPP_FUNC(Il2CppClass*, class_get_interfaces, (Il2CppClass * klass, void* *iter));
        IL2CPP_FUNC(const PropertyInfo*, class_get_properties, (Il2CppClass * klass, void* *iter));
        IL2CPP_FUNC(const PropertyInfo*, class_get_property_from_name, (Il2CppClass * klass, const char *name));
        IL2CPP_FUNC(FieldInfo*, class_get_field_from_name, (Il2CppClass * klass, const char *name));
        IL2CPP_FUNC(const MethodInfo*, class_get_methods, (Il2CppClass * klass, void* *iter));
        IL2CPP_FUNC(const MethodInfo*, class_get_method_from_name, (const Il2CppClass * klass, const char* name, int argsCount));
        IL2CPP_FUNC(const char*, class_get_name, (const Il2CppClass * klass));
        IL2CPP_FUNC(void, type_get_name_chunked, (const Il2CppType * type, void(*chunkReportFunc)(void* data, void* userData), void* userData));
        IL2CPP_FUNC(const char*, class_get_namespace, (const Il2CppClass * klass));
        IL2CPP_FUNC(Il2CppClass*, class_get_parent, (Il2CppClass * klass));
        IL2CPP_FUNC(Il2CppClass*, class_get_declaring_type, (const Il2CppClass * klass));
        IL2CPP_FUNC(int32_t, class_instance_size, (Il2CppClass * klass));
        IL2CPP_FUNC(size_t, class_num_fields, (const Il2CppClass * enumKlass));
        IL2CPP_FUNC(bool, class_is_valuetype, (const Il2CppClass * klass));
        IL2CPP_FUNC(int32_t, class_value_size, (Il2CppClass * klass, uint32_t * align));
        IL2CPP_FUNC(bool, class_is_blittable, (const Il2CppClass * klass));
        IL2CPP_FUNC(int, class_get_flags, (const Il2CppClass * klass));
        IL2CPP_FUNC(bool, class_is_abstract, (const Il2CppClass * klass));
        IL2CPP_FUNC(bool, class_is_interface, (const Il2CppClass * klass));
        IL2CPP_FUNC(int, class_array_element_size, (const Il2CppClass * klass));
        IL2CPP_FUNC(Il2CppClass*, class_from_type, (const Il2CppType * type));
        IL2CPP_FUNC(const Il2CppType*, class_get_type, (Il2CppClass * klass));
        IL2CPP_FUNC(uint32_t, class_get_type_token, (Il2CppClass * klass));
        IL2CPP_FUNC(bool, class_has_attribute, (Il2CppClass * klass, Il2CppClass * attr_class));
        IL2CPP_FUNC(bool, class_has_references, (Il2CppClass * klass));
        IL2CPP_FUNC(bool, class_is_enum, (const Il2CppClass * klass));
        IL2CPP_FUNC(const Il2CppImage*, class_get_image, (Il2CppClass * klass));
        IL2CPP_FUNC(const char*, class_get_assemblyname, (const Il2CppClass * klass));
        IL2CPP_FUNC(int, class_get_rank, (const Il2CppClass * klass));
        IL2CPP_FUNC(uint32_t, class_get_data_size, (const Il2CppClass * klass));
        IL2CPP_FUNC(void*, class_get_static_field_data, (const Il2CppClass * klass));
        IL2CPP_FUNC(size_t, class_get_bitmap_size, (const Il2CppClass * klass));
        IL2CPP_FUNC(void, class_get_bitmap, (Il2CppClass * klass, size_t * bitmap));
        IL2CPP_FUNC(bool, stats_dump_to_file, (const char *path));
        IL2CPP_FUNC(uint64_t, stats_get_value, (Il2CppStat stat));
        IL2CPP_FUNC(Il2CppDomain*, domain_get, ());
        IL2CPP_FUNC(const Il2CppAssembly*, domain_assembly_open, (Il2CppDomain * domain, const char* name));
        IL2CPP_FUNC(const Il2CppAssembly**, domain_get_assemblies, (const Il2CppDomain * domain, size_t * size));
        IL2CPP_FUNC(void, raise_exception, (Il2CppException*));
        IL2CPP_FUNC(Il2CppException*, exception_from_name_msg, (const Il2CppImage * image, const char *name_space, const char *name, const char *msg));
        IL2CPP_FUNC(Il2CppException*, get_exception_argument_null, (const char *arg));
        IL2CPP_FUNC(void, format_exception, (const Il2CppException * ex, char* message, int message_size));
        IL2CPP_FUNC(void, format_stack_trace, (const Il2CppException * ex, char* output, int output_size));
        IL2CPP_FUNC(void, unhandled_exception, (Il2CppException*));
        IL2CPP_FUNC(void, native_stack_trace, (const Il2CppException *ex, uintptr_t **addresses, int *numFrames, char **imageUUID, char **imageName));
        IL2CPP_FUNC(int, field_get_flags, (FieldInfo * field));
        IL2CPP_FUNC(const char*, field_get_name, (FieldInfo * field));
        IL2CPP_FUNC(Il2CppClass*, field_get_parent, (FieldInfo * field));
        IL2CPP_FUNC(size_t, field_get_offset, (FieldInfo * field));
        IL2CPP_FUNC(const Il2CppType*, field_get_type, (FieldInfo * field));
        IL2CPP_FUNC(void, field_get_value, (Il2CppObject * obj, FieldInfo * field, void *value));
        IL2CPP_FUNC(Il2CppObject*, field_get_value_object, (FieldInfo * field, Il2CppObject * obj));
        IL2CPP_FUNC(bool, field_has_attribute, (FieldInfo * field, Il2CppClass * attr_class));
        IL2CPP_FUNC(void, field_set_value, (Il2CppObject * obj, FieldInfo * field, void *value));
        IL2CPP_FUNC(void, field_static_get_value, (FieldInfo * field, void *value));
        IL2CPP_FUNC(void, field_static_set_value, (FieldInfo * field, void *value));
        IL2CPP_FUNC(void, field_set_value_object, (Il2CppObject * instance, FieldInfo * field, Il2CppObject * value));
        IL2CPP_FUNC(bool, field_is_literal, (FieldInfo * field));
        IL2CPP_FUNC(void, gc_collect, (int maxGenerations));
        IL2CPP_FUNC(int32_t, gc_collect_a_little, ());
        IL2CPP_FUNC(void, gc_disable, ());
        IL2CPP_FUNC(void, gc_enable, ());
        IL2CPP_FUNC(bool, gc_is_disabled, ());
        IL2CPP_FUNC(int64_t, gc_get_max_time_slice_ns, ());
        IL2CPP_FUNC(void, gc_set_max_time_slice_ns, (int64_t maxTimeSlice));
        IL2CPP_FUNC(bool, gc_is_incremental, ());
        IL2CPP_FUNC(int64_t, gc_get_used_size, ());
        IL2CPP_FUNC(int64_t, gc_get_heap_size, ());
        IL2CPP_FUNC(void, gc_wbarrier_set_field, (Il2CppObject * obj, void **targetAddress, void *object));
        IL2CPP_FUNC(bool, gc_has_strict_wbarriers, ());
        IL2CPP_FUNC(void, gc_set_external_allocation_tracker, (void(*func)(void*, size_t, int)));
        IL2CPP_FUNC(void, gc_set_external_wbarrier_tracker, (void(*func)(void**)));
        IL2CPP_FUNC(void, gc_foreach_heap, (void(*func)(void* data, void* userData), void* userData));
        IL2CPP_FUNC(void, stop_gc_world, ());
        IL2CPP_FUNC(void, start_gc_world, ());
        IL2CPP_FUNC(void*, gc_alloc_fixed, (std::size_t size));
        IL2CPP_FUNC(void, gc_free_fixed, (void* address));
        IL2CPP_FUNC(uint32_t, gchandle_new, (Il2CppObject * obj, bool pinned));
        IL2CPP_FUNC(uint32_t, gchandle_new_weakref, (Il2CppObject * obj, bool track_resurrection));
        IL2CPP_FUNC(Il2CppObject*, gchandle_get_target, (uint32_t gchandle));
        IL2CPP_FUNC(void, gchandle_free, (uint32_t gchandle));
        IL2CPP_FUNC(void, gchandle_foreach_get_target, (void(*func)(void* data, void* userData), void* userData));
        IL2CPP_FUNC(uint32_t, object_header_size, ());
        IL2CPP_FUNC(uint32_t, array_object_header_size, ());
        IL2CPP_FUNC(uint32_t, offset_of_array_length_in_array_object_header, ());
        IL2CPP_FUNC(uint32_t, offset_of_array_bounds_in_array_object_header, ());
        IL2CPP_FUNC(uint32_t, allocation_granularity, ());
        IL2CPP_FUNC(void*, unity_liveness_allocate_struct, (Il2CppClass * filter, int max_object_count, il2cpp_register_object_callback callback, void* userdata, il2cpp_liveness_reallocate_callback reallocate));
        IL2CPP_FUNC(void, unity_liveness_calculation_from_root, (Il2CppObject * root, void* state));
        IL2CPP_FUNC(void, unity_liveness_calculation_from_statics, (void* state));
        IL2CPP_FUNC(void, unity_liveness_finalize, (void* state));
        IL2CPP_FUNC(void, unity_liveness_free_struct, (void* state));
        IL2CPP_FUNC(const Il2CppType*, method_get_return_type, (const MethodInfo * method));
        IL2CPP_FUNC(Il2CppClass*, method_get_declaring_type, (const MethodInfo * method));
        IL2CPP_FUNC(const char*, method_get_name, (const MethodInfo * method));
        IL2CPP_FUNC(const MethodInfo*, method_get_from_reflection, (const Il2CppReflectionMethod * method));
        IL2CPP_FUNC(Il2CppReflectionMethod*, method_get_object, (const MethodInfo * method, Il2CppClass * refclass));
        IL2CPP_FUNC(bool, method_is_generic, (const MethodInfo * method));
        IL2CPP_FUNC(bool, method_is_inflated, (const MethodInfo * method));
        IL2CPP_FUNC(bool, method_is_instance, (const MethodInfo * method));
        IL2CPP_FUNC(uint32_t, method_get_param_count, (const MethodInfo * method));
        IL2CPP_FUNC(const Il2CppType*, method_get_param, (const MethodInfo * method, uint32_t index));
        IL2CPP_FUNC(Il2CppClass*, method_get_class, (const MethodInfo * method));
        IL2CPP_FUNC(bool, method_has_attribute, (const MethodInfo * method, Il2CppClass * attr_class));
        IL2CPP_FUNC(uint32_t, method_get_flags, (const MethodInfo * method, uint32_t * iflags));
        IL2CPP_FUNC(uint32_t, method_get_token, (const MethodInfo * method));
        IL2CPP_FUNC(const char*, method_get_param_name, (const MethodInfo * method, uint32_t index));
        IL2CPP_FUNC(void, profiler_install, (Il2CppProfiler * prof, Il2CppProfileFunc shutdown_callback));
        IL2CPP_FUNC(void, profiler_set_events, (Il2CppProfileFlags events));
        IL2CPP_FUNC(void, profiler_install_enter_leave, (Il2CppProfileMethodFunc enter, Il2CppProfileMethodFunc fleave));
        IL2CPP_FUNC(void, profiler_install_allocation, (Il2CppProfileAllocFunc callback));
        IL2CPP_FUNC(void, profiler_install_gc, (Il2CppProfileGCFunc callback, Il2CppProfileGCResizeFunc heap_resize_callback));
        IL2CPP_FUNC(void, profiler_install_fileio, (Il2CppProfileFileIOFunc callback));
        IL2CPP_FUNC(void, profiler_install_thread, (Il2CppProfileThreadFunc start, Il2CppProfileThreadFunc end));
        IL2CPP_FUNC(uint32_t, property_get_flags, (const PropertyInfo * prop));
        IL2CPP_FUNC(const MethodInfo*, property_get_get_method, (const PropertyInfo * prop));
        IL2CPP_FUNC(const MethodInfo*, property_get_set_method, (const PropertyInfo * prop));
        IL2CPP_FUNC(const char*, property_get_name, (const PropertyInfo * prop));
        IL2CPP_FUNC(Il2CppClass*, property_get_parent, (const PropertyInfo * prop));
        IL2CPP_FUNC(Il2CppClass*, object_get_class, (Il2CppObject * obj));
        IL2CPP_FUNC(uint32_t, object_get_size, (Il2CppObject * obj));
        IL2CPP_FUNC(const MethodInfo*, object_get_virtual_method, (Il2CppObject * obj, const MethodInfo * method));
        IL2CPP_FUNC(Il2CppObject*, object_new, (const Il2CppClass * klass));
        IL2CPP_FUNC(void*, object_unbox, (Il2CppObject * obj));
        IL2CPP_FUNC(Il2CppObject*, value_box, (Il2CppClass * klass, void* data));
        IL2CPP_FUNC(void, monitor_enter, (Il2CppObject * obj));
        IL2CPP_FUNC(bool, monitor_try_enter, (Il2CppObject * obj, uint32_t timeout));
        IL2CPP_FUNC(void, monitor_exit, (Il2CppObject * obj));
        IL2CPP_FUNC(void, monitor_pulse, (Il2CppObject * obj));
        IL2CPP_FUNC(void, monitor_pulse_all, (Il2CppObject * obj));
        IL2CPP_FUNC(void, monitor_wait, (Il2CppObject * obj));
        IL2CPP_FUNC(bool, monitor_try_wait, (Il2CppObject * obj, uint32_t timeout));
        IL2CPP_FUNC(Il2CppObject*, runtime_invoke, (const MethodInfo * method, void *obj, void **params, Il2CppException **exc));
        IL2CPP_FUNC(Il2CppObject*, runtime_invoke_convert_args, (const MethodInfo * method, void *obj, Il2CppObject **params, int paramCount, Il2CppException **exc));
        IL2CPP_FUNC(void, runtime_class_init, (Il2CppClass * klass));
        IL2CPP_FUNC(void, runtime_object_init, (Il2CppObject * obj));
        IL2CPP_FUNC(void, runtime_object_init_exception, (Il2CppObject * obj, Il2CppException** exc));
        IL2CPP_FUNC(void, runtime_unhandled_exception_policy_set, (Il2CppRuntimeUnhandledExceptionPolicy value));
        IL2CPP_FUNC(int32_t, string_length, (Il2CppString * str));
        IL2CPP_FUNC(Il2CppChar*, string_chars, (Il2CppString * str));
        IL2CPP_FUNC(Il2CppString*, string_new, (const char* str));
        IL2CPP_FUNC(Il2CppString*, string_new_len, (const char* str, uint32_t length));
        IL2CPP_FUNC(Il2CppString*, string_new_utf16, (const Il2CppChar * text, int32_t len));
        IL2CPP_FUNC(Il2CppString*, string_new_wrapper, (const char* str));
        IL2CPP_FUNC(Il2CppString*, string_intern, (Il2CppString * str));
        IL2CPP_FUNC(Il2CppString*, string_is_interned, (Il2CppString * str));
        IL2CPP_FUNC(Il2CppThread*, thread_current, ());
        IL2CPP_FUNC(Il2CppThread*, thread_attach, (Il2CppDomain * domain));
        IL2CPP_FUNC(void, thread_detach, (Il2CppThread * thread));
        IL2CPP_FUNC(Il2CppThread**, thread_get_all_attached_threads, (size_t * size));
        IL2CPP_FUNC(bool, is_vm_thread, (Il2CppThread * thread));
        IL2CPP_FUNC(void, current_thread_walk_frame_stack, (Il2CppFrameWalkFunc func, void* user_data));
        IL2CPP_FUNC(void, thread_walk_frame_stack, (Il2CppThread * thread, Il2CppFrameWalkFunc func, void* user_data));
        IL2CPP_FUNC(bool, current_thread_get_top_frame, (Il2CppStackFrameInfo * frame));
        IL2CPP_FUNC(bool, thread_get_top_frame, (Il2CppThread * thread, Il2CppStackFrameInfo * frame));
        IL2CPP_FUNC(bool, current_thread_get_frame_at, (int32_t offset, Il2CppStackFrameInfo * frame));
        IL2CPP_FUNC(bool, thread_get_frame_at, (Il2CppThread * thread, int32_t offset, Il2CppStackFrameInfo * frame));
        IL2CPP_FUNC(int32_t, current_thread_get_stack_depth, ());
        IL2CPP_FUNC(int32_t, thread_get_stack_depth, (Il2CppThread * thread));
        IL2CPP_FUNC(void, override_stack_backtrace, (Il2CppBacktraceFunc stackBacktraceFunc));
        IL2CPP_FUNC(Il2CppObject*, type_get_object, (const Il2CppType * type));
        IL2CPP_FUNC(int, type_get_type, (const Il2CppType * type));
        IL2CPP_FUNC(Il2CppClass*, type_get_class_or_element_class, (const Il2CppType * type));
        IL2CPP_FUNC(char*, type_get_name, (const Il2CppType * type));
        IL2CPP_FUNC(bool, type_is_byref, (const Il2CppType * type));
        IL2CPP_FUNC(uint32_t, type_get_attrs, (const Il2CppType * type));
        IL2CPP_FUNC(bool, type_equals, (const Il2CppType * type, const Il2CppType * otherType));
        IL2CPP_FUNC(char*, type_get_assembly_qualified_name, (const Il2CppType * type));
        IL2CPP_FUNC(bool, type_is_static, (const Il2CppType * type));
        IL2CPP_FUNC(bool, type_is_pointer_type, (const Il2CppType * type));
        IL2CPP_FUNC(const Il2CppAssembly*, image_get_assembly, (const Il2CppImage * image));
        IL2CPP_FUNC(const char*, image_get_name, (const Il2CppImage * image));
        IL2CPP_FUNC(const char*, image_get_filename, (const Il2CppImage * image));
        IL2CPP_FUNC(const MethodInfo*, image_get_entry_point, (const Il2CppImage * image));
        IL2CPP_FUNC(size_t, image_get_class_count, (const Il2CppImage * image));
        IL2CPP_FUNC(const Il2CppClass*, image_get_class, (const Il2CppImage * image, size_t index));
        IL2CPP_FUNC(Il2CppManagedMemorySnapshot*, capture_memory_snapshot, ());
        IL2CPP_FUNC(void, free_captured_memory_snapshot, (Il2CppManagedMemorySnapshot * snapshot));
        IL2CPP_FUNC(void, set_find_plugin_callback, (Il2CppSetFindPlugInCallback method));
        IL2CPP_FUNC(void, register_log_callback, (Il2CppLogCallback method));
        IL2CPP_FUNC(void, debugger_set_agent_options, (const char* options));
        IL2CPP_FUNC(bool, is_debugger_attached, ());
        IL2CPP_FUNC(void, register_debugger_agent_transport, (Il2CppDebuggerTransport * debuggerTransport));
        IL2CPP_FUNC(bool, debug_get_method_info, (const MethodInfo*, Il2CppMethodDebugInfo * methodDebugInfo));
        IL2CPP_FUNC(void, unity_install_unitytls_interface, (const void* unitytlsInterfaceStruct));
        IL2CPP_FUNC(Il2CppCustomAttrInfo*, custom_attrs_from_class, (Il2CppClass * klass));
        IL2CPP_FUNC(Il2CppCustomAttrInfo*, custom_attrs_from_method, (const MethodInfo * method));
        IL2CPP_FUNC(Il2CppObject*, custom_attrs_get_attr, (Il2CppCustomAttrInfo * ainfo, Il2CppClass * attr_klass));
        IL2CPP_FUNC(bool, custom_attrs_has_attr, (Il2CppCustomAttrInfo * ainfo, Il2CppClass * attr_klass));
        IL2CPP_FUNC(Il2CppArray*, custom_attrs_construct, (Il2CppCustomAttrInfo * cinfo));
        IL2CPP_FUNC(void, custom_attrs_free, (Il2CppCustomAttrInfo * ainfo));
        IL2CPP_FUNC(void, class_set_userdata, (Il2CppClass * klass, void* userdata));
        IL2CPP_FUNC(int, class_get_userdata_offset, ());
        IL2CPP_FUNC(void, set_default_thread_affinity, (int64_t affinity_mask));
#pragma endregion

        static bool initialised;
        static void initialise();
    };
}

#endif //GLUON_IL2CPP_FUNCTIONS_HPP_
