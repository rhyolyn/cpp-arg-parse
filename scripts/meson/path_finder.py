import os
import shutil
import subprocess

class PathFinder:
    @staticmethod
    def find_vswhere():
        program_files_x86 = os.environ.get("ProgramFiles(x86)", r"C:\Program Files (x86)")
        vswhere_path = os.path.join(program_files_x86, "Microsoft Visual Studio", "Installer", "vswhere.exe")
        if os.path.isfile(vswhere_path):
            return vswhere_path
        return shutil.which("vswhere.exe")

    @staticmethod
    def find_vcvarsall():
        vswhere = PathFinder.find_vswhere()
        if not vswhere:
            raise RuntimeError("vswhere.exe not found. Please ensure Visual Studio is installed.")
        result = subprocess.run([
            vswhere,
            "-latest",
            "-products", "*",
            "-requires", "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
            "-property", "installationPath"
        ], stdout=subprocess.PIPE, check=True, encoding="utf-8")
        install_path = result.stdout.strip()
        if not install_path:
            raise RuntimeError("No suitable Visual Studio installation found.")
        vcvarsall = os.path.join(install_path, "VC", "Auxiliary", "Build", "vcvars64.bat")
        if not os.path.isfile(vcvarsall):
            raise RuntimeError(f"vcvars64.bat not found at {vcvarsall}")
        return vcvarsall

    @staticmethod
    def run_in_vs_env(commands):
        vcvarsall = PathFinder.find_vcvarsall()
        full_cmd = f'"{vcvarsall}" && ' + " && ".join(commands)
        print(f"Running in VS environment: {full_cmd}")
        subprocess.check_call(full_cmd, shell=True)