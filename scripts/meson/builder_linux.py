import subprocess
from typing import List

from builder import MesonBuilder


class MesonBuilderLinux(MesonBuilder):

    def _run_commands(self, clean: bool = False) -> None:
        commands = self._get_ordered_commands()
        for command in commands:
            self._run_command(command)

    def setup(self) -> None:
        self._run_command(self._commands[self.Command.SETUP])

    def compile_code(self) -> None:
        self._run_command(self._commands[self.Command.COMPILE])

    def test(self) -> None:
        self._run_command(self._commands[self.Command.TEST])

    def install(self) -> None:
        self._run_command(self._commands[self.Command.INSTALL])

    def _run_command(self, command: List[str]) -> None:
        command_string = self._get_command_string(command)
        print(f"Running: {command_string}")
        subprocess.check_call(command)