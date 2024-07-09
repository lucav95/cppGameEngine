#pragma once

#include <string>

class Action {
	std::string			m_name = "NONE";
	std::string			m_type = "NONE"; // "START" / "END"

public:
	Action();
	Action(const std::string& name, const std::string& type);

	const std::string&	getName() const;
	const std::string&	getType() const;
	std::string			toString() const;
};