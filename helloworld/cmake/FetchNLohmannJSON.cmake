#[======================================================================[.rst:

Fetch nlohmann_json
~~~~~~~~~~~~~~~~~~~

Fetches nlohman_json single header library from the local filesystem.

Targets
-------

``nlohmann_json::nlohmann_json``
    The header only json library. Include in source with 
    ``#include <nlohmann_json/json.hpp>``

Links
-----

* https://github.com/nlohmann/json
#]======================================================================]

set(_nljsonVersion v3.7.3)
set(_nljsonDir "${CMAKE_CURRENT_LIST_DIR}/../thirdparty/nlohmann_json-${_nljsonVersion}")

# Configure the header only library
add_library(nlohmann_json INTERFACE)
add_library(nlohmann_json::nlohmann_json ALIAS nlohmann_json)
target_include_directories(nlohmann_json INTERFACE ${_nljsonDir}/include)
