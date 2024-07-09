#ifndef RULE_H
#define RULE_H

#include <godot_cpp/classes/tile_map.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>


namespace godot 
{

	class Rule : public Object {
		GDCLASS(Rule, Object )
			
	private:
		String letter;
		PackedStringArray results;

		RandomNumberGenerator rand;

	protected:
		static void _bind_methods();

	public:
		Rule();
		Rule(String l, PackedStringArray r);

		String get_letter() const;
		void set_letter(const String &l);

		String get_random_result();
		PackedStringArray get_results() const;
		void set_results(const PackedStringArray& r);
		void add_result(const String& r);
	};

}

#endif