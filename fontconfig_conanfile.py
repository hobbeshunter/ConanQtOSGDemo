from conans import ConanFile, tools
from conans.errors import ConanException, ConanInvalidConfiguration

required_conan_version = ">=1.32"


class FontconfigConan(ConanFile):
    name = "fontconfig"
    license = "MIT"
    url = "https://github.com/conan-io/conan-center-index"
    description = "Fontconfig is a library for configuring and customizing font access"
    homepage = "https://gitlab.freedesktop.org/fontconfig/fontconfig"
    topics = ("conan", "fontconfig", "fonts", "freedesktop")
    settings = "os"

    def configure(self):
        if self.settings.os not in ["Linux"]:
            raise ConanInvalidConfiguration(
                "This recipe supports only Linux.")

    def package_id(self):
        self.info.header_only()

    def system_requirements(self):
        packages = []
        if tools.os_info.is_linux and self.settings.os == "Linux":
            if tools.os_info.with_apt:
                packages = ["libfontconfig1", "fontconfig-config",
                            "libfontconfig-dev", "fontconfig-dbgsym", "libfontconfig1-dbgsym"]
            else:
                self.output.warn("Do not know how to install 'fontconfig' for {}.".format(
                    tools.os_info.linux_distro))

        if packages:
            package_tool = tools.SystemPackageTool(
                conanfile=self, default_mode="verify")
            package_tool.install_packages(update=True, packages=packages)

    def package_info(self):
        pkg_config = tools.PkgConfig("fontconfig")
        if not pkg_config.provides:
            raise ConanException(
                "Fontconfig development files aren't available, give up")
        libs = [lib[2:] for lib in pkg_config.libs_only_l]
        lib_dirs = [lib[2:] for lib in pkg_config.libs_only_L]
        ldflags = [flag for flag in pkg_config.libs_only_other]
        include_dirs = [include[2:] for include in pkg_config.cflags_only_I]
        cflags = [
            flag for flag in pkg_config.cflags_only_other if not flag.startswith("-D")]
        defines = [flag[2:]
                   for flag in pkg_config.cflags_only_other if flag.startswith("-D")]

        self.cpp_info.system_libs = libs
        self.cpp_info.libdirs = lib_dirs
        self.cpp_info.sharedlinkflags = ldflags
        self.cpp_info.exelinkflags = ldflags
        self.cpp_info.defines = defines
        self.cpp_info.includedirs = include_dirs
        self.cpp_info.cflags = cflags
        self.cpp_info.cxxflags = cflags
        self.cpp_info.version = pkg_config.version[0]

