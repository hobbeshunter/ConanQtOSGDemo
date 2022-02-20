from conans import ConanFile, tools, CMake, RunEnvironment

class ConanQTOSGDemo(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "qt", "cmake", "cmake_find_package", "cmake_paths"
    requires = "qt/[~=6.1]", "fontconfig/system@mwudenka/snapshot", "openscenegraph/[~=3.6]", "eigen/[~=3.3]"

    # def build_requirements(self):
    #     self.build_requires("cmake/3.20.0")

    def configure(self):
        tools.check_min_cppstd(self, "17")

        self.options["qt"].shared = True
        self.options["qt"].opengl = "desktop"
        # self.options["qt"].gui = True
        # self.options["qt"].with_vulkan = False
        # self.options["qt"].openssl = False
        # self.options["qt"].with_glib = False
        # self.options["qt"].with_freetype = False
        # self.options["qt"].with_fontconfig = True
        # self.options["qt"].with_icu = False
        # self.options["qt"].with_harfbuzz = True
        # self.options["qt"].with_libjpeg = False
        # self.options["qt"].with_libpng = False
        # self.options["qt"].with_sqlite3 = False
        # self.options["qt"].with_mysql = False
        # self.options["qt"].with_pq = False
        # self.options["qt"].with_odbc = False
        # self.options["qt"].with_zstd = False
        # self.options["qt"].with_brotli = False
        # self.options["qt"].gui = True
        # self.options["qt"].widgets = True
        # self.options["qt"].qtwayland = True
        # self.options["qt"].qtdeclarative = True
        # self.options["qt"].qtsvg = True

    def _configure_cmake(self):
        cmake = CMake(self)

        for key, value in self.options["openscenegraph"].items():
            if key.startswith("with_"):
                cmake.definitions["OSG_HAS_" + key.upper()] = 1 if value else 0
        if self.settings.os == "Macos":
            cmake.definitions["OSG_HAS_WITH_GIF"] = 0
            cmake.definitions["OSG_HAS_WITH_JPEG"] = 0
            cmake.definitions["OSG_HAS_WITH_PNG"] = 0

        cmake.configure()
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()
