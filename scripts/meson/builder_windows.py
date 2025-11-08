from builder import MesonBuilder
from scripts.meson.path_finder import PathFinder


class MesonBuilderWindows(MesonBuilder):

    def _run_commands(self, clean: bool = False) -> None:
        commands = self._get_ordered_commands()
        command_strings = [self._get_command_string(command) for command in commands]
        PathFinder.run_in_vs_env(command_strings)

    def setup(self) -> None:
        command_string = self._get_command_string(self._commands[self.Command.SETUP])
        PathFinder.run_in_vs_env([command_string])

    def compile_code(self) -> None:
        command_string = self._get_command_string(self._commands[self.Command.COMPILE])
        PathFinder.run_in_vs_env([command_string])

    def test(self) -> None:
        command_string = self._get_command_string(self._commands[self.Command.TEST])
        PathFinder.run_in_vs_env([command_string])

    def install(self) -> None:
        command_string = self._get_command_string(self._commands[self.Command.INSTALL])
        PathFinder.run_in_vs_env([command_string])