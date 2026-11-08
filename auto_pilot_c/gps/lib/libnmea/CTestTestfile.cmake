# CMake generated Testfile for 
# Source directory: /opt/auto_pilot_c/lib/libnmea
# Build directory: /opt/auto_pilot_c/lib/libnmea
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(utests "/opt/auto_pilot_c/lib/libnmea/bin/utests")
set_tests_properties(utests PROPERTIES  ENVIRONMENT "NMEA_PARSER_PATH=/opt/auto_pilot_c/lib/libnmea/parsers/" _BACKTRACE_TRIPLES "/opt/auto_pilot_c/lib/libnmea/CMakeLists.txt;248;add_test;/opt/auto_pilot_c/lib/libnmea/CMakeLists.txt;0;")
add_test(utests-parse "/opt/auto_pilot_c/lib/libnmea/bin/utests-parse")
set_tests_properties(utests-parse PROPERTIES  ENVIRONMENT "NMEA_PARSER_PATH=/opt/auto_pilot_c/lib/libnmea/parsers/" _BACKTRACE_TRIPLES "/opt/auto_pilot_c/lib/libnmea/CMakeLists.txt;248;add_test;/opt/auto_pilot_c/lib/libnmea/CMakeLists.txt;0;")
add_test(utests-nmea "/opt/auto_pilot_c/lib/libnmea/bin/utests-nmea")
set_tests_properties(utests-nmea PROPERTIES  ENVIRONMENT "NMEA_PARSER_PATH=/opt/auto_pilot_c/lib/libnmea/parsers/" _BACKTRACE_TRIPLES "/opt/auto_pilot_c/lib/libnmea/CMakeLists.txt;248;add_test;/opt/auto_pilot_c/lib/libnmea/CMakeLists.txt;0;")
