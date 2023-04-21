from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps
from conan.tools import files
import os   

required_conan_version = ">=1.45.0"

class GTlabLoggingConan(ConanFile):
    name = "gtlab-logging"
    version = "4.1.0"
    license = "GTlab license"
    author = "Martin Siggel <martin.siggel@dlr.de>"
    url = "https://gitlab.dlr.de/at-twk/gtlab-logging"
    homepage = "https://gitlab.dlr.de/at-twk/gtlab-logging"
    toppics = "logging", "qt"
    description = "Simple logging interface"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False], "build": ["stable", "unstable"]}
    default_options = {"shared": True, "fPIC": True, "build": "unstable"}
    exports_sources = "src/*", "README.md"

    def generate(self):
        CMakeToolchain(self).generate()
        CMakeDeps(self).generate()
  
    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def build(self):    
        cmake = CMake(self)
        cmake.configure(build_script_folder="src")
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

        files.rmdir(self, os.path.join(self.package_folder, "lib", "cmake"))
        files.copy(self, "README.md", dst=os.path.join(self.package_folder, "licenses"), src=self.source_folder)

    def package_info(self):
        self.cpp_info.libs = ["GTlabLogging"]
        
        self.cpp_info.includedirs.append(os.path.join("include", "logging"))

        if self.settings.build_type != "Debug":
            self.cpp_info.libs = ['GTlabLogging']
        else:
            self.cpp_info.libs = ['GTlabLogging-d']

        self.cpp_info.set_property("cmake_file_name", "GTlabLogging")
        self.cpp_info.set_property("cmake_target_name", "GTlab::Logging")
