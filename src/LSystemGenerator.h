#ifndef LSYSTEMGENERATOR_H
#define LSYSTEMGENERATOR_H

#include <godot_cpp/classes/tile_map.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/char_utils.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/packed_data_container.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "Rule.h"

namespace godot 
{

	class LSystemGenerator : public Node2D {
		GDCLASS(LSystemGenerator, Node2D)

	private:

		String rootSentence;
		int iterationLimit;
		std::vector<Rule> rules;

		String separator1;
		String separator2;

		PackedStringArray rulesString;

	protected:
		static void _bind_methods();

	public:
		LSystemGenerator();

		void set_rules(const PackedStringArray& r);
		PackedStringArray get_rules() const;

		void set_root_sentence(const String& r);
		String get_root_sentence() const;

		void set_iteration_limit(const int& l);
		int get_itaration_limit() const;

		void set_separator1(const String& r);
		String get_separator1() const;

		void set_separator2(const String& r);
		String get_separator2() const;
		

		String generateSentence(String word = "");
		String growRecursive(String word, int curIteration);
		String processRulesRecursivelly(String newWord, String c, int curIteration); 

	};

}

#endif