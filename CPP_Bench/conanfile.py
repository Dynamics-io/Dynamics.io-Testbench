from conans import ConanFile

class Dynamics_IO(ConanFile):
    name = "Dynamics.IO-Testbench"
    version = "0.1"
    license = "MIT"
    description = "CPP Test bench for Dynamics.IO"

    requires = (
        "spdlog/1.8.0",
        "boost/1.81.0",
        "glm/cci.20230113",
        #"vk-bootstrap/0.4",
        "opencl-headers/2022.09.30",
        "vulkan-headers/1.3.239.0",
        "vulkan-memory-allocator/3.0.1",
        "glfw/3.3.8"
    )

    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = "*", "!*build/*"

    def build(self):
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy('*.so*', dst='lib', src='lib')
