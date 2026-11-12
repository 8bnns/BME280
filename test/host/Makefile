# Host-side unit tests: build the library against stub Arduino headers
# and run it against a simulated BME280/BMP280.
CXX      ?= g++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -Wno-unused-parameter -Werror
SRC_DIR  := ../../src
SRCS     := test_bme280.cpp $(SRC_DIR)/BME280.cpp $(SRC_DIR)/BME280I2C.cpp \
            $(SRC_DIR)/BME280Spi.cpp $(SRC_DIR)/BME280SpiSw.cpp \
            $(SRC_DIR)/EnvironmentCalculations.cpp

test: test_bme280
	./test_bme280

test_bme280: $(SRCS) stubs/*.h
	$(CXX) $(CXXFLAGS) -Istubs -I$(SRC_DIR) $(SRCS) -o $@

clean:
	rm -f test_bme280

.PHONY: test clean
