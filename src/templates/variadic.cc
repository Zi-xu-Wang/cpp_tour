#include <iostream>
#include <sstream>
#include <string>
#include <vector>
//-----------------------------------------------------
// Related to PA5
// Forward declaraion
template <typename... Args>
std::tuple<Args...> ParseStr(std::vector<std::string>& paramV);

std::vector<std::string> Split(const std::string& str) {
  std::stringstream ss(str);
  std::vector<std::string> result;
  while (ss.good()) {
    std::string substr;
    std::getline(ss, substr, ',');
    result.push_back(substr);
  }
  return result;
}

struct Op {
  virtual const char* GetName() const = 0;
  virtual void Parse(const std::string& params) = 0;
  virtual void Execute(class Machine& machine) = 0;
};

struct MovI : Op {
  const char* GetName() const override { return "movi"; }
  void Parse(const std::string& params) override {
    // Split comma-separated list
    std::vector<std::string> paramV = Split(params);
    opReg1 = paramV[0];            // This is a string
    opInt = std::stoi(paramV[1]);  // Gotta convert this to int!
  }
  void Execute(class Machine& machine) override { /* Do whatever */
  }
  // Member data for this op!
  std::string opReg1;
  int opInt;
};

template <typename... Args>
struct OpBase : Op {
  void Parse(const std::string& params) override {
    // Split comma-separated list
    std::vector<std::string> paramV = Split(params);
    std::reverse(paramV.begin(), paramV.end());
    mParameters = ParseStr<Args...>(paramV);
  }

  std::tuple<Args...> mParameters;
};

//-----------------------------------------------------
// ParseElem
template <typename T>
std::tuple<T> ParseElem(const std::string& elem) {}

// Specialized template
template <>
inline std::tuple<int> ParseElem(const std::string& elem) {
  return std::make_tuple(std::stoi(elem));
}

// Specialized template
template <>
inline std::tuple<std::string> ParseElem(const std::string& elem) {
  return std::make_tuple(elem);
}

//-----------------------------------------------------
// Forward declaration
template <typename T, typename... Args>
std::tuple<T, Args...> ParseStrHelper(std::vector<std::string>& paramV);
//-----------------------------------------------------

template <typename... Args>
std::tuple<Args...> ParseStr(std::vector<std::string>& paramV) {
  return ParseStrHelper<Args...>(paramV);
}

// Base case
template <>
std::tuple<> ParseStr(std::vector<std::string>& paramV) {
  return std::make_tuple();
}

//  Arg... = <int, float, std::string>
//  T = int, Arg... = <float, std::string>
//                   T = float, Arg... = std::string
//                                     T = std::string, Arg... = {}

//-----------------------------------------------------
template <typename T, typename... Args>
std::tuple<T, Args...> ParseStrHelper(std::vector<std::string>& paramV) {
  std::string elem = paramV.back();
  paramV.pop_back();

  return std::tuple_cat(ParseElem<T>(elem), ParseStr<Args...>(paramV));
}
//-----------------------------------------------------
struct MovI2 : OpBase<std::string, int> {
  const char* GetName() const override { return "movi"; }
  void Execute(class Machine& machine) override {
    // Still have to implement this
  }
};

int main() {
  std::vector<std::string> paramV = {"5", "r1"};
  auto t = ParseStr<std::string, int>(paramV);

  std::cout << "std::get<0>(t): " << std::get<0>(t) << std::endl;
  std::cout << "std::get<1>(t): " << std::get<1>(t) << std::endl;
  MovI2 m;
  m.Parse("r1,5");

  std::cout << "std::get<0>(m.mParameters): " << std::get<0>(m.mParameters)
            << std::endl;
  return 0;
}

// mov r1,r2,r3

// params = "r1,r2,r3" ----split----> { "r1", "r2", "r3" }

// std::vector<int>

// intput File:
// "Add r1, r2, r3"  ====> opName = "Add", params = "r1, r2, r3"
// "And r1, r2, r3"
// "Mov r1, 100"

// Split : "r1, r2, r3"  ====> {"r1", "r2", "r3" }
// ParseElem: "r1" ====> ("r1")
//          : "11" ====> (11)

// ParseString: {"r1", "r2", "r3" } ===> ParseHelper:
//                                                std::tup_cat (ParseElem("r1"),
//                                                ParseString({"r2", "r3" }) =
//                                                ("r1", "r2", "r3")