#include "MyCamera.h"
#include <godot_cpp/classes/remote_transform2d.hpp>

using namespace godot;


void MyCamera::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_limits", "limits"), &MyCamera::set_limits);
    ClassDB::bind_method(D_METHOD("turn_on"), &MyCamera::turn_on);
    ClassDB::bind_method(D_METHOD("turn_off"), &MyCamera::turn_off);
    ClassDB::bind_method(D_METHOD("change_zoom", "new_zoom"), &MyCamera::change_zoom);

    ClassDB::bind_method(D_METHOD("get_min_zoom"), &MyCamera::get_min_zoom);
    ClassDB::bind_method(D_METHOD("get_max_zoom"), &MyCamera::get_max_zoom);

    ADD_SIGNAL(MethodInfo("zoom_changed", PropertyInfo(Variant::FLOAT, "new_zoom")));
}

void MyCamera::_input(const Ref<InputEvent>& event) {
    
    if (!enabled) return;

    Ref<InputEventMouseMotion> mouse_motion_event = event;
    Ref<InputEventMouseButton> mouse_button_event = event;

    if (mouse_button_event.is_valid()) 
    {
        if (mouse_button_event->is_pressed() && mouse_button_event->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT) {
            dragging = true;
            drag_start =  get_local_mouse_position();
            camera_start =  get_position();
            UtilityFunctions::print(get_zoom(), " ", get_local_mouse_position());
        }
        else if (!mouse_button_event->is_pressed() && mouse_button_event->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT) {
            dragging = false;
        }
        else if (mouse_button_event->is_pressed() && mouse_button_event->get_button_index() == MouseButton::MOUSE_BUTTON_WHEEL_UP) {
            change_zoom(get_zoom() * Vector2(1.04, 1.04));
            emit_signal("zoom_changed", get_zoom().x);
        }
        else if (mouse_button_event->is_pressed() && mouse_button_event->get_button_index() == MouseButton::MOUSE_BUTTON_WHEEL_DOWN) {
           
           // if (get_zoom() * Vector2(0.94, 0.94) >= maxZoom)
            //if (limits.size.width * (get_zoom() - Vector2(0.04, 0.04)).x <= get_viewport_rect().size.width)
            //if (1 / (get_zoom() - Vector2(0.04, 0.04)).x * DisplayServer::get_singleton()->window_get_size().width
            //    <= get_limit(SIDE_RIGHT) - get_limit(SIDE_LEFT) &&
            //    1 / (get_zoom() - Vector2(0.04, 0.04)).y * DisplayServer::get_singleton()->window_get_size().height
            //    <= get_limit(SIDE_BOTTOM) - get_limit(SIDE_TOP))
            //{
                change_zoom(get_zoom() * Vector2(0.94, 0.94));
                emit_signal("zoom_changed", get_zoom().x);
            //}
           /// else
           /// {
           //     change_zoom(maxZoom);
           // }
            //Transform2D tr = get_transform();

            //UtilityFunctions::print("the scale is : ", tr.get_scale(), " the zoom is : ", get_zoom());
        }
    }
   
    //if (dragging && mouse_motion_event.is_valid()) {
    //    Vector2 offset = mouse_motion_event->get_position() - drag_start;
    //    Vector2 new_position = camera_start - offset * get_zoom().x;
    //    new_position = new_position.clamp(limits.position, limits.position + limits.size);
    //    set_position(new_position);
    //}
    
}

void MyCamera::_ready()
{
    enabled = true;
    
    MyTileMap* mtm = get_node<MyTileMap>("../../MainScene/MyTileMap");
    if (mtm)
    {
        Rect2i mapLimits = DisplayServer::get_singleton()->screen_get_usable_rect();
        Vector2 limSize = mtm->map_to_local(mapLimits.get_size());
        Vector2 limPos = mtm->map_to_local(mapLimits.get_position());
        limits = Rect2(limPos - limSize * 1000, limSize * 2001);
        Vector2 viewSize = DisplayServer::get_singleton()->screen_get_size();

        minZoom = Vector2(1,1) / 1.9 * MAX(viewSize.width / limits.size.width, viewSize.height / limits.size.height);
        
        maxZoom = Vector2(4,4);

        set_limit(SIDE_LEFT, limits.position.x);
        set_limit(SIDE_RIGHT, limits.position.x + limits.size.width);
        set_limit(SIDE_TOP, limits.position.y);
        set_limit(SIDE_BOTTOM, limits.position.y + limits.size.height);
    }
    else
    {
        minZoom = Vector2(0.000001, 0.000001);
        maxZoom = Vector2(2, 2);
    }
   
    make_current();
    change_zoom(Vector2(1, 1) * 0.3);
   
    emit_signal("zoom_changed", get_zoom().x);
}

void MyCamera::set_limits(const Rect2& new_limits)
{
    limits = new_limits;
    
}

Rect2 godot::MyCamera::get_limits() const
{
    return limits;
}

void MyCamera::change_zoom(Vector2 newZoom)
{
    if (newZoom < minZoom)
    {
        set_zoom(minZoom);
    }
    if (newZoom > maxZoom)
    {
        set_zoom(maxZoom);
    }
    else
    {
        set_zoom(newZoom);
    }
    
}

void MyCamera::turn_on()
{
    enabled = true;
}

void MyCamera::turn_off()
{
    enabled = false;
    dragging = false;
}

double MyCamera::get_min_zoom()
{
    return minZoom.x;
}

double MyCamera::get_max_zoom()
{
    return maxZoom.x;
}

MyCamera::MyCamera()
{
    minZoom = Vector2(0.000001, 0.000001);
    maxZoom = Vector2(2, 2);
    change_zoom(Vector2(1, 1) * 0.3);
}

void MyCamera::_process(double _delta)
{
    if (dragging)
    {
        set_position((camera_start + drag_start) - get_local_mouse_position()); 
    }
}