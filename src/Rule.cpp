#include "Rule.h"

using namespace godot;

void Rule::_bind_methods()
{
}

Rule::Rule()
{
	letter = "";
	results = PackedStringArray();
}

Rule::Rule(String l, PackedStringArray r)
{
	letter = l;
	results = r;
}


String Rule::get_letter() const
{
	return letter;
}

void Rule::set_letter(const String &l)
{
	letter = l;
}

PackedStringArray Rule::get_results() const
{
	return results;
}


String Rule::get_random_result()
{
	if (results.size() > 0)
	{
		
		rand.randomize();
		return results[rand.randi() % results.size()];
	}
	return "";
}

void Rule::set_results(const PackedStringArray& r)
{
	results = r;
}

void Rule::add_result(const String& r)
{
	results.append(r);
}