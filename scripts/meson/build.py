import sys
from pathlib import Path

project_root = str(Path(__file__).resolve().parents[2])
sys.path.insert(0, project_root)

from scripts.meson.builder_factory import MesonBuilderFactory, OperatingSystem

class ArgParser:
    def __init__(self):
        import argparse
        self.parser = argparse.ArgumentParser(description="Meson build script with Visual Studio environment.")
        self.parser.add_argument('--clean', action='store_true',
        help='Remove build and install directories before building (clean build)')
        self.parser.add_argument('--build-dir', type=str, default=None,
                               help=f'Build directory path (default: project_root/build)')
        self.parser.add_argument('--install-dir', type=str, default=None,
                               help=f'Install directory path (default: project_root/install/bin)')
        self.parser.add_argument('--os', type=str, choices=[os.value for os in OperatingSystem],
                               help='Target operating system (windows, linux, darwin). If not specified, auto-detects current OS')

    def parse(self):
        return self.parser.parse_args()

def main():
    args = ArgParser().parse()

    builder = MesonBuilderFactory().create_builder(
        project_root=project_root,
        build_dir=args.build_dir,
        install_dir=args.install_dir,
        target_os=OperatingSystem(args.os) if args.os else None
    )
    builder.run(clean=args.clean)

if __name__ == "__main__":
    try:
        main()
    except Exception as ex:
        print(f"Fatal error: {ex}")
        sys.exit(1)