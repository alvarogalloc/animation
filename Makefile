
# make the action to run cppcheck and another for clang-format with microsoft	style
# cppcheck will use the compile_commands.json file to check the code

format:
	@clang-format -i src/*.cpp src/*.mpp src/core/*.mpp src/ext/*.mpp

check:
	@cppcheck --enable=style,performance,warning,portability --inconclusive src/*.mpp src/*.cpp --std=c++20 --language=c++ --suppress=cppcheckError --suppress=internalAstError --suppress=unmatchedSuppression --suppress=passedByValue --suppress=syntaxError --suppress=preprocessorErrorDirective
