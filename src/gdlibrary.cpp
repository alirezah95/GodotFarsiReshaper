#include "reshaper.hpp"

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options* opt)
{
	godot::Godot::gdnative_init(opt);
}


extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options* opt)
{
	godot::Godot::gdnative_terminate(opt);
}


extern "C" void GDN_EXPORT godot_nativescript_init(void* handle)
{
	godot::Godot::nativescript_init(handle);

	/* Now we register all of our classes in library */
	godot::register_class<godot::FarsiReshaper>();
}
