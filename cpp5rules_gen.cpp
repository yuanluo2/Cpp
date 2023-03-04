#include <iostream>
#include <string>
#include <format>

/*
	clang++ cpp5rules_gen.cpp -std=c++20 -o gen5rules.exe
*/
int main(int argc, char* argv[]){
	if (argc != 2){
		std::cout << std::format("usage: {} <class_name>", argv[0]);
		return 1;
	}
	
	std::string className = argv[1];
	std::string str;
	
	str += std::format("\n");
	str += std::format("class {0} {{\n", className);
	str += std::format("    \n");
	str += std::format("public:\n");
	str += std::format("    explicit {0}(){{}}\n", className);
	str += std::format("\n");
	str += std::format("    explicit {0}(const {0}&) = delete;\n", className);
	str += std::format("    {0}& operator=(const {0}&) = delete;\n", className);
	str += std::format("\n");
	str += std::format("    explicit {0}({0}&&) noexcept {{\n", className);
	str += std::format("        std::swap(, );\n");
	str += std::format("    }}\n");
	str += std::format("\n");
	str += std::format("    {0}& operator=({0}&&) noexcept {{\n", className);
	str += std::format("        std::swap(, );\n");
	str += std::format("        return *this;\n");
	str += std::format("    }}\n");
	str += std::format("\n");
	str += std::format("    ~{0}() noexcept {{\n", className);
	str += std::format("        \n");
	str += std::format("    }}\n");
	str += std::format("}};\n");
	
	std::cout << str;
	return 0;
}
