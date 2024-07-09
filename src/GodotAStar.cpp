#include "GodotAStar.h"

using namespace godot;

void GodotAStar::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("findPath"), &GodotAStar::findPath);
}

void GodotAStar::_ready()
{
	Node2D* sn = get_node<Node2D>("startNode");
	Node2D* tn = get_node<Node2D>("endNode");
	NavigationAgent2D* na = get_node<NavigationAgent2D>("startNode/NavigationAgent2D");
	if (sn)
		startNode = sn;
	if (tn)
		targetNode = tn;
	if (na)
		navAgent = na;
}


PackedVector2Array GodotAStar::findPath(Vector2 start, Vector2 finish)
{

	if (!(startNode && targetNode && navAgent)) return PackedVector2Array();

	startNode->set_position(start);
	targetNode->set_position(finish);

	navAgent->set_target_position(targetNode->get_position());
	navAgent->get_next_path_position();
	PackedVector2Array res = navAgent->get_current_navigation_path();
	res.append(finish);
	res.append(start);

	return res;
}


