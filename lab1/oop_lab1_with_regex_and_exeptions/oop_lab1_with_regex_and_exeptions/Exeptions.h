#pragma once
#include <exception>
#include <stdexcept>
#include <vector> 

class exc_compile final : public std::exception {
public:
	typedef std::vector<std::pair<int, std::string>> DataType;
	typedef std::exception BaseClass;
	explicit exc_compile(DataType && errors) noexcept : BaseClass("Compile error"), _errors(std::move(errors)) {};
	explicit exc_compile(DataType const& errors) : BaseClass("Compile error"), _errors(errors) {};
	const DataType& getData() {
		return _errors;
	}
private:
	DataType _errors;
};

class exc_io final : public std::runtime_error {
public:
	typedef std::runtime_error BaseClass;
	explicit exc_io(const std::string& _Message) noexcept : BaseClass(_Message.c_str()) {}
	explicit exc_io(const char *_Message) noexcept : BaseClass(_Message) {}
};

class exc_access final : std::runtime_error {
	using std::runtime_error::runtime_error;
};

class exc_search final : std::runtime_error {
	using std::runtime_error::runtime_error;
};

class exc_cast final : std::runtime_error {
	using std::runtime_error::runtime_error;
};
