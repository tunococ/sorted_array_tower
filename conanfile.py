from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeDeps, CMakeToolchain, CMake


class SortedArrayTowerConan(ConanFile):
    name = "sorted_array_tower"
    version = "1.0.0"
    
    # Metadata
    description = "A C++ library for efficient sorted array operations"
    author = "Your Name"
    license = "MIT"
    url = "https://github.com/yourusername/sorted_array_tower"
    
    # Settings
    settings = "os", "compiler", "build_type", "arch"
    
    # Requirements
    requires = (
        "doctest/[>=2.5.2]",
    )

    # Options
    options = {
        "use_modules": [True, False],
    }
    default_options = {
        "use_modules": True,
    }

    def layout(self):
        cmake_layout(self, build_folder="build")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["SAT_USE_MODULES"] = self.options.use_modules
        tc.cache_variables["CMAKE_CXX_STANDARD"] = "20"
        tc.cache_variables["CMAKE_CXX_STANDARD_REQUIRED"] = "ON"
        tc.cache_variables["CMAKE_CXX_EXTENSIONS"] = "OFF"
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure(variables={
            "SAT_USE_MODULES": self.options.use_modules,
        })
        cmake.build()

    def test(self):
        cmake = CMake(self)
        cmake.ctest()

    def package(self):
        cmake = CMake(self)
        cmake.install()
    
    def package_info(self):
        self.cpp_info.libs = ["sorted_array_tower"]
        self.cpp_info.set_property("cmake_find_mode", "config")
        self.cpp_info.set_property("cmake_file_name", "sorted_array_tower")
        self.cpp_info.set_property("cmake_target_name", "sorted_array_tower::sorted_array_tower")
