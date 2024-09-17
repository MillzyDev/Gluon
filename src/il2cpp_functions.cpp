#include <windows.h>
#include <psapi.h>

#include "il2cpp_functions.hpp"

#include "abortion.hpp"
#include "gluon_logging.hpp"
#include "xref_helpers.hpp"

#define IL2CPP_INIT(rt, name, ...) rt(*Gluon::Il2CppFunctions::il2cpp_##name) __VA_ARGS__

#define IL2CPP_LOAD(name) \
*reinterpret_cast<FARPROC *>(&il2cpp_##name) = GetProcAddress(il2cppAssembly, "il2cpp_" #name); \
Gluon::Logging::Logger::info("Loaded: " #name ", error: {}", GetLastError());                                  \
SetLastError(0)

#pragma region IL2CPP Exports
IL2CPP_INIT(int, init, (const char* domain_name));
IL2CPP_INIT(int, init_utf16, (const Il2CppChar * domain_name));
IL2CPP_INIT(void, shutdown, ());
IL2CPP_INIT(void, set_config_dir, (const char *config_path));
IL2CPP_INIT(void, set_data_dir, (const char *data_path));
IL2CPP_INIT(void, set_temp_dir, (const char *temp_path));
IL2CPP_INIT(void, set_commandline_arguments, (int argc, const char* const argv[], const char* basedir));
IL2CPP_INIT(void, set_commandline_arguments_utf16, (int argc, const Il2CppChar * const argv[], const char* basedir));
IL2CPP_INIT(void, set_config_utf16, (const Il2CppChar * executablePath));
IL2CPP_INIT(void, set_config, (const char* executablePath));
IL2CPP_INIT(void, set_memory_callbacks, (Il2CppMemoryCallbacks * callbacks));
IL2CPP_INIT(const Il2CppImage*, get_corlib, ());
IL2CPP_INIT(void, add_internal_call, (const char* name, Il2CppMethodPointer method));
IL2CPP_INIT(Il2CppMethodPointer, resolve_icall, (const char* name));
IL2CPP_INIT(void*, alloc, (size_t size));
IL2CPP_INIT(void, free, (void* ptr));
IL2CPP_INIT(Il2CppClass*, array_class_get, (Il2CppClass * element_class, uint32_t rank));
IL2CPP_INIT(uint32_t, array_length, (Il2CppArray * array));
IL2CPP_INIT(uint32_t, array_get_byte_length, (Il2CppArray * array));
IL2CPP_INIT(Il2CppArray*, array_new, (Il2CppClass * elementTypeInfo, il2cpp_array_size_t length));
IL2CPP_INIT(Il2CppArray*, array_new_specific, (Il2CppClass * arrayTypeInfo, il2cpp_array_size_t length));
IL2CPP_INIT(Il2CppArray*, array_new_full, (Il2CppClass * array_class, il2cpp_array_size_t * lengths, il2cpp_array_size_t * lower_bounds));
IL2CPP_INIT(Il2CppClass*, bounded_array_class_get, (Il2CppClass * element_class, uint32_t rank, bool bounded));
IL2CPP_INIT(int, array_element_size, (const Il2CppClass * array_class));
IL2CPP_INIT(const Il2CppImage*, assembly_get_image, (const Il2CppAssembly * assembly));
IL2CPP_INIT(void, class_for_each, (void(*klassReportFunc)(Il2CppClass* klass, void* userData), void* userData));
IL2CPP_INIT(const Il2CppType*, class_enum_basetype, (Il2CppClass * klass));
IL2CPP_INIT(bool, class_is_generic, (const Il2CppClass * klass));
IL2CPP_INIT(bool, class_is_inflated, (const Il2CppClass * klass));
IL2CPP_INIT(bool, class_is_assignable_from, (Il2CppClass * klass, Il2CppClass * oklass));
IL2CPP_INIT(bool, class_is_subclass_of, (Il2CppClass * klass, Il2CppClass * klassc, bool check_interfaces));
IL2CPP_INIT(bool, class_has_parent, (Il2CppClass * klass, Il2CppClass * klassc));
IL2CPP_INIT(Il2CppClass*, class_from_il2cpp_type, (const Il2CppType * type));
IL2CPP_INIT(Il2CppClass*, class_from_name, (const Il2CppImage * image, const char* namespaze, const char *name));
IL2CPP_INIT(Il2CppClass*, class_from_system_type, (Il2CppReflectionType * type));
IL2CPP_INIT(Il2CppClass*, class_get_element_class, (Il2CppClass * klass));
IL2CPP_INIT(const EventInfo*, class_get_events, (Il2CppClass * klass, void* *iter));
IL2CPP_INIT(FieldInfo*, class_get_fields, (Il2CppClass * klass, void* *iter));
IL2CPP_INIT(Il2CppClass*, class_get_nested_types, (Il2CppClass * klass, void* *iter));
IL2CPP_INIT(Il2CppClass*, class_get_interfaces, (Il2CppClass * klass, void* *iter));
IL2CPP_INIT(const PropertyInfo*, class_get_properties, (Il2CppClass * klass, void* *iter));
IL2CPP_INIT(const PropertyInfo*, class_get_property_from_name, (Il2CppClass * klass, const char *name));
IL2CPP_INIT(FieldInfo*, class_get_field_from_name, (Il2CppClass * klass, const char *name));
IL2CPP_INIT(const MethodInfo*, class_get_methods, (Il2CppClass * klass, void* *iter));
IL2CPP_INIT(const MethodInfo*, class_get_method_from_name, (const Il2CppClass * klass, const char* name, int argsCount));
IL2CPP_INIT(const char*, class_get_name, (const Il2CppClass * klass));
IL2CPP_INIT(void, type_get_name_chunked, (const Il2CppType * type, void(*chunkReportFunc)(void* data, void* userData), void* userData));
IL2CPP_INIT(const char*, class_get_namespace, (const Il2CppClass * klass));
IL2CPP_INIT(Il2CppClass*, class_get_parent, (Il2CppClass * klass));
IL2CPP_INIT(Il2CppClass*, class_get_declaring_type, (const Il2CppClass * klass));
IL2CPP_INIT(int32_t, class_instance_size, (Il2CppClass * klass));
IL2CPP_INIT(size_t, class_num_fields, (const Il2CppClass * enumKlass));
IL2CPP_INIT(bool, class_is_valuetype, (const Il2CppClass * klass));
IL2CPP_INIT(int32_t, class_value_size, (Il2CppClass * klass, uint32_t * align));
IL2CPP_INIT(bool, class_is_blittable, (const Il2CppClass * klass));
IL2CPP_INIT(int, class_get_flags, (const Il2CppClass * klass));
IL2CPP_INIT(bool, class_is_abstract, (const Il2CppClass * klass));
IL2CPP_INIT(bool, class_is_interface, (const Il2CppClass * klass));
IL2CPP_INIT(int, class_array_element_size, (const Il2CppClass * klass));
IL2CPP_INIT(Il2CppClass*, class_from_type, (const Il2CppType * type));
IL2CPP_INIT(const Il2CppType*, class_get_type, (Il2CppClass * klass));
IL2CPP_INIT(uint32_t, class_get_type_token, (Il2CppClass * klass));
IL2CPP_INIT(bool, class_has_attribute, (Il2CppClass * klass, Il2CppClass * attr_class));
IL2CPP_INIT(bool, class_has_references, (Il2CppClass * klass));
IL2CPP_INIT(bool, class_is_enum, (const Il2CppClass * klass));
IL2CPP_INIT(const Il2CppImage*, class_get_image, (Il2CppClass * klass));
IL2CPP_INIT(const char*, class_get_assemblyname, (const Il2CppClass * klass));
IL2CPP_INIT(int, class_get_rank, (const Il2CppClass * klass));
IL2CPP_INIT(uint32_t, class_get_data_size, (const Il2CppClass * klass));
IL2CPP_INIT(void*, class_get_static_field_data, (const Il2CppClass * klass));
IL2CPP_INIT(size_t, class_get_bitmap_size, (const Il2CppClass * klass));
IL2CPP_INIT(void, class_get_bitmap, (Il2CppClass * klass, size_t * bitmap));
IL2CPP_INIT(bool, stats_dump_to_file, (const char *path));
IL2CPP_INIT(uint64_t, stats_get_value, (Il2CppStat stat));
IL2CPP_INIT(Il2CppDomain*, domain_get, ());
IL2CPP_INIT(const Il2CppAssembly*, domain_assembly_open, (Il2CppDomain * domain, const char* name));
IL2CPP_INIT(const Il2CppAssembly**, domain_get_assemblies, (const Il2CppDomain * domain, size_t * size));
IL2CPP_INIT(void, raise_exception, (Il2CppException*));
IL2CPP_INIT(Il2CppException*, exception_from_name_msg, (const Il2CppImage * image, const char *name_space, const char *name, const char *msg));
IL2CPP_INIT(Il2CppException*, get_exception_argument_null, (const char *arg));
IL2CPP_INIT(void, format_exception, (const Il2CppException * ex, char* message, int message_size));
IL2CPP_INIT(void, format_stack_trace, (const Il2CppException * ex, char* output, int output_size));
IL2CPP_INIT(void, unhandled_exception, (Il2CppException*));
IL2CPP_INIT(void, native_stack_trace, (const Il2CppException *ex, uintptr_t **addresses, int *numFrames, char **imageUUID, char **imageName));
IL2CPP_INIT(int, field_get_flags, (FieldInfo * field));
IL2CPP_INIT(const char*, field_get_name, (FieldInfo * field));
IL2CPP_INIT(Il2CppClass*, field_get_parent, (FieldInfo * field));
IL2CPP_INIT(size_t, field_get_offset, (FieldInfo * field));
IL2CPP_INIT(const Il2CppType*, field_get_type, (FieldInfo * field));
IL2CPP_INIT(void, field_get_value, (Il2CppObject * obj, FieldInfo * field, void *value));
IL2CPP_INIT(Il2CppObject*, field_get_value_object, (FieldInfo * field, Il2CppObject * obj));
IL2CPP_INIT(bool, field_has_attribute, (FieldInfo * field, Il2CppClass * attr_class));
IL2CPP_INIT(void, field_set_value, (Il2CppObject * obj, FieldInfo * field, void *value));
IL2CPP_INIT(void, field_static_get_value, (FieldInfo * field, void *value));
IL2CPP_INIT(void, field_static_set_value, (FieldInfo * field, void *value));
IL2CPP_INIT(void, field_set_value_object, (Il2CppObject * instance, FieldInfo * field, Il2CppObject * value));
IL2CPP_INIT(bool, field_is_literal, (FieldInfo * field));
IL2CPP_INIT(void, gc_collect, (int maxGenerations));
IL2CPP_INIT(int32_t, gc_collect_a_little, ());
IL2CPP_INIT(void, gc_disable, ());
IL2CPP_INIT(void, gc_enable, ());
IL2CPP_INIT(bool, gc_is_disabled, ());
IL2CPP_INIT(int64_t, gc_get_max_time_slice_ns, ());
IL2CPP_INIT(void, gc_set_max_time_slice_ns, (int64_t maxTimeSlice));
IL2CPP_INIT(bool, gc_is_incremental, ());
IL2CPP_INIT(int64_t, gc_get_used_size, ());
IL2CPP_INIT(int64_t, gc_get_heap_size, ());
IL2CPP_INIT(void, gc_wbarrier_set_field, (Il2CppObject * obj, void **targetAddress, void *object));
IL2CPP_INIT(bool, gc_has_strict_wbarriers, ());
IL2CPP_INIT(void, gc_set_external_allocation_tracker, (void(*func)(void*, size_t, int)));
IL2CPP_INIT(void, gc_set_external_wbarrier_tracker, (void(*func)(void**)));
IL2CPP_INIT(void, gc_foreach_heap, (void(*func)(void* data, void* userData), void* userData));
IL2CPP_INIT(void, stop_gc_world, ());
IL2CPP_INIT(void, start_gc_world, ());
IL2CPP_INIT(void*, gc_alloc_fixed, (std::size_t size));
IL2CPP_INIT(void, gc_free_fixed, (void* address));
IL2CPP_INIT(uint32_t, gchandle_new, (Il2CppObject * obj, bool pinned));
IL2CPP_INIT(uint32_t, gchandle_new_weakref, (Il2CppObject * obj, bool track_resurrection));
IL2CPP_INIT(Il2CppObject*, gchandle_get_target, (uint32_t gchandle));
IL2CPP_INIT(void, gchandle_free, (uint32_t gchandle));
IL2CPP_INIT(void, gchandle_foreach_get_target, (void(*func)(void* data, void* userData), void* userData));
IL2CPP_INIT(uint32_t, object_header_size, ());
IL2CPP_INIT(uint32_t, array_object_header_size, ());
IL2CPP_INIT(uint32_t, offset_of_array_length_in_array_object_header, ());
IL2CPP_INIT(uint32_t, offset_of_array_bounds_in_array_object_header, ());
IL2CPP_INIT(uint32_t, allocation_granularity, ());
IL2CPP_INIT(void*, unity_liveness_allocate_struct, (Il2CppClass * filter, int max_object_count, il2cpp_register_object_callback callback, void* userdata, il2cpp_liveness_reallocate_callback reallocate));
IL2CPP_INIT(void, unity_liveness_calculation_from_root, (Il2CppObject * root, void* state));
IL2CPP_INIT(void, unity_liveness_calculation_from_statics, (void* state));
IL2CPP_INIT(void, unity_liveness_finalize, (void* state));
IL2CPP_INIT(void, unity_liveness_free_struct, (void* state));
IL2CPP_INIT(const Il2CppType*, method_get_return_type, (const MethodInfo * method));
IL2CPP_INIT(Il2CppClass*, method_get_declaring_type, (const MethodInfo * method));
IL2CPP_INIT(const char*, method_get_name, (const MethodInfo * method));
IL2CPP_INIT(const MethodInfo*, method_get_from_reflection, (const Il2CppReflectionMethod * method));
IL2CPP_INIT(Il2CppReflectionMethod*, method_get_object, (const MethodInfo * method, Il2CppClass * refclass));
IL2CPP_INIT(bool, method_is_generic, (const MethodInfo * method));
IL2CPP_INIT(bool, method_is_inflated, (const MethodInfo * method));
IL2CPP_INIT(bool, method_is_instance, (const MethodInfo * method));
IL2CPP_INIT(uint32_t, method_get_param_count, (const MethodInfo * method));
IL2CPP_INIT(const Il2CppType*, method_get_param, (const MethodInfo * method, uint32_t index));
IL2CPP_INIT(Il2CppClass*, method_get_class, (const MethodInfo * method));
IL2CPP_INIT(bool, method_has_attribute, (const MethodInfo * method, Il2CppClass * attr_class));
IL2CPP_INIT(uint32_t, method_get_flags, (const MethodInfo * method, uint32_t * iflags));
IL2CPP_INIT(uint32_t, method_get_token, (const MethodInfo * method));
IL2CPP_INIT(const char*, method_get_param_name, (const MethodInfo * method, uint32_t index));
IL2CPP_INIT(void, profiler_install, (Il2CppProfiler * prof, Il2CppProfileFunc shutdown_callback));
IL2CPP_INIT(void, profiler_set_events, (Il2CppProfileFlags events));
IL2CPP_INIT(void, profiler_install_enter_leave, (Il2CppProfileMethodFunc enter, Il2CppProfileMethodFunc fleave));
IL2CPP_INIT(void, profiler_install_allocation, (Il2CppProfileAllocFunc callback));
IL2CPP_INIT(void, profiler_install_gc, (Il2CppProfileGCFunc callback, Il2CppProfileGCResizeFunc heap_resize_callback));
IL2CPP_INIT(void, profiler_install_fileio, (Il2CppProfileFileIOFunc callback));
IL2CPP_INIT(void, profiler_install_thread, (Il2CppProfileThreadFunc start, Il2CppProfileThreadFunc end));
IL2CPP_INIT(uint32_t, property_get_flags, (const PropertyInfo * prop));
IL2CPP_INIT(const MethodInfo*, property_get_get_method, (const PropertyInfo * prop));
IL2CPP_INIT(const MethodInfo*, property_get_set_method, (const PropertyInfo * prop));
IL2CPP_INIT(const char*, property_get_name, (const PropertyInfo * prop));
IL2CPP_INIT(Il2CppClass*, property_get_parent, (const PropertyInfo * prop));
IL2CPP_INIT(Il2CppClass*, object_get_class, (Il2CppObject * obj));
IL2CPP_INIT(uint32_t, object_get_size, (Il2CppObject * obj));
IL2CPP_INIT(const MethodInfo*, object_get_virtual_method, (Il2CppObject * obj, const MethodInfo * method));
IL2CPP_INIT(Il2CppObject*, object_new, (const Il2CppClass * klass));
IL2CPP_INIT(void*, object_unbox, (Il2CppObject * obj));
IL2CPP_INIT(Il2CppObject*, value_box, (Il2CppClass * klass, void* data));
IL2CPP_INIT(void, monitor_enter, (Il2CppObject * obj));
IL2CPP_INIT(bool, monitor_try_enter, (Il2CppObject * obj, uint32_t timeout));
IL2CPP_INIT(void, monitor_exit, (Il2CppObject * obj));
IL2CPP_INIT(void, monitor_pulse, (Il2CppObject * obj));
IL2CPP_INIT(void, monitor_pulse_all, (Il2CppObject * obj));
IL2CPP_INIT(void, monitor_wait, (Il2CppObject * obj));
IL2CPP_INIT(bool, monitor_try_wait, (Il2CppObject * obj, uint32_t timeout));
IL2CPP_INIT(Il2CppObject*, runtime_invoke, (const MethodInfo * method, void *obj, void **params, Il2CppException **exc));
IL2CPP_INIT(Il2CppObject*, runtime_invoke_convert_args, (const MethodInfo * method, void *obj, Il2CppObject **params, int paramCount, Il2CppException **exc));
IL2CPP_INIT(void, runtime_class_init, (Il2CppClass * klass));
IL2CPP_INIT(void, runtime_object_init, (Il2CppObject * obj));
IL2CPP_INIT(void, runtime_object_init_exception, (Il2CppObject * obj, Il2CppException** exc));
IL2CPP_INIT(void, runtime_unhandled_exception_policy_set, (Il2CppRuntimeUnhandledExceptionPolicy value));
IL2CPP_INIT(int32_t, string_length, (Il2CppString * str));
IL2CPP_INIT(Il2CppChar*, string_chars, (Il2CppString * str));
IL2CPP_INIT(Il2CppString*, string_new, (const char* str));
IL2CPP_INIT(Il2CppString*, string_new_len, (const char* str, uint32_t length));
IL2CPP_INIT(Il2CppString*, string_new_utf16, (const Il2CppChar * text, int32_t len));
IL2CPP_INIT(Il2CppString*, string_new_wrapper, (const char* str));
IL2CPP_INIT(Il2CppString*, string_intern, (Il2CppString * str));
IL2CPP_INIT(Il2CppString*, string_is_interned, (Il2CppString * str));
IL2CPP_INIT(Il2CppThread*, thread_current, ());
IL2CPP_INIT(Il2CppThread*, thread_attach, (Il2CppDomain * domain));
IL2CPP_INIT(void, thread_detach, (Il2CppThread * thread));
IL2CPP_INIT(Il2CppThread**, thread_get_all_attached_threads, (size_t * size));
IL2CPP_INIT(bool, is_vm_thread, (Il2CppThread * thread));
IL2CPP_INIT(void, current_thread_walk_frame_stack, (Il2CppFrameWalkFunc func, void* user_data));
IL2CPP_INIT(void, thread_walk_frame_stack, (Il2CppThread * thread, Il2CppFrameWalkFunc func, void* user_data));
IL2CPP_INIT(bool, current_thread_get_top_frame, (Il2CppStackFrameInfo * frame));
IL2CPP_INIT(bool, thread_get_top_frame, (Il2CppThread * thread, Il2CppStackFrameInfo * frame));
IL2CPP_INIT(bool, current_thread_get_frame_at, (int32_t offset, Il2CppStackFrameInfo * frame));
IL2CPP_INIT(bool, thread_get_frame_at, (Il2CppThread * thread, int32_t offset, Il2CppStackFrameInfo * frame));
IL2CPP_INIT(int32_t, current_thread_get_stack_depth, ());
IL2CPP_INIT(int32_t, thread_get_stack_depth, (Il2CppThread * thread));
IL2CPP_INIT(void, override_stack_backtrace, (Il2CppBacktraceFunc stackBacktraceFunc));
IL2CPP_INIT(Il2CppObject*, type_get_object, (const Il2CppType * type));
IL2CPP_INIT(int, type_get_type, (const Il2CppType * type));
IL2CPP_INIT(Il2CppClass*, type_get_class_or_element_class, (const Il2CppType * type));
IL2CPP_INIT(char*, type_get_name, (const Il2CppType * type));
IL2CPP_INIT(bool, type_is_byref, (const Il2CppType * type));
IL2CPP_INIT(uint32_t, type_get_attrs, (const Il2CppType * type));
IL2CPP_INIT(bool, type_equals, (const Il2CppType * type, const Il2CppType * otherType));
IL2CPP_INIT(char*, type_get_assembly_qualified_name, (const Il2CppType * type));
IL2CPP_INIT(bool, type_is_static, (const Il2CppType * type));
IL2CPP_INIT(bool, type_is_pointer_type, (const Il2CppType * type));
IL2CPP_INIT(const Il2CppAssembly*, image_get_assembly, (const Il2CppImage * image));
IL2CPP_INIT(const char*, image_get_name, (const Il2CppImage * image));
IL2CPP_INIT(const char*, image_get_filename, (const Il2CppImage * image));
IL2CPP_INIT(const MethodInfo*, image_get_entry_point, (const Il2CppImage * image));
IL2CPP_INIT(size_t, image_get_class_count, (const Il2CppImage * image));
IL2CPP_INIT(const Il2CppClass*, image_get_class, (const Il2CppImage * image, size_t index));
IL2CPP_INIT(Il2CppManagedMemorySnapshot*, capture_memory_snapshot, ());
IL2CPP_INIT(void, free_captured_memory_snapshot, (Il2CppManagedMemorySnapshot * snapshot));
IL2CPP_INIT(void, set_find_plugin_callback, (Il2CppSetFindPlugInCallback method));
IL2CPP_INIT(void, register_log_callback, (Il2CppLogCallback method));
IL2CPP_INIT(void, debugger_set_agent_options, (const char* options));
IL2CPP_INIT(bool, is_debugger_attached, ());
IL2CPP_INIT(void, register_debugger_agent_transport, (Il2CppDebuggerTransport * debuggerTransport));
IL2CPP_INIT(bool, debug_get_method_info, (const MethodInfo*, Il2CppMethodDebugInfo * methodDebugInfo));
IL2CPP_INIT(void, unity_install_unitytls_interface, (const void* unitytlsInterfaceStruct));
IL2CPP_INIT(Il2CppCustomAttrInfo*, custom_attrs_from_class, (Il2CppClass * klass));
IL2CPP_INIT(Il2CppCustomAttrInfo*, custom_attrs_from_method, (const MethodInfo * method));
IL2CPP_INIT(Il2CppObject*, custom_attrs_get_attr, (Il2CppCustomAttrInfo * ainfo, Il2CppClass * attr_klass));
IL2CPP_INIT(bool, custom_attrs_has_attr, (Il2CppCustomAttrInfo * ainfo, Il2CppClass * attr_klass));
IL2CPP_INIT(Il2CppArray*, custom_attrs_construct, (Il2CppCustomAttrInfo * cinfo));
IL2CPP_INIT(void, custom_attrs_free, (Il2CppCustomAttrInfo * ainfo));
IL2CPP_INIT(void, class_set_userdata, (Il2CppClass * klass, void* userdata));
IL2CPP_INIT(int, class_get_userdata_offset, ());
IL2CPP_INIT(void, set_default_thread_affinity, (int64_t affinity_mask));

IL2CPP_INIT(bool, Class_Init, (Il2CppClass *klass));
IL2CPP_INIT(Il2CppClass *, MetadataCache_GetTypeInfoFromHandle, (Il2CppMetadataTypeHandle handle));
IL2CPP_INIT(Il2CppClass *, MetadataCache_GetTypeInfoFromTypeIndex, (TypeIndex index));
IL2CPP_INIT(Il2CppClass *, GlobalMetadata_GetTypeInfoFromHandle, (TypeDefinitionIndex index));
IL2CPP_INIT(Il2CppClass *, GlobalMetadata_GetTypeInfoFromTypeDefinitionIndex, (TypeDefinitionIndex index));
IL2CPP_INIT(Il2CppClass *, FromTypeDefinition, (TypeDefinitionIndex index));
IL2CPP_INIT(std::string, Type_GetName, (const Il2CppType *type, Il2CppTypeNameFormat format));
IL2CPP_INIT(Il2CppClass *, Class_FromIl2CppType, (Il2CppType *type));
IL2CPP_INIT(Il2CppClass *, GenericClass_GetClass, (Il2CppGenericClass *genericClass));
IL2CPP_INIT(Il2CppClass *, Class_GetPtrClass, (Il2CppClass *elementClass));
IL2CPP_INIT(std::vector<const Il2CppAssembly *>, Assembly_GetAllAssemblies, ());
#pragma endregion

namespace Gluon {
    bool Il2CppFunctions::initialised;
    Il2CppDefaults *Il2CppFunctions::il2cppDefaults;

    Il2CppMetadataRegistration **Il2CppFunctions::il2cppMetadataRegistrationPtr;
    void **Il2CppFunctions::globalMetadataPtr;
    Il2CppGlobalMetadataHeader **Il2CppFunctions::globalMetadataHeaderPtr;

    uint64_t getOffsetFromPtr(uint32_t *ptr) {
        HANDLE main = GetCurrentProcess();
        HMODULE gameAssembly = GetModuleHandleA("GameAssembly.dll");

        MODULEINFO moduleInfo;
        GetModuleInformation(main, gameAssembly, &moduleInfo, sizeof(moduleInfo));

        uint32_t *base = static_cast<uint32_t *>(moduleInfo.lpBaseOfDll);
        return ptr - base;
    }

    uint32_t *traceClassInit() {
        auto Array_NewSpecific_addr = Gluon::XrefHelpers::findNthJmp<1>(
                reinterpret_cast<const uint32_t *>(Il2CppFunctions::il2cpp_array_new_specific));
        Array_NewSpecific_addr.value();
        auto match = Gluon::XrefHelpers::findNthCall<1>(Array_NewSpecific_addr.value());
        if (!match) {
            Gluon::Logging::Logger::error("Failed to find Class::Init. Will abort.");
            SAFE_ABORT();
        }

        Gluon::Logger::info("Class::Init offset: {:0x}", getOffsetFromPtr(match.value()));

        return match.value();
    }

    uint32_t *traceGetTypeInfoFromHandle() {
        auto Image_GetType_addr = Gluon::XrefHelpers::findNthJmp<1>(reinterpret_cast<uint32_t *>(Il2CppFunctions::il2cpp_image_get_class));
        if (!Image_GetType_addr) {
            SAFE_ABORT_MSG("Failed to find Image::GetType!");
        }

        auto MetadataCache_GetTypeInfoFromHandle_addr = Gluon::XrefHelpers::findNthJmp<1>(Image_GetType_addr.value());
        if (!MetadataCache_GetTypeInfoFromHandle_addr) {
            SAFE_ABORT_MSG("Failed to find MetadataCache::GetTypeInfoFromHandle!");
        }

        return MetadataCache_GetTypeInfoFromHandle_addr.value();
    }

    uint32_t *traceGetTypeInfoFromHandle2() {
        auto GlobalMetadata_GetTypeInfoFromHandle_addr = Gluon::XrefHelpers::findNthJmp<1>(reinterpret_cast<uint32_t *>(Il2CppFunctions::il2cpp_MetadataCache_GetTypeInfoFromHandle));
        if (!GlobalMetadata_GetTypeInfoFromHandle_addr) {
            SAFE_ABORT_MSG("Failed to find GlobalMetadata::GetTypeInfoFromHandle!");
        }

        return GlobalMetadata_GetTypeInfoFromHandle_addr.value();
    }

    uint32_t *traceGetTypeInfoFromTypeIndex() {
        // il2cpp_field_get_value_object // 1st JMP
        //     Field::GetValueObjectForThread // 5th CALL
        //         utils::BlobReader::GetConstrantValueFromBlob // 1st CALL
        //             BlobReader::GetConstantValueFromBlob // 10th CALL
        //                 MetadataCache::GetTypeInfoFromTypeIndex
        auto Field_GetValueObjectForThread = Gluon::XrefHelpers::findNthJmp<1, false, -1, 1024>(reinterpret_cast<uint32_t *>(Il2CppFunctions::il2cpp_field_get_value_object));
        if (!Field_GetValueObjectForThread) {
            SAFE_ABORT_MSG("Failed to find Field::GetValueObjectForThread!");
        }

        auto BlobReader_GetConstantValueFromBlob = Gluon::XrefHelpers::findNthCall<5>(Field_GetValueObjectForThread.value());
        if (!BlobReader_GetConstantValueFromBlob) {
            SAFE_ABORT_MSG("Failed to find BlobReader::GetConstantValueFromBlob!");
        }

        auto BlobReader_GetConstantValueFromBlob2 = Gluon::XrefHelpers::findNthCall<1>(BlobReader_GetConstantValueFromBlob.value());
        if (!BlobReader_GetConstantValueFromBlob2) {
            SAFE_ABORT_MSG("Failed to find BlobReader::GetConstantValueFromBlob2!");
        }

        auto MetadataCache_GetTypeInfoFromTypeIndex = Gluon::XrefHelpers::findNthCall<10>(BlobReader_GetConstantValueFromBlob2.value());
        if (!MetadataCache_GetTypeInfoFromTypeIndex) {
            SAFE_ABORT_MSG("Failed to find MetadataCache::GetTypeInfoFromTypeIndex!");
        }

        return MetadataCache_GetTypeInfoFromTypeIndex.value();
    }

    uint32_t *traceGetTypeInfoFromTypeDefinitionIndex() {
        uint32_t *getTypeInfoFromHandle = reinterpret_cast<uint32_t *>(Il2CppFunctions::il2cpp_GlobalMetadata_GetTypeInfoFromHandle);
        auto GlobalMetadata_GetTypeInfoFromTypeDefinitionIndex_addr = Gluon::XrefHelpers::findNthJmp<1>(getTypeInfoFromHandle);
        if (!GlobalMetadata_GetTypeInfoFromTypeDefinitionIndex_addr) {
            SAFE_ABORT_MSG("Failed to find GlobalMetadata::GetTypeInfoFromTypeDefinitionIndex!");
        }

        return GlobalMetadata_GetTypeInfoFromTypeDefinitionIndex_addr.value();
    }

    uint32_t *traceFromTypeDefinition() {
        // GetTypeInfoFromTypeDefinitionIndex
        //     FromTypeDefinition 2nd Call
        auto fromTypeDefinition_addr = Gluon::XrefHelpers::findNthCall<3, true>(
                reinterpret_cast<uint32_t *>(Il2CppFunctions::il2cpp_GlobalMetadata_GetTypeInfoFromTypeDefinitionIndex)
        );
        if (!fromTypeDefinition_addr) {
            SAFE_ABORT_MSG("Failed to find FromTypeDefinition!");
        }
        return fromTypeDefinition_addr.value();
    }

    uint32_t *traceTypeGetName() {
        auto Type_GetName_addr = Gluon::XrefHelpers::findNthCall<1>(reinterpret_cast<uint32_t *>(Il2CppFunctions::il2cpp_type_get_assembly_qualified_name));
        if (!Type_GetName_addr) {
            SAFE_ABORT_MSG("Failed to find Type::GetName!");
        }
        return Type_GetName_addr.value();
    }

    uint32_t *traceClassFromIl2CppType() {
        auto Class_FromIl2CppType_addr = Gluon::XrefHelpers::findNthJmp<1>(reinterpret_cast<uint32_t *>(Gluon::Il2CppFunctions::il2cpp_class_from_il2cpp_type));
        if (!Class_FromIl2CppType_addr) {
            SAFE_ABORT_MSG("Failed to find Class::FromIl2CppType");
        }

        return Class_FromIl2CppType_addr.value();
    }

    uint32_t *traceGenericClassGetClass() {
        uint32_t *fromIl2CppType = reinterpret_cast<uint32_t *>(Il2CppFunctions::il2cpp_Class_FromIl2CppType);
        auto GenericClass_GetClass_addr = Gluon::XrefHelpers::findNthJmp<25, true>(fromIl2CppType);
        if (!GenericClass_GetClass_addr) {
            SAFE_ABORT_MSG("Failed to find GenericClass::GetClass!");
        }

        return GenericClass_GetClass_addr.value();
    }

    uint32_t *traceClassGetPtrClass() {
        uint32_t *fromIl2CppType = reinterpret_cast<uint32_t *>(Il2CppFunctions::il2cpp_Class_FromIl2CppType);
        auto Class_GetPtrClass_addr = Gluon::XrefHelpers::findNthJmp<22>(fromIl2CppType);
        if (!Class_GetPtrClass_addr) {
            SAFE_ABORT_MSG("Failed to find Class::GetPtrClass!");
        }

        return Class_GetPtrClass_addr.value();
    }

    uint32_t *traceAssemblyGetAllAssemblies() {
        auto Assembly_GetAllAssemblies_addr = Gluon::XrefHelpers::findNthCall<1>(reinterpret_cast<uint32_t *>(Il2CppFunctions::il2cpp_domain_get_assemblies));
        if (!Assembly_GetAllAssemblies_addr) {
            SAFE_ABORT_MSG("Failed to find Assembly::GetAllAssemblies!");
        }

        return Assembly_GetAllAssemblies_addr.value();
    }

    void traceAllFunctions() {
        Il2CppFunctions::il2cpp_Class_Init =
                reinterpret_cast<decltype(Il2CppFunctions::il2cpp_Class_Init)>(traceClassInit());
        Il2CppFunctions::il2cpp_MetadataCache_GetTypeInfoFromHandle =
                reinterpret_cast<decltype(Il2CppFunctions::il2cpp_MetadataCache_GetTypeInfoFromHandle)>(traceGetTypeInfoFromHandle());
        Il2CppFunctions::il2cpp_MetadataCache_GetTypeInfoFromTypeIndex =
                reinterpret_cast<decltype(Il2CppFunctions::il2cpp_MetadataCache_GetTypeInfoFromTypeIndex)>(traceGetTypeInfoFromTypeIndex());
        Il2CppFunctions::il2cpp_GlobalMetadata_GetTypeInfoFromHandle = // function inlined – preserving other value for compatibility purposes
                reinterpret_cast<decltype(Il2CppFunctions::il2cpp_GlobalMetadata_GetTypeInfoFromHandle)>(traceGetTypeInfoFromHandle2());
        Il2CppFunctions::il2cpp_GlobalMetadata_GetTypeInfoFromTypeDefinitionIndex =
                reinterpret_cast<decltype(Il2CppFunctions::il2cpp_GlobalMetadata_GetTypeInfoFromTypeDefinitionIndex)>(traceGetTypeInfoFromTypeDefinitionIndex());
        Il2CppFunctions::il2cpp_FromTypeDefinition =
                reinterpret_cast<decltype(Il2CppFunctions::il2cpp_FromTypeDefinition)>(traceFromTypeDefinition());
        Il2CppFunctions::il2cpp_Type_GetName =
                reinterpret_cast<decltype(Il2CppFunctions::il2cpp_Type_GetName)>(traceTypeGetName());
        Il2CppFunctions::il2cpp_Class_FromIl2CppType =
                reinterpret_cast<decltype(Il2CppFunctions::il2cpp_Class_FromIl2CppType)>(traceClassFromIl2CppType());
        Il2CppFunctions::il2cpp_GenericClass_GetClass =
                reinterpret_cast<decltype(Il2CppFunctions::il2cpp_GenericClass_GetClass)>(traceGenericClassGetClass());
        Il2CppFunctions::il2cpp_Class_GetPtrClass =
                reinterpret_cast<decltype(Il2CppFunctions::il2cpp_Class_GetPtrClass)>(traceClassGetPtrClass());
        Il2CppFunctions::il2cpp_Assembly_GetAllAssemblies =
                reinterpret_cast<decltype(Il2CppFunctions::il2cpp_Assembly_GetAllAssemblies)>(traceAssemblyGetAllAssemblies());
    }

    void traceDefaults() {
        auto runtimeInit = Gluon::XrefHelpers::findNthCall<2>(reinterpret_cast<const uint32_t *>(Il2CppFunctions::il2cpp_init));
        if (!runtimeInit) {
            SAFE_ABORT_MSG("Failed to find Runtime::Init!");
        }

        auto address = Gluon::XrefHelpers::findNthLea<4>(runtimeInit.value());
        if (!address) {
            SAFE_ABORT_MSG("Failed to find effective address for defaults!");
        }
        Il2CppFunctions::il2cppDefaults = reinterpret_cast<Il2CppDefaults *>(address.value());
    }

    uint32_t *traceGlobalMetadataHeader() {
        // GetTypeInfoFromTypeDefinitionIndex
        //     FromTypeDefinition 2nd Call
        //         5th MOV
        Gluon::Logger::info("Starting GlobalMetadataHeader trace");
        auto globalMetadataHeader_addr = Gluon::XrefHelpers::findNthMov<5, true>(reinterpret_cast<const uint32_t *>(Il2CppFunctions::il2cpp_FromTypeDefinition));
        if (!globalMetadataHeader_addr) {
            SAFE_ABORT_MSG("Failed to find s_GlobalMetadataHeader!");
        }
        Gluon::Logger::info("Finished GlobalMetadataHeader trace");
        return globalMetadataHeader_addr.value();
    }

    uint32_t *traceIl2CppMetadataRegistration() {
        // GetTypeInfoFromTypeDefinitionIndex
        //     FromTypeDefinition
        //         7th MOV
        auto il2cppMetadataRegistration_addr = Gluon::XrefHelpers::findNthMov<7, true>(reinterpret_cast<const uint32_t *>(Il2CppFunctions::il2cpp_FromTypeDefinition));
        if (!il2cppMetadataRegistration_addr) {
            SAFE_ABORT_MSG("Failed to find s_GlobalMetadata!");
        }

        return il2cppMetadataRegistration_addr.value();
    }

    uint32_t *traceGlobalMetadata() {
        // GetTypeInfoFromTypeDefinitionIndex
        //     FromTypeDefinition
        //         20th MOV
        auto globalMetadata_addr = Gluon::XrefHelpers::findNthMov<19, true>(reinterpret_cast<const uint32_t *>(Il2CppFunctions::il2cpp_FromTypeDefinition));
        if (!globalMetadata_addr) {
            SAFE_ABORT_MSG("Failed to find s_GlobalMetadataHeader!");
        }

        return globalMetadata_addr.value();
    }

    void traceMetadata() {
        Il2CppFunctions::globalMetadataHeaderPtr = reinterpret_cast<Il2CppGlobalMetadataHeader **>(traceGlobalMetadataHeader());
        Il2CppFunctions::il2cppMetadataRegistrationPtr = reinterpret_cast<Il2CppMetadataRegistration **>(traceIl2CppMetadataRegistration());
        Il2CppFunctions::globalMetadataPtr = reinterpret_cast<void **>(traceGlobalMetadata());
    }

    void Il2CppFunctions::initialise() {
        constexpr const char *kIl2CppAssembly = "GameAssembly.dll";

        if (initialised) {
            return;
        }

        SetLastError(0);
        HMODULE il2cppAssembly = LoadLibraryA(kIl2CppAssembly);
        if (!il2cppAssembly) {
            Gluon::Logging::Logger::error("Unable to load IL2CPP main assembly. Will abort.");
            SAFE_ABORT();
        }

        IL2CPP_LOAD(init);
        IL2CPP_LOAD(init_utf16);
        IL2CPP_LOAD(shutdown);
        IL2CPP_LOAD(set_config_dir);
        IL2CPP_LOAD(set_data_dir);
        IL2CPP_LOAD(set_temp_dir);
        IL2CPP_LOAD(set_commandline_arguments);
        IL2CPP_LOAD(set_commandline_arguments_utf16);
        IL2CPP_LOAD(set_config_utf16);
        IL2CPP_LOAD(set_config);
        IL2CPP_LOAD(set_memory_callbacks);
        IL2CPP_LOAD(get_corlib);
        IL2CPP_LOAD(add_internal_call);
        IL2CPP_LOAD(resolve_icall);
        IL2CPP_LOAD(alloc);
        IL2CPP_LOAD(free);
        IL2CPP_LOAD(array_class_get);
        IL2CPP_LOAD(array_length);
        IL2CPP_LOAD(array_get_byte_length);
        IL2CPP_LOAD(array_new);
        IL2CPP_LOAD(array_new_specific);
        IL2CPP_LOAD(array_new_full);
        IL2CPP_LOAD(bounded_array_class_get);
        IL2CPP_LOAD(array_element_size);
        IL2CPP_LOAD(assembly_get_image);
        IL2CPP_LOAD(class_for_each);
        IL2CPP_LOAD(class_enum_basetype);
        IL2CPP_LOAD(class_is_generic);
        IL2CPP_LOAD(class_is_inflated);
        IL2CPP_LOAD(class_is_assignable_from);
        IL2CPP_LOAD(class_is_subclass_of);
        IL2CPP_LOAD(class_has_parent);
        IL2CPP_LOAD(class_from_il2cpp_type);
        IL2CPP_LOAD(class_from_name);
        IL2CPP_LOAD(class_from_system_type);
        IL2CPP_LOAD(class_get_element_class);
        IL2CPP_LOAD(class_get_events);
        IL2CPP_LOAD(class_get_fields);
        IL2CPP_LOAD(class_get_nested_types);
        IL2CPP_LOAD(class_get_interfaces);
        IL2CPP_LOAD(class_get_properties);
        IL2CPP_LOAD(class_get_property_from_name);
        IL2CPP_LOAD(class_get_field_from_name);
        IL2CPP_LOAD(class_get_methods);
        IL2CPP_LOAD(class_get_method_from_name);
        IL2CPP_LOAD(class_get_name);
        IL2CPP_LOAD(type_get_name_chunked);
        IL2CPP_LOAD(class_get_namespace);
        IL2CPP_LOAD(class_get_parent);
        IL2CPP_LOAD(class_get_declaring_type);
        IL2CPP_LOAD(class_instance_size);
        IL2CPP_LOAD(class_num_fields);
        IL2CPP_LOAD(class_is_valuetype);
        IL2CPP_LOAD(class_value_size);
        IL2CPP_LOAD(class_is_blittable);
        IL2CPP_LOAD(class_get_flags);
        IL2CPP_LOAD(class_is_abstract);
        IL2CPP_LOAD(class_is_interface);
        IL2CPP_LOAD(class_array_element_size);
        IL2CPP_LOAD(class_from_type);
        IL2CPP_LOAD(class_get_type);
        IL2CPP_LOAD(class_get_type_token);
        IL2CPP_LOAD(class_has_attribute);
        IL2CPP_LOAD(class_has_references);
        IL2CPP_LOAD(class_is_enum);
        IL2CPP_LOAD(class_get_image);
        IL2CPP_LOAD(class_get_assemblyname);
        IL2CPP_LOAD(class_get_rank);
        IL2CPP_LOAD(class_get_data_size);
        IL2CPP_LOAD(class_get_static_field_data);
        IL2CPP_LOAD(class_get_bitmap_size);
        IL2CPP_LOAD(class_get_bitmap);
        IL2CPP_LOAD(stats_dump_to_file);
        IL2CPP_LOAD(stats_get_value);
        IL2CPP_LOAD(domain_get);
        IL2CPP_LOAD(domain_assembly_open);
        IL2CPP_LOAD(domain_get_assemblies);
        IL2CPP_LOAD(raise_exception);
        IL2CPP_LOAD(exception_from_name_msg);
        IL2CPP_LOAD(get_exception_argument_null);
        IL2CPP_LOAD(format_exception);
        IL2CPP_LOAD(format_stack_trace);
        IL2CPP_LOAD(unhandled_exception);
        IL2CPP_LOAD(field_get_flags);
        IL2CPP_LOAD(field_get_name);
        IL2CPP_LOAD(field_get_parent);
        IL2CPP_LOAD(field_get_offset);
        IL2CPP_LOAD(field_get_type);
        IL2CPP_LOAD(field_get_value);
        IL2CPP_LOAD(field_get_value_object);
        IL2CPP_LOAD(field_has_attribute);
        IL2CPP_LOAD(field_set_value);
        IL2CPP_LOAD(field_static_get_value);
        IL2CPP_LOAD(field_static_set_value);
        IL2CPP_LOAD(field_set_value_object);
        IL2CPP_LOAD(field_is_literal);
        IL2CPP_LOAD(gc_collect);
        IL2CPP_LOAD(gc_collect_a_little);
        IL2CPP_LOAD(gc_disable);
        IL2CPP_LOAD(gc_enable);
        IL2CPP_LOAD(gc_is_disabled);
        IL2CPP_LOAD(gc_get_max_time_slice_ns);
        IL2CPP_LOAD(gc_set_max_time_slice_ns);
        IL2CPP_LOAD(gc_is_incremental);
        IL2CPP_LOAD(gc_get_used_size);
        IL2CPP_LOAD(gc_get_heap_size);
        IL2CPP_LOAD(gc_wbarrier_set_field);
        IL2CPP_LOAD(gc_has_strict_wbarriers);
        IL2CPP_LOAD(gc_set_external_allocation_tracker);
        IL2CPP_LOAD(gc_set_external_wbarrier_tracker);
        IL2CPP_LOAD(gc_foreach_heap);
        IL2CPP_LOAD(gc_free_fixed);
        IL2CPP_LOAD(gc_alloc_fixed);
        IL2CPP_LOAD(stop_gc_world);
        IL2CPP_LOAD(start_gc_world);
        IL2CPP_LOAD(gchandle_new);
        IL2CPP_LOAD(gchandle_new_weakref);
        IL2CPP_LOAD(gchandle_get_target);
        IL2CPP_LOAD(gchandle_free);
        IL2CPP_LOAD(gchandle_foreach_get_target);
        IL2CPP_LOAD(object_header_size);
        IL2CPP_LOAD(array_object_header_size);
        IL2CPP_LOAD(offset_of_array_length_in_array_object_header);
        IL2CPP_LOAD(offset_of_array_bounds_in_array_object_header);
        IL2CPP_LOAD(allocation_granularity);
        IL2CPP_LOAD(unity_liveness_allocate_struct);
        IL2CPP_LOAD(unity_liveness_finalize);
        IL2CPP_LOAD(unity_liveness_free_struct);
        IL2CPP_LOAD(unity_liveness_calculation_from_root);
        IL2CPP_LOAD(unity_liveness_calculation_from_statics);
        IL2CPP_LOAD(method_get_return_type);
        IL2CPP_LOAD(method_get_declaring_type);
        IL2CPP_LOAD(method_get_name);
        IL2CPP_LOAD(method_get_from_reflection);
        IL2CPP_LOAD(method_get_object);
        IL2CPP_LOAD(method_is_generic);
        IL2CPP_LOAD(method_is_inflated);
        IL2CPP_LOAD(method_is_instance);
        IL2CPP_LOAD(method_get_param_count);
        IL2CPP_LOAD(method_get_param);
        IL2CPP_LOAD(method_get_class);
        IL2CPP_LOAD(method_has_attribute);
        IL2CPP_LOAD(method_get_flags);
        IL2CPP_LOAD(method_get_token);
        IL2CPP_LOAD(method_get_param_name);
        IL2CPP_LOAD(profiler_install);
        IL2CPP_LOAD(profiler_set_events);
        IL2CPP_LOAD(profiler_install_enter_leave);
        IL2CPP_LOAD(profiler_install_allocation);
        IL2CPP_LOAD(profiler_install_gc);
        IL2CPP_LOAD(profiler_install_fileio);
        IL2CPP_LOAD(profiler_install_thread);
        IL2CPP_LOAD(property_get_flags);
        IL2CPP_LOAD(property_get_get_method);
        IL2CPP_LOAD(property_get_set_method);
        IL2CPP_LOAD(property_get_name);
        IL2CPP_LOAD(property_get_parent);
        IL2CPP_LOAD(object_get_class);
        IL2CPP_LOAD(object_get_size);
        IL2CPP_LOAD(object_get_virtual_method);
        IL2CPP_LOAD(object_new);
        IL2CPP_LOAD(object_unbox);
        IL2CPP_LOAD(value_box);
        IL2CPP_LOAD(monitor_enter);
        IL2CPP_LOAD(monitor_try_enter);
        IL2CPP_LOAD(monitor_exit);
        IL2CPP_LOAD(monitor_pulse);
        IL2CPP_LOAD(monitor_pulse_all);
        IL2CPP_LOAD(monitor_wait);
        IL2CPP_LOAD(monitor_try_wait);
        IL2CPP_LOAD(runtime_invoke);
        IL2CPP_LOAD(runtime_invoke_convert_args);
        IL2CPP_LOAD(runtime_class_init);
        IL2CPP_LOAD(runtime_object_init);
        IL2CPP_LOAD(runtime_object_init_exception);
        IL2CPP_LOAD(runtime_unhandled_exception_policy_set);
        IL2CPP_LOAD(string_length);
        IL2CPP_LOAD(string_chars);
        IL2CPP_LOAD(string_new);
        IL2CPP_LOAD(string_new_len);
        IL2CPP_LOAD(string_new_utf16);
        IL2CPP_LOAD(string_new_wrapper);
        IL2CPP_LOAD(string_intern);
        IL2CPP_LOAD(string_is_interned);
        IL2CPP_LOAD(thread_current);
        IL2CPP_LOAD(thread_attach);
        IL2CPP_LOAD(thread_detach);
        IL2CPP_LOAD(thread_get_all_attached_threads);
        IL2CPP_LOAD(is_vm_thread);
        IL2CPP_LOAD(current_thread_walk_frame_stack);
        IL2CPP_LOAD(thread_walk_frame_stack);
        IL2CPP_LOAD(current_thread_get_top_frame);
        IL2CPP_LOAD(thread_get_top_frame);
        IL2CPP_LOAD(current_thread_get_frame_at);
        IL2CPP_LOAD(thread_get_frame_at);
        IL2CPP_LOAD(current_thread_get_stack_depth);
        IL2CPP_LOAD(thread_get_stack_depth);
        IL2CPP_LOAD(override_stack_backtrace);
        IL2CPP_LOAD(type_get_object);
        IL2CPP_LOAD(type_get_type);
        IL2CPP_LOAD(type_get_class_or_element_class);
        IL2CPP_LOAD(type_get_name);
        IL2CPP_LOAD(type_is_byref);
        IL2CPP_LOAD(type_get_attrs);
        IL2CPP_LOAD(type_equals);
        IL2CPP_LOAD(type_get_assembly_qualified_name);
        IL2CPP_LOAD(type_is_static);
        IL2CPP_LOAD(type_is_pointer_type);
        IL2CPP_LOAD(image_get_assembly);
        IL2CPP_LOAD(image_get_name);
        IL2CPP_LOAD(image_get_filename);
        IL2CPP_LOAD(image_get_entry_point);
        IL2CPP_LOAD(image_get_class_count);
        IL2CPP_LOAD(image_get_class);
        IL2CPP_LOAD(capture_memory_snapshot);
        IL2CPP_LOAD(free_captured_memory_snapshot);
        IL2CPP_LOAD(set_find_plugin_callback);
        IL2CPP_LOAD(register_log_callback);
        IL2CPP_LOAD(debugger_set_agent_options);
        IL2CPP_LOAD(is_debugger_attached);
        IL2CPP_LOAD(register_debugger_agent_transport);
        IL2CPP_LOAD(debug_get_method_info);
        IL2CPP_LOAD(unity_install_unitytls_interface);
        IL2CPP_LOAD(custom_attrs_from_class);
        IL2CPP_LOAD(custom_attrs_from_method);
        IL2CPP_LOAD(custom_attrs_get_attr);
        IL2CPP_LOAD(custom_attrs_has_attr);
        IL2CPP_LOAD(custom_attrs_construct);
        IL2CPP_LOAD(custom_attrs_free);
        IL2CPP_LOAD(class_set_userdata);
        IL2CPP_LOAD(class_get_userdata_offset);

        traceAllFunctions();
        traceDefaults();
        traceMetadata();

        initialised = true;
    }

    const char *Il2CppFunctions::MetadataCache_GetStringFromIndex(StringIndex index) {
        checkGlobalMetadata();
        IL2CPP_ASSERT(index <= globalMetadataHeader->stringSize);
        const char *strings = (reinterpret_cast<const char *>(globalMetadata) + globalMetadataHeader->stringOffset) + index;
        return strings;
    }

    const Il2CppTypeDefinition *Il2CppFunctions::MetadataCache_GetTypeDefinitionFromIndex(TypeDefinitionIndex index) {
        checkGlobalMetadata();

        if (index == kTypeDefinitionIndexInvalid) {
            return nullptr;
        }

        IL2CPP_ASSERT(index >= 0 && static_cast<uint32_t>(index) < globalMetadataHeader->typeDefinitionsSize / sizeof(Il2CppTypeDefinition));
        auto typeDefinitions = reinterpret_cast<const Il2CppTypeDefinition *>(reinterpret_cast<const char *>(globalMetadata) + globalMetadataHeader->typeDefinitionsOffset);
        return typeDefinitions + index;
    }

    TypeDefinitionIndex Il2CppFunctions::MetadataCache_GetExportedTypeFromIndex(TypeDefinitionIndex index) {
        checkGlobalMetadata();

        if (index == kTypeDefinitionIndexInvalid) {
            return kTypeDefinitionIndexInvalid;
        }

        IL2CPP_ASSERT(index >= 0 && static_cast<uint32_t>(index) < globalMetadataHeader->exportedTypeDefinitionsSize / sizeof(TypeDefinitionIndex));
        auto exportedTypes = reinterpret_cast<TypeDefinitionIndex *>(reinterpret_cast<char *>(globalMetadata) + globalMetadataHeader->exportedTypeDefinitionsOffset);
        return *(exportedTypes + index);
    }

    const Il2CppGenericContainer *
    Il2CppFunctions::MetadataCache_GetGenericContainerFromIndex(GenericContainerIndex index) {
        checkGlobalMetadata();

        if (index == kGenericContainerIndexInvalid) {
            return nullptr;
        }

        IL2CPP_ASSERT(index >= 0 && static_cast<uint32_t>(index) <= globalMetadataHeader->genericContainersSize / sizeof(Il2CppGenericContainer));
        const Il2CppGenericContainer *genericContainers = reinterpret_cast<const Il2CppGenericContainer *>(reinterpret_cast<const char *>(globalMetadata) + globalMetadataHeader->genericContainersOffset);
        return genericContainers + index;
    }

    const Il2CppGenericParameter *
    Il2CppFunctions::MetadataCache_GetGenericParameterFromIndex(GenericParameterIndex index) {
        checkGlobalMetadata();
        if (index == kGenericParameterIndexInvalid) {
            return nullptr;
        }

        IL2CPP_ASSERT(index >= 0 && static_cast<uint32_t>(index) <= globalMetadataHeader->genericParametersSize / sizeof(Il2CppGenericParameter));
        const Il2CppGenericParameter* genericParameters = reinterpret_cast<const Il2CppGenericParameter *>(reinterpret_cast<const char *>(globalMetadata) + globalMetadataHeader->genericParametersOffset);
        return genericParameters + index;
    }

    Il2CppClass *Il2CppFunctions::MetadataCache_GetNestedTypeFromIndex(NestedTypeIndex index) {
        checkGlobalMetadata();

        IL2CPP_ASSERT(index >= 0 && static_cast<uint32_t>(index) <= globalMetadataHeader->nestedTypesCount / sizeof(TypeDefinitionIndex));
        auto nestedTypeIndices = reinterpret_cast<const TypeDefinitionIndex *>(reinterpret_cast<const char *>(globalMetadata) + globalMetadataHeader->nestedTypesOffset);

        return GlobalMetadata_GetTypeInfoFromTypeDefinitionIndex(nestedTypeIndices[index]);
    }

    TypeDefinitionIndex Il2CppFunctions::MetadataCache_GetIndexForTypeDefinition(const Il2CppClass *klass) {
        checkGlobalMetadata();
        return MetadataCache_GetIndexForTypeDefinition(reinterpret_cast<const Il2CppTypeDefinition*>(klass->typeMetadataHandle));
    }

    TypeDefinitionIndex
    Il2CppFunctions::MetadataCache_GetIndexForTypeDefinition(const Il2CppTypeDefinition *typeDefinition) {
        checkGlobalMetadata();
        IL2CPP_ASSERT(klass);
        const Il2CppTypeDefinition* typeDefinitions = reinterpret_cast<const Il2CppTypeDefinition *>(reinterpret_cast<const char *>(globalMetadata) + globalMetadataHeader->typeDefinitionsOffset);

        IL2CPP_ASSERT(typeDefinition->typeDefinition >= typeDefinitions && typeDefinition->typeDefinition < typeDefinitions + globalMetadataHeader->typeDefinitionsSize / sizeof(Il2CppTypeDefinition));
        ptrdiff_t index = typeDefinition - typeDefinitions;
        IL2CPP_ASSERT(index <= std::numeric_limits<TypeDefinitionIndex>::max());
        return static_cast<TypeDefinitionIndex>(index);
    }

    GenericParameterIndex
    Il2CppFunctions::MetadataCache_GetGenericParameterIndexFromParameter(Il2CppMetadataGenericParameterHandle handle) {
        checkGlobalMetadata();
        const Il2CppGenericParameter* genericParameter = reinterpret_cast<const Il2CppGenericParameter*>(handle);
        const Il2CppGenericParameter* genericParameters = reinterpret_cast<const Il2CppGenericParameter*>(reinterpret_cast<const char *>(globalMetadata) + globalMetadataHeader->genericParametersOffset);

        IL2CPP_ASSERT(genericParameter >= genericParameters && genericParameter < genericParameters + globalMetadataHeader->genericParametersSize / sizeof(Il2CppGenericParameter));

        ptrdiff_t index = reinterpret_cast<Il2CppGenericParameter const*>(genericParameter) - reinterpret_cast<Il2CppGenericParameter const*>(genericParameters);
        IL2CPP_ASSERT(index <= std::numeric_limits<GenericParameterIndex>::max());
        return static_cast<GenericParameterIndex>(index);
    }

    const Il2CppTypeDefinition *Il2CppFunctions::MetadataCache_GetTypeDefinition(Il2CppClass *klass) {
        checkGlobalMetadata();
        return reinterpret_cast<const Il2CppTypeDefinition*>(klass->typeMetadataHandle);
    }

    GenericParameterIndex Il2CppFunctions::MetadataCache_GetGenericContainerIndex(Il2CppClass *klass) {
        checkGlobalMetadata();
        return MetadataCache_GetTypeDefinition(klass)->genericContainerIndex;
    }
}