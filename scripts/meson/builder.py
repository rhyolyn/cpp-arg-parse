import os
import shutil
from abc import ABC, abstractmethod
from enum import Enum
from typing import Dict, List, Optional


class MesonBuilder(ABC):
    """Abstract base class for Meson build system implementations across different platforms."""

    BUILDER: str = "meson"
    DEFAULT_BUILD_DIR: str = "build"
    DEFAULT_INSTALL_DIR: str = f"install{os.sep}bin"

    class Command(Enum):
        SETUP = "setup"
        COMPILE = "compile"
        TEST = "test"
        INSTALL = "install"

    def __init__(self, project_root: str, build_dir: Optional[str] = None, install_dir: Optional[str] = None) -> None:
        """Initialize the Meson builder.

        Args:
            project_root: Root directory of the project
            build_dir: Build directory path (defaults to project_root/build)
            install_dir: Install directory path (defaults to project_root/install/bin)
        """
        self._project_root = os.path.abspath(project_root)
        self._build_dir = build_dir or self._get_default_build_dir()
        self._install_dir = install_dir or self._get_default_install_dir()

        os.makedirs(self._build_dir, exist_ok=True)
        self._commands: Dict[MesonBuilder.Command, List[str]] = self._build_command_dict()

    def _get_default_build_dir(self) -> str:
        return os.path.join(self._project_root, self.DEFAULT_BUILD_DIR)

    def _get_default_install_dir(self) -> str:
        return os.path.join(self._project_root, self.DEFAULT_INSTALL_DIR)

    def _build_command_dict(self) -> Dict['MesonBuilder.Command', List[str]]:
        return {
            self.Command.SETUP: [self.BUILDER, self.Command.SETUP.value, self._build_dir, "--prefix", self._install_dir],
            self.Command.COMPILE: [self.BUILDER, self.Command.COMPILE.value, "-C", self._build_dir],
            self.Command.TEST: [self.BUILDER, self.Command.TEST.value, "-C", self._build_dir],
            self.Command.INSTALL: [self.BUILDER, self.Command.INSTALL.value, "-C", self._build_dir]
        }

    def clean(self) -> None:
        """Remove build and install directories."""
        print(f"Cleaning build directory: {self._build_dir}")
        shutil.rmtree(self._build_dir, ignore_errors=True)
        print(f"Cleaning install directory: {self._install_dir}")
        shutil.rmtree(self._install_dir, ignore_errors=True)

    def run(self, clean: bool = False) -> None:
        """Run the complete build process (setup, compile, test, install).

        Args:
            clean: Whether to clean directories before building
        """
        if clean:
            self.clean()
        self._run_commands(clean)

    @abstractmethod
    def setup(self) -> None:
        """Configure the build system and prepare for compilation."""
        pass

    @abstractmethod
    def compile_code(self) -> None:
        """Compile the project source code."""
        pass

    @abstractmethod
    def test(self) -> None:
        """Run the project's test suite."""
        pass

    @abstractmethod
    def install(self) -> None:
        """Install the compiled project to the specified install directory."""
        pass

    @abstractmethod
    def _run_commands(self, clean: bool = False) -> None:
        pass

    def _get_ordered_commands(self) -> List[List[str]]:
        return [self._commands[cmd] for cmd in
                [self.Command.SETUP, self.Command.COMPILE, self.Command.TEST, self.Command.INSTALL]]

    @staticmethod
    def _get_command_string(commands: List[str]) -> str:
        return ' '.join(f'"{arg}"' if ' ' in arg else arg for arg in commands)