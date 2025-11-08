import platform
from enum import Enum
from typing import Optional, Dict
from builder import MesonBuilder
from builder_windows import MesonBuilderWindows
from builder_linux import MesonBuilderLinux


class OperatingSystem(Enum):
    WINDOWS = "windows"
    LINUX = "linux"
    DARWIN = "darwin"


class MesonBuilderFactory:
    _SYSTEM_MAP: Dict[str, OperatingSystem] = {
        OperatingSystem.WINDOWS.value: OperatingSystem.WINDOWS,
        OperatingSystem.LINUX.value: OperatingSystem.LINUX,
        OperatingSystem.DARWIN.value: OperatingSystem.DARWIN
    }

    @staticmethod
    def create_builder(project_root: str, build_dir: Optional[str] = None, install_dir: Optional[str] = None,
                       target_os: Optional[OperatingSystem] = None) -> MesonBuilder:
        """
        Create a MesonBuilder for the specified or detected OS.

        Args:
            project_root: Root directory of the project
            build_dir: Build directory (optional)
            install_dir: Install directory (optional)
            target_os: Target OS (OperatingSystem enum). If None, detects current OS

        Returns:
            Appropriate MesonBuilder instance

        Raises:
            ValueError: If target_os is unsupported or path_finder is missing for Windows
        """
        if not project_root:
            raise ValueError("project_root cannot be empty")

        if target_os is None:
            target_os = MesonBuilderFactory._detect_os()

        if target_os == OperatingSystem.WINDOWS:
            return MesonBuilderWindows(project_root, build_dir, install_dir)
        elif target_os in (OperatingSystem.LINUX, OperatingSystem.DARWIN):
            return MesonBuilderLinux(project_root, build_dir, install_dir)
        else:
            raise ValueError(f"Unsupported target OS: {target_os}")

    @staticmethod
    def _detect_os() -> OperatingSystem:
        """Detect the current operating system."""
        system: str = platform.system().lower()

        detected_os = MesonBuilderFactory._SYSTEM_MAP.get(system)
        if detected_os is None:
            raise ValueError(f"Unsupported OS: {system}")

        return detected_os