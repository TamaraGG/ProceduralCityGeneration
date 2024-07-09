#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <godot_cpp/classes/camera2d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_with_modifiers.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/display_server.hpp>


#include "MyTileMap.h"

namespace godot
{

    class MyCamera : public Camera2D {
        GDCLASS(MyCamera, Camera2D)

    private:
        bool dragging = false;
        Vector2 drag_start;
        Vector2 camera_start;
        float zoom_speed = 0.1f;
        Rect2 limits;

        bool enabled;

        Vector2 minZoom;
        Vector2 maxZoom;

    protected:
        static void _bind_methods();

    public:

        MyCamera();
        void _process(double _delta);
        void _input(const Ref<InputEvent>& event);
        void _ready();
        void set_limits(const Rect2& new_limits);
        Rect2 get_limits() const;

        void change_zoom(Vector2 newZoom);

        void turn_on();
        void turn_off();

        double get_min_zoom();
        double get_max_zoom();
    };
}
#endif