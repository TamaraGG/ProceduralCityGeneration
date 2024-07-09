#include "LSystemGenerator.h"

using namespace godot;

void LSystemGenerator::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("get_rules"), &LSystemGenerator::get_rules);
	ClassDB::bind_method(D_METHOD("set_rules", "rules"), &LSystemGenerator::set_rules);
	ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "rules"), "set_rules", "get_rules");

	ClassDB::bind_method(D_METHOD("get_root_sentence"), &LSystemGenerator::get_root_sentence);
	ClassDB::bind_method(D_METHOD("set_root_sentence", "root sentence"), &LSystemGenerator::set_root_sentence);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "root sentence"), "set_root_sentence", "get_root_sentence");

	ClassDB::bind_method(D_METHOD("get_iteration_limit"), &LSystemGenerator::get_itaration_limit);
	ClassDB::bind_method(D_METHOD("set_iteration_limit", "iteration limit"), &LSystemGenerator::set_iteration_limit);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "iteration limit", PROPERTY_HINT_RANGE, "0,,,or_greater"), "set_iteration_limit", "get_iteration_limit");

	ClassDB::bind_method(D_METHOD("get_separator1"), &LSystemGenerator::get_separator1);
	ClassDB::bind_method(D_METHOD("set_separator1", "letter and rules separator"), &LSystemGenerator::set_separator1);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "letter and rules separator"), "set_separator1", "get_separator1");

	ClassDB::bind_method(D_METHOD("get_separator2"), &LSystemGenerator::get_separator2);
	ClassDB::bind_method(D_METHOD("set_separator2", "rules separator"), &LSystemGenerator::set_separator2);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "rules separator"), "set_separator2", "get_separator2");
}

LSystemGenerator::LSystemGenerator()
{
	rootSentence = "";
	iterationLimit = 0;
	rulesString = PackedStringArray();

	separator1 = "->";
	separator2 = ",";
}

void LSystemGenerator::set_root_sentence(const String& r)
{
	rootSentence = r;
}

String LSystemGenerator::get_root_sentence() const
{
	return rootSentence;
}

void LSystemGenerator::set_iteration_limit(const int& l)
{
	iterationLimit = l;

}

int LSystemGenerator::get_itaration_limit() const
{
	return iterationLimit;
}

void LSystemGenerator::set_separator1(const String& r)
{
	separator1 = r;
}

String LSystemGenerator::get_separator1() const
{
	return separator1;
}

void LSystemGenerator::set_separator2(const String& r)
{
	separator2 = r;
}

String LSystemGenerator::get_separator2() const
{
	return separator2;
}



void LSystemGenerator::set_rules(const PackedStringArray& r)
{
	rules.clear();
	//String separator1 = "->";
	//String separator2 = ", ";

	String curRuleStr;
	PackedStringArray curRuleArray;

	for (int i = 0; i < r.size(); i++)
	{
		curRuleStr = r[i];
		if (curRuleStr.count(separator1) == 1)
		{
			curRuleArray = curRuleStr.split(separator1);
			String letter = curRuleArray[0];
			PackedStringArray results = curRuleArray[1].split(separator2);
			rules.push_back(Rule(letter, results));
		}
	}
	std::cout << "\n\nim here\n\n";
	rulesString = r;
}

PackedStringArray LSystemGenerator::get_rules() const
{
	return rulesString;
}


String LSystemGenerator::generateSentence(String word)
{
	if (word.length() == 0)
	{
		word = rootSentence;
	}
	return growRecursive(word, 0);
}

String LSystemGenerator::growRecursive(String word, int curIteration)
{
	if (curIteration >= iterationLimit)
	{
		return word;
	}

	String newWord;
	PackedStringArray wordLetters = word.split();

	for (int i = 0; i < wordLetters.size(); i++)
	{
		newWord += wordLetters[i];
		newWord = processRulesRecursivelly(newWord, wordLetters[i], curIteration);
	}
	
	return newWord;
}

String LSystemGenerator::processRulesRecursivelly(String newWord, String c, int curIteration)
{
	for (int i = 0; i < rules.size(); i++)
	{
		if (rules[i].get_letter() == c)
		{
			newWord += growRecursive(rules[i].get_random_result(), curIteration + 1);
		}
	}
	return newWord;
}


