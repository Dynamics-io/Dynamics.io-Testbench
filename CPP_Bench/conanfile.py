from conans import ConanFile

class Dynamics_IO(ConanFile):
    name = "Dynamics.IO-Testbench"
    version = "0.1"
    license = "MIT"
    description = "CPP Test bench for Dynamics.IO"

    requires = (
        "spdlog/1.8.0",
        "boost/1.75.0",
        "glm/0.9.9.8",
        #"vk-bootstrap/0.4",
        "opencl-headers/2020.06.16"
    )

    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = "*", "!*build/*"

    def build(self):
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy('*.so*', dst='lib', src='lib')
