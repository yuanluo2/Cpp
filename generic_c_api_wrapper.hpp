#include <string>
#include <format>

/* 
  directly use c apis.
*/
template<typename CFunc, typename ErrorCondition, typename ErrorMsgGen, typename ... Args>
decltype(auto) my(CFunc cFunc, ErrorCondition errorCond, ErrorMsgGen errorMsgGen, Args ... args) {
	auto result = cFunc(std::forward<Args>(args)...);

	if (errorCond(result)) {
		throw std::runtime_error{ errorMsgGen() };
	}

	return result;
}


/* 
  c apis wrapper class.
*/
template<typename CFunction, typename ErrorCond, typename ErrorMsgGen>
class CWrapper {
	CFunction    c_func;
	ErrorCond    error_condition;
	ErrorMsgGen  error_msg_gen;
public:
	explicit CWrapper(CFunction _c_func_, ErrorCond _error_condition_, ErrorMsgGen _error_msg_gen_) :
		c_func          { _c_func_ },
		error_condition { _error_condition_ },
		error_msg_gen   { _error_msg_gen_ }
	{}

	template<typename ... Args>
	decltype(auto) operator()(Args ... args) {
		auto result = c_func(std::forward<Args>(args)...);

		if (error_condition(result)) {
			throw std::runtime_error{ error_msg_gen() };
		}

		return result;
	}
};

template<typename CFunction, typename ErrorCond, typename ErrorMsgGen>
decltype(auto) make_c_wrapper(CFunction _c_func_, ErrorCond _error_condition_, ErrorMsgGen _error_msg_gen_) {
	return CWrapper<CFunction, ErrorCond, ErrorMsgGen>{ _c_func_, _error_condition_, _error_msg_gen_ };
}


/*
  some extra useful tools. you can customize them by yourself.
*/
class ErrorMsg {
	std::string funcName;
public:
	ErrorMsg(std::string _func_name_) : funcName{ _func_name_ } {}
	const std::string& operator()() { return std::format("{} failed(). error code is {}.", funcName, WSAGetLastError()); }
};

auto not_0          = [](auto&& i) { return i != 0; };
auto lt_0           = [](auto&& i) { return i < 0; };
auto eq_nullptr     = [](auto&& i) { return i == nullptr; };
auto invalid_socket = [](auto&& i){ return i == INVALID_SOCKET; };


/*
  usage:
  on windows platform, if we want to use socket api, we have to use WSAStartup() first, otherwise, we would get an error code: 10093
  
  directly use c apis:
  
  try{
    SOCKET sock = my(socket, invalid_socket, ErrorMsg("socket"), AF_INET, SOCK_STREAM, IPPROTO_TCP);
  }
  catch(const std::exception& e){
    std::cerr << e.what() << "\n";
  }
  
  
  c api wrapper class:
  
  try{
    auto my_socket = make_c_wrapper(socket, invalid_socket, ErrorMsg("socket"));
    
    my_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  }
  catch(const std::exception& e){
    std::cerr << e.what() << "\n";
  }
  
*/
