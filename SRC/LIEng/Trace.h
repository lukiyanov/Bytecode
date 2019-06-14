#pragma once
#include <string>
#include <iostream>

void Trace			 (const std::string& str, std::ostream& out = std::cout);
void EnableTrace	 (const bool bEnable);
bool GetTraceEnabled ();